/*
	ARAMv0, the minimalistic Audio Recorder And Music
	Copyright (C) 2016 Eigil Hysvær

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "buffer.h"
#include <iostream>
#include <exception>
#include <algorithm>

namespace aram
{

	/*
	 *  double_buffer
	 */
	double_buffer::double_buffer(int buffer_size) : b_idx(0), bbuf_ready(true)
	{
		dbuf[0] = new buf_t(buffer_size);
		dbuf[1] = new buf_t(buffer_size);
		swap(); //Initialize the iterators!
	}

	double_buffer::~double_buffer()
	{
		delete dbuf[1];
		delete dbuf[0];
	}

	buf_t* double_buffer::front_buffer()
	{
		return dbuf[b_idx];
	}

	buf_t* double_buffer::back_buffer()
	{
		return dbuf[(b_idx + 1)&1];
	}

	void double_buffer::swap()
	{
		lock_guard<mutex> guard(swap_safe);
		++b_idx &= 1;

		fbuf_itr = front_buffer()->begin();
		bbuf_ready = true;
	}

	/*
	 *  load_and_read_buffer
	 */
	load_and_read_buffer::load_and_read_buffer(int siz) : double_buffer(siz)
	{
	}

	bool load_and_read_buffer::read_front_buffer(sample_t* to_buf, int len)
	{
		bool did_swap;
		buf_itr_t end_itr = front_buffer()->end();
		if(fbuf_itr + len < end_itr)
		{
			copy(fbuf_itr, fbuf_itr + len, to_buf);
			fbuf_itr += len;
			did_swap = false;
		}
		else
		{
			int cutlen = end_itr - fbuf_itr;
			int remaining = len - cutlen;
			copy(fbuf_itr, end_itr, to_buf);
			swap();
			copy(fbuf_itr, fbuf_itr + remaining, &to_buf[cutlen]);
			fbuf_itr += remaining;
			did_swap = true;
		}
		return did_swap;
	}

	int load_and_read_buffer::load_back_buffer(forward_list<istream*>& istr_list)
	{
		if(!bbuf_ready)
		{
			return 0;
		}
		buf_itr_t end_itr;
		buf_itr_t bbuf_itr;
		{
			lock_guard<mutex> guard(swap_safe);
			end_itr = back_buffer()->end();
			bbuf_itr = back_buffer()->begin();
		}

		int total_len = 0;
		int fillsize = end_itr - bbuf_itr;

		while(!istr_list.empty())
		{
			istream& istr = *istr_list.front();

			int stream_pos = istr.tellg();
			istr.seekg(0, ios::end);
			int stream_end = istr.tellg();
			istr.seekg(stream_pos, ios::beg);

			int len = end_itr - bbuf_itr;
			if(len > (stream_end - stream_pos) / sizeof (sample_t))
			{
				len = (stream_end - stream_pos) / sizeof (sample_t);
			}
			total_len += len;

			istr.read(reinterpret_cast<char*>(&(*bbuf_itr)), len * sizeof (sample_t));
			bbuf_itr += len;

			fillsize = end_itr - bbuf_itr;
			if(fillsize == 0)
			{
				//the buffer is filled
				break;
			}
			//We're done with this stream
			istr_list.pop_front();
		}

		bbuf_itr = fill_n(bbuf_itr, fillsize, 0.f);

		if(bbuf_itr != end_itr)
		{
			throw runtime_error("bug in load_back_buffer!");
		}

		bbuf_ready = false;

		return total_len;
	}

	int load_and_read_buffer::load_back_buffer_and_swap(forward_list<istream*>& istr)
	{
		int samples_read = load_back_buffer(istr);
		swap();
		return samples_read;
	}

	/*
	 *  write_and_store_buffer
	 */
	write_and_store_buffer::write_and_store_buffer(int siz) : double_buffer(siz)
	{
	}

	bool write_and_store_buffer::write_front_buffer(sample_t* from_buf, int len)
	{
		buf_itr_t itr;
		{
			lock_guard<mutex> guard(swap_safe);
			if(fbuf_itr + len < front_buffer()->end())
			{
				itr = fbuf_itr;
				fbuf_itr += len;
			}
			else
			{
				return false;
			}
		}

		copy(from_buf, &from_buf[len], &(*itr));

		itr += len;

		return true;
	}

	int write_and_store_buffer::swap_and_store_back_buffer(ostream& ostr)
	{
		int len = fbuf_itr - front_buffer()->begin();
		swap();

		ostr.write(reinterpret_cast<char*>(&(*back_buffer()->begin())), len * sizeof (sample_t));

		return len;
	}

	/*
	 *  single_buffer
	 */
	single_buffer::single_buffer(istream& istr)
	{
		istr.seekg(0, ios::end);
		int len = istr.tellg() / sizeof (sample_t);
		istr.seekg(0, ios::beg);

		buf = new buf_t(len);
		reset();
		istr.read(reinterpret_cast<char*>(&(*buf_itr)), len * sizeof (sample_t));
	}

	void single_buffer::read(sample_t* to, int len)
	{
		if(buf_itr + len > buf->end())
		{
			throw "out of bounds"; //maybe better to fill w zeros?
		}
		copy(buf_itr, buf_itr + len, to);
		buf_itr += len;
	}

	void single_buffer::reset()
	{
		buf_itr = buf->begin();
	}
}

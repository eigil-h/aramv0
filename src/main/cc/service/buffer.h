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

#ifndef ARAM_BUFFERS_H
#define ARAM_BUFFERS_H

#include <array>
#include <vector>
#include <mutex>

using namespace std;

namespace aram
{
	typedef float sample_t;
	typedef vector<sample_t> buf_t;
	typedef buf_t::iterator buf_itr_t;

	class double_buffer
	{
	protected:
		array<buf_t*, 2> dbuf;
		buf_itr_t fbuf_itr;
		bool bbuf_ready;
		int b_idx;
		mutex swap_safe;

		buf_t* front_buffer();
		buf_t* back_buffer();
		void swap();

	public:
		/**
		 * Constructor, allocate buffers.
		 * @param len Length of each buffer.
		 */
		double_buffer(int siz);

		/**
		 * Free buffers.
		 */
		virtual ~double_buffer();
	};

	class load_and_read_buffer : public double_buffer
	{
	public:

		load_and_read_buffer(int siz);

		/**
		 * Copy len floats to given buffer. Swaps the front/back buffers if necessary.
		 * Note, len > buffers is not considered. This class' intention is to be used as
		 * a large buffer for some smaller real time/hardware buffers.
		 * Return true if there was a swap.
		 */
		bool read_front_buffer(sample_t* to_buf, int len);

		/**
		 * Fill up back buffer. Excess buffer space are filled with 0.
		 * Return number of actual samples read from streams.
		 */
		int load_back_buffer(istream& istr);

		/**
		 * Use first time so that there's data to read in the front buffer.
		 * Return number of actual samples read from streams.
		 */
		int load_back_buffer_and_swap(istream& istr);
	};

	class write_and_store_buffer : public double_buffer
	{
	public:

		write_and_store_buffer(int siz);

		/**
		 * Write len number of samples to front buffer.istr_list
		 * Return true if it could write all.
		 * Note - if false it means that there are samples that never will be stored.
		 */
		bool write_front_buffer(sample_t* from_buf, int len);

		/**
		 * Swap the buffers and write to ostr as much as was written to it.
		 * Return number of samples stored.
		 */
		int swap_and_store_back_buffer(ostream& ostr);
	};

	class single_buffer
	{
		buf_t* buf;
		buf_itr_t buf_itr;

	public:
		single_buffer(istream& istr);
		void read(sample_t* to, int len);
		void reset();
	};
}

#endif

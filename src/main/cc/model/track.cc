/*
	ARAMv0, the minimalistic Audio Recorder And Music
	Copyright (C) 2016-2017 Eigil Hysvær

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

#include "track.h"
#include "../service/system.h"
#include "../service/audio_engine.h"
#include <iostream>
#include <chrono>

#define BUFFER_SIZ 512*1024

namespace aram
{

	track::track(const string& name, const string& title_name)
	: name_(name),
	track_directory_(system::data_path() + "/" + title_name + "/" + name),
	playback_buffer_left_(new load_and_read_buffer(BUFFER_SIZ)),
	playback_buffer_right_(new load_and_read_buffer(BUFFER_SIZ)),
	recording_buffer_left_(new write_and_store_buffer(BUFFER_SIZ)),
	recording_buffer_right_(new write_and_store_buffer(BUFFER_SIZ)),
	is_recording_(false),
	is_playback_(false)
	{
		system::mkdir(track_directory_);
	}

	track::~track()
	{
	}

	const string& track::name() const
	{
		return name_;
	}

	void track::prepare_recording()
	{
		sample_rate_ = audio_engine::instance().sample_rate(); //todo not stored anywhere!
		is_recording_ = true;

		swap_and_store_thread_ = shared_ptr<thread>(new thread([ = ]{
			swap_and_store_handler();
		}));

		audio_engine::instance().register_recording_buffers(recording_buffer_left_,
						recording_buffer_right_);
	}

	void track::cleanup()
	{
		if(is_recording_)
		{
			is_recording_ = false;
			swap_and_store_thread_->join();
		}

		if(is_playback_)
		{
			is_playback_ = false;
			load_and_read_thread_->join();
		}
	}

	void track::swap_and_store_handler() const
	{
		ofstream left_channel(track_directory_ + "/left", ios::binary);
		ofstream right_channel(track_directory_ + "/right", ios::binary);

		while(is_recording_)
		{
			this_thread::sleep_for(chrono::seconds(1));
			recording_buffer_left_->swap_and_store_back_buffer(left_channel);
			recording_buffer_right_->swap_and_store_back_buffer(right_channel);
		}
	}

	void track::prepare_playback()
	{
		load_and_read_thread_ = shared_ptr<thread>(new thread([=]
		{
			ifstream left_channel(track_directory_ + "/left", ios::binary);
			ifstream right_channel(track_directory_ + "/right", ios::binary);

			playback_buffer_left_->load_back_buffer_and_swap(left_channel);
			playback_buffer_right_->load_back_buffer_and_swap(right_channel);

			is_playback_ = true;

			while(is_playback_)
			{
				this_thread::sleep_for(chrono::seconds(1));
				
				playback_buffer_left_->load_back_buffer(left_channel);
				playback_buffer_right_->load_back_buffer(right_channel);
			}
		}));

		audio_engine::instance().register_playback_buffers(playback_buffer_left_,
						playback_buffer_right_);
	}
}

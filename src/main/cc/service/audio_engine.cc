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

#include "audio_engine.h"
#include "jack_engine.h"
#include "silence_engine.h"
#include "helper.h"

#include <memory>
#include <iostream>
#include <cstring>

using namespace std;

namespace aram
{

	audio_engine& audio_engine::instance()
	{
		static unique_ptr<audio_engine> audio_engine_singleton(assemble_new());
		return *audio_engine_singleton;
	}

	audio_engine* audio_engine::assemble_new()
	{
		if(program_args::instance().audio_engine == "jack")
			return new jack_engine();
		else if(program_args::instance().audio_engine == "silence")
			return new silence_engine();
		else throw runtime_error("No such audio engine");
	}

	audio_engine::audio_engine()
	: running_(false), recording_(false), num_playback_tracks_(0)
	{
	}

	audio_engine::~audio_engine()
	{
	}

	void audio_engine::register_recording_buffers(
					shared_ptr<write_and_store_buffer> left,
					shared_ptr<write_and_store_buffer> right)
	{
		recording_left_buffer_ = left;
		recording_right_buffer_ = right;
		recording_ = true;
	}

	void audio_engine::register_playback_buffers(
					shared_ptr<load_and_read_buffer> left,
					shared_ptr<load_and_read_buffer> right)
	{
		playback_left_buffer_vector_.push_back(left);
		playback_right_buffer_vector_.push_back(right);
		num_playback_tracks_++;
	}

	void audio_engine::unregister_buffers()
	{
		recording_ = false;
		num_playback_tracks_ = 0;
		playback_left_buffer_vector_.clear();
		playback_right_buffer_vector_.clear();
		recording_left_buffer_.reset();
		recording_right_buffer_.reset();
	}

	void audio_engine::start()
	{
		running_ = true;
	}

	void audio_engine::stop()
	{
		running_ = false;
	}

	void audio_engine::on_frame_ready(unsigned frame_count,
					sample_t* capture_left,
					sample_t* capture_right,
					sample_t* playback_left,
					sample_t* playback_right)
	{
		if(running_)
		{
			if(recording_)
			{
				if(!recording_left_buffer_->write_front_buffer(capture_left, frame_count))
				{
					cout << "abort!" << endl;
				}
				if(!recording_right_buffer_->write_front_buffer(capture_right, frame_count))
				{
					cout << "abort! TODO - make recording stop in a clean and nice way" << endl;
				}
			}

			//mix (future: let something else do the mixing, ie make ports per track)
			for(unsigned i = 0; i < frame_count; i++)
			{
				sample_t sample;
				sample_t mixed_sample = 0.0f;
				for(shared_ptr<load_and_read_buffer> l : playback_left_buffer_vector_)
				{
					l->read_front_buffer(&sample, 1);
					mixed_sample += sample;
				}
				mixed_sample += *capture_left++;
				*playback_left++ = mixed_sample / (num_playback_tracks_ + 1);

				mixed_sample = 0.0f;
				for(shared_ptr<load_and_read_buffer> r : playback_right_buffer_vector_)
				{
					r->read_front_buffer(&sample, 1);
					mixed_sample += sample;
				}
				mixed_sample += *capture_right++;
				*playback_right++ = mixed_sample / (num_playback_tracks_ + 1);
			}
		}
		else
		{
			//unconditionally copy input buffer to output buffer for immediate playback
			::memcpy(playback_left, capture_left, sizeof(float) * frame_count);
			::memcpy(playback_right, capture_right, sizeof(float) * frame_count);
		}
	}
}

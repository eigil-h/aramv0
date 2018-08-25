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
}

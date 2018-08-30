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

#ifndef ARAM_AUDIO_ENGINE_H
#define	ARAM_AUDIO_ENGINE_H

#include "buffer.h"
#include <memory>
using namespace std;

namespace aram
{

	class audio_engine
	{
	public:
		static audio_engine& instance();
		virtual ~audio_engine();
		
		virtual unsigned sample_rate() = 0;

		void register_recording_buffers(shared_ptr<write_and_store_buffer> left, shared_ptr<write_and_store_buffer> right);
		void register_playback_buffers(shared_ptr<load_and_read_buffer> left, shared_ptr<load_and_read_buffer> right);
		void unregister_buffers();

		void start();
		void stop();
		
	protected:
		bool running_;
		bool recording_;
		int num_playback_tracks_;
		vector<shared_ptr<load_and_read_buffer>> playback_left_buffer_vector_;
		vector<shared_ptr<load_and_read_buffer>> playback_right_buffer_vector_;
		shared_ptr<write_and_store_buffer> recording_left_buffer_;
		shared_ptr<write_and_store_buffer> recording_right_buffer_;

		audio_engine();
		audio_engine(const audio_engine&) = delete;
		audio_engine& operator=(const audio_engine&) = delete;

		void on_frame_ready(unsigned frame_count,
												sample_t* capture_left,
												sample_t* capture_right,
												sample_t* playback_left,
												sample_t* playback_right);

	private:
		static audio_engine* assemble_new();
	};
}
#endif

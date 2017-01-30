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

#ifndef ARAM_TRACK_H
#define	ARAM_TRACK_H

#include "../service/buffer.h"
#include <string>
#include <memory>
#include <thread>
#include <fstream>

using namespace std;

namespace aram
{
	class track
	{
	public:
		track(const string& name, const string& title_name);
		virtual ~track();

		const string& name() const;
		void prepare_recording();
		void swap_and_store_handler() const;
		void prepare_playback();
		void cleanup();

	private:
		string name_;
		string track_directory_;
		shared_ptr<load_and_read_buffer> playback_buffer_left_;
		shared_ptr<load_and_read_buffer> playback_buffer_right_;
		shared_ptr<write_and_store_buffer> recording_buffer_left_;
		shared_ptr<write_and_store_buffer> recording_buffer_right_;
		bool is_recording_;
		bool is_playback_;
		shared_ptr<thread> swap_and_store_thread_;
		shared_ptr<thread> load_and_read_thread_;
		int sample_rate_;
	};
}

#endif

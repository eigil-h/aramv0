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
#include "signal.h"
#include <iostream>

namespace aram
{

	signal& signal::instance()
	{
		static signal sig;
		return sig;
	}

	signal::signal()
	{
		frame_ready.connect(sigc::mem_fun(this, &signal::on_frame_ready));
	}

	void signal::on_frame_ready(unsigned frame_count)
	{
		string n = "frame ready@" + to_string(frame_count);
		stats[n.c_str()]++;
	}

	void signal::on_xrun()
	{
		stats["xrun"]++;
	}

	void signal::on_sample_rate(unsigned new_sample_rate)
	{
		string name = "sample rate@" + to_string(new_sample_rate);
		stats[name.c_str()]++;
	}

	void signal::on_audio_engine_shutdown()
	{
		stats["audio engine_shutdown"]++;
	}

	void signal::on_error(const char* msg)
	{
		string name = string("error@") + msg;
		stats[name.c_str()]++;
	}

	void signal::on_start_record(const char* name)
	{
		string n = string("start recording@") + name;
		stats[n.c_str()]++;
	}

	void signal::on_stop_record()
	{
		stats["stop recording"]++;
	}

	void signal::on_start_playback(const char* name)
	{
		string n = string("start playback@") + name;
		stats[n.c_str()]++;
	}

	void signal::on_stop_playback()
	{
		stats["stop playback"]++;
	}

	void signal::print_stats()
	{
		cout << "stats:\n";
		for(auto const& stat : stats)
		{
			cout << "\t" << stat.first << ":\t" << stat.second << "\n";
		}
		cout << endl;
	}
}

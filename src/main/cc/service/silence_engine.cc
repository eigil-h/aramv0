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

#include "silence_engine.h"
#include "signal.h"
#include <iostream>
#include <memory>

aram::silence_engine::silence_engine() :
				mainturbo_thread(&silence_engine::mainturbo, this)
{
	cout << "Constructing the Silence Audio Engine" << endl;

	/* Connect callbacks
	 */
	signal::instance().frame_ready.connect(sigc::mem_fun(this, &silence_engine::on_frame_ready));
}

aram::silence_engine::~silence_engine()
{
	mainturbo_thread.detach();
	cout << "Silence Audio Engine destructed" << endl;
}

unsigned aram::silence_engine::sample_rate() 
{
	return 10;
}

void aram::silence_engine::mainturbo() 
{
	while(true)
	{
		auto until = std::chrono::steady_clock::now() + std::chrono::milliseconds(1000);
		signal::instance().frame_ready(10);
		this_thread::sleep_until(until);
	}
}

void aram::silence_engine::on_frame_ready(unsigned frame_count) 
{ 
	unique_ptr<sample_t[]> capture_left(new sample_t[frame_count]);
	unique_ptr < sample_t[] > capture_right(new sample_t[frame_count]);
	unique_ptr < sample_t[] > playback_left(new sample_t[frame_count]);
	unique_ptr < sample_t[] > playback_right(new sample_t[frame_count]);

	audio_engine::on_frame_ready(frame_count,
					&capture_left[0],
					&capture_right[0],
					&playback_left[0],
					&playback_right[0]);
}
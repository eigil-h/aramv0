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
	cout << "Silence Audio Engine destructed" << endl;
}

unsigned aram::silence_engine::sample_rate() 
{
	return 48000;
}

void aram::silence_engine::mainturbo() 
{
	while(running_)
	{
		signal::instance().frame_ready(1);
		this_thread::sleep_for(chrono::milliseconds(50));
	}
}

void aram::silence_engine::on_frame_ready(unsigned frame_count) { }
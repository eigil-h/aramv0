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

#ifndef ARAM_SILENCE_ENGINE_H
#define ARAM_SILENCE_ENGINE_H

#include "audio_engine.h"
#include <thread>

using namespace std;

namespace aram
{

	class silence_engine : protected audio_engine
	{
	public:
		silence_engine();
		~silence_engine();

		unsigned sample_rate();

	private:
		thread mainturbo_thread;


		void mainturbo();
		void on_frame_ready(unsigned frame_count);
	};

	//need a thread to trigger 'on frame ready' 
}

#endif


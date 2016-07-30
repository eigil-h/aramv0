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

#ifndef ARAM_SIGNAL_H
#define	ARAM_SIGNAL_H

#include <sigc++/sigc++.h>

namespace aram
{
	class signal
	{
	public:
		static signal& instance();

		sigc::signal<void, unsigned> frame_ready;
		sigc::signal<void> xrun;
		sigc::signal<void, unsigned> sample_rate;
		sigc::signal<void> audio_engine_shutdown;
		sigc::signal<void, const char*> error;

		sigc::signal<void, const char*> start_record;
		sigc::signal<void> stop_record;
		sigc::signal<void, const char*> start_playback;
		sigc::signal<void> stop_playback;

	private:
		signal();
		signal(const signal&) = delete;
		signal& operator=(const signal&) = delete;
	};
}

#endif


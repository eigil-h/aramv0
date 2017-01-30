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

#ifndef ARAM_JACK_ENGINE_H
#define	ARAM_JACK_ENGINE_H

#define JACK_CALLBACK_SUCCESS 0;

#include "audio_engine.h"
#include <jack/jack.h>

namespace aram
{

	class jack_engine : protected audio_engine
	{
	public:
		jack_engine();
		virtual ~jack_engine();

		unsigned sample_rate();
		
	private:
		jack_client_t* jack_client_;
		jack_port_t* capture_left_;
		jack_port_t* capture_right_;
		jack_port_t* playback_left_;
		jack_port_t* playback_right_;

		void on_frame_ready(unsigned frameCount);
	};

	/*
	 * Wrapper for jack_get_ports()
	 */
	class jack_get_ports
	{
		const char** ports_;
		unsigned ports_size_;

	public:
		jack_get_ports(jack_client_t* jack_client, unsigned long flags);
		virtual ~jack_get_ports();

		unsigned ports_size();
		const char* port_at(unsigned port_index);
	};
}

#endif

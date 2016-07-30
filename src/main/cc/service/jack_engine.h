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

namespace aram
{

	class jack_engine : protected audio_engine
	{
	public:
		jack_engine();
		jack_engine(const jack_engine& orig);
		virtual ~jack_engine();
	private:

	};
}

#endif

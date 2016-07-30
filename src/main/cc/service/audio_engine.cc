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

#include "audio_engine.h"
#include "jack_engine.h"

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
		//Here's the opportunity to create various implementations of audio_engine
		return new jack_engine();		
	}

	audio_engine::audio_engine()
	{
	}

	audio_engine::~audio_engine()
	{
	}
}

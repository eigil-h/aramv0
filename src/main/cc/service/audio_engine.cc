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
#include "signal.h"
#include <memory>

using namespace std;


/* JACK c-style callbacks
 */
static int on_frame_ready_jack_fun(uint32_t frame_count, void* ignore)
{
	aram::signal::frame_ready(frame_count);
	return JACK_CALLBACK_SUCCESS;
}

static int on_xrun_jack_fun(void* ignore)
{
	aram::signal::xrun();
	return JACK_CALLBACK_SUCCESS;
}

static int on_sample_rate_change_jack_fun(unsigned sample_rate, void* ignore)
{
	aram::signal::sample_rate(sample_rate);
	return JACK_CALLBACK_SUCCESS;
}

static void on_shutdown_jack_fun(void* ignore)
{
	aram::signal::audio_engine_shutdown();
}

static void on_error_jack_fun(const char* msg)
{
	aram::signal::error(msg);
}


namespace aram
{
	audio_engine& audio_engine::instance()
	{
		static unique_ptr<audio_engine> audio_engine_singleton(assemble_new());
		return *audio_engine_singleton;
	}

	audio_engine* audio_engine::assemble_new()
	{
		return new audio_engine();		
	}

	audio_engine::audio_engine()
	{
	}

	audio_engine::~audio_engine()
	{
	}
}

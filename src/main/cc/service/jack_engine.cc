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

#include "jack_engine.h"
#include "signal.h"

#include <memory>
#include <cstring>

using namespace std;

/* JACK c-style callbacks
 */
static int on_frame_ready_jack_fun(uint32_t frame_count, void* ignore)
{
	aram::signal::instance().frame_ready(frame_count);
	return JACK_CALLBACK_SUCCESS;
}

static int on_xrun_jack_fun(void* ignore)
{
	aram::signal::instance().xrun();
	return JACK_CALLBACK_SUCCESS;
}

static int on_sample_rate_change_jack_fun(unsigned sample_rate, void* ignore)
{
	aram::signal::instance().sample_rate(sample_rate);
	return JACK_CALLBACK_SUCCESS;
}

static void on_shutdown_jack_fun(void* ignore)
{
	aram::signal::instance().audio_engine_shutdown();
}

static void on_error_jack_fun(const char* msg)
{
	aram::signal::instance().error(msg);
}


namespace aram
{

	jack_engine::jack_engine()
	{
		/* Initialize
		 */
		jack_set_error_function(on_error_jack_fun);
		jack_status_t status;
		jack_client_ = jack_client_open("aram", JackNullOption, &status);
		if(jack_client_ == nullptr)
		{
			throw runtime_error("Jack server is not running.");
		}

		jack_set_process_callback(jack_client_, on_frame_ready_jack_fun, this);
		jack_set_xrun_callback(jack_client_, on_xrun_jack_fun, this);
		jack_set_sample_rate_callback(jack_client_, on_sample_rate_change_jack_fun, this);
		jack_on_shutdown(jack_client_, on_shutdown_jack_fun, this);

		/* Register ports
		 */
		capture_left_ = jack_port_register(jack_client_, "aram-capture-left", JACK_DEFAULT_AUDIO_TYPE, 
						JackPortIsInput, 0L);
		capture_right_ = jack_port_register(jack_client_, "aram-capture-right", JACK_DEFAULT_AUDIO_TYPE,
						JackPortIsInput, 0L);
		playback_left_ = jack_port_register(jack_client_, "aram-playback-left", JACK_DEFAULT_AUDIO_TYPE,
						JackPortIsOutput, 0L);
		playback_right_ = jack_port_register(jack_client_, "aram-playback-right", JACK_DEFAULT_AUDIO_TYPE,
						JackPortIsOutput, 0L);

		/* Activate
		 */
		int error_code = jack_activate(jack_client_);
		if(error_code != 0)
		{
			throw runtime_error("Jack activation failed with error " + error_code);
		}

		/* Connect ports
		 */
		aram::jack_get_ports capture_ports(jack_client_, JackPortIsPhysical | JackPortIsOutput);

		if(capture_ports.ports_size() == 2)
		{
			if(jack_connect(jack_client_, capture_ports.port_at(0), jack_port_name(capture_left_)) != 0)
			{
				throw runtime_error("Can't connect ports");
			}
			if(jack_connect(jack_client_, capture_ports.port_at(1), jack_port_name(capture_right_)) != 0)
			{
				throw runtime_error("Can't connect ports");
			}
		} else {
			throw runtime_error("Unexpected number of physical capture ports: " + capture_ports.ports_size());			
		}


		aram::jack_get_ports playback_ports(jack_client_, JackPortIsPhysical | JackPortIsInput);

		if(playback_ports.ports_size() == 2)
		{
			if(jack_connect(jack_client_, jack_port_name(playback_left_), playback_ports.port_at(0)) != 0)
			{
				throw runtime_error("Can't connect ports");
			}
			if(jack_connect(jack_client_, jack_port_name(playback_right_), playback_ports.port_at(1)) != 0)
			{
				throw runtime_error("Can't connect ports");
			}
		}
		else
		{
			throw runtime_error("Unexpected number of physical playback ports: " + playback_ports.ports_size());
		}

		/* Connect callbacks
		 */
		signal::instance().frame_ready.connect(sigc::mem_fun(this, &jack_engine::on_frame_ready));
	}

	jack_engine::~jack_engine()
	{
	}

	void jack_engine::on_frame_ready(unsigned frame_count)
	{
		float* capture_left   = reinterpret_cast<float*>(jack_port_get_buffer(capture_left_, frame_count));
		float* capture_right  = reinterpret_cast<float*>(jack_port_get_buffer(capture_right_, frame_count));
		float* playback_left  = reinterpret_cast<float*>(jack_port_get_buffer(playback_left_, frame_count));
		float* playback_right = reinterpret_cast<float*>(jack_port_get_buffer(playback_right_, frame_count));

		//unconditionally copy input buffer to output buffer for immediate playback
		::memcpy(playback_left, capture_left, sizeof (float) * frame_count);
		::memcpy(playback_right, capture_right, sizeof (float) * frame_count);
	}

	/* 
	 * jack_get_ports
	 */

	jack_get_ports::jack_get_ports(jack_client_t* jack_client, unsigned long flags)
	: ports_size_(0)
	{
		ports_ = ::jack_get_ports(jack_client, nullptr, nullptr, flags);
		if(ports_ == nullptr)
		{
			throw runtime_error("Can't connect to physical ports");
		}
		while(ports_[ports_size_] != nullptr)
		{
			ports_size_++;
		}
	}

	jack_get_ports::~jack_get_ports()
	{
		jack_free(ports_);
	}

	unsigned jack_get_ports::ports_size()
	{
		return ports_size_;
	}

	const char* jack_get_ports::port_at(unsigned port_index)
	{
		if(port_index >= ports_size_)
		{
			throw runtime_error(port_index + "-> No such port! Number of ports are " + ports_size_);
		}
		return ports_[port_index];
	}
}
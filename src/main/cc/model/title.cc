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

#include "title.h"
#include "../service/system.h"
#include "../service/signal.h"
#include "src/main/cc/service/audio_engine.h"
#include <iostream>
#include <sigc++-2.0/sigc++/signal.h>

namespace aram
{

	title::title(const string& name)
	{
		name_ = name;
		system::mkdir(system::data_path() + "/" + name);

		for(string track_name : system::directories(system::data_path() + "/" + name))
		{
			tracks_.emplace_back(track_name, name);
		}

		signal::instance().start_record.connect(sigc::mem_fun(this, &title::start_recording));
		signal::instance().stop_record.connect(sigc::mem_fun(this, &title::stop));
	}

	title::~title()
	{
	}

	const string& title::name() const
	{
		return name_;
	}

	const vector<track>& title::tracks() const
	{
		return tracks_;
	}

	vector<string> title::find_all()
	{
		vector<string> titles;
		for(string title_name : system::directories(system::data_path()))
		{
			titles.push_back(title_name);
		}
		return titles;
	}

	void title::start_recording(const string& track_name)
	{
		cout << "start record track " << track_name << " for " << name() << endl;

		//if it didn't already exist
		tracks_.emplace_back(track_name, name_);

		for(track& t : tracks_)
		{
			if(t.name() == track_name) {
				t.prepare_recording();
			} else {
				t.prepare_playback();
			}
		}
		
		audio_engine::instance().start();
	}

	void title::start_playback()
	{
		cout << "start playback " << name() << endl;

		for(track& t : tracks_)
		{
			t.prepare_playback();
		}
		
		cout << "starting audio engine" << endl;

		audio_engine::instance().start();
	}

	void title::stop()
	{
		cout << "stop playback/recording " << name() << endl;
		//stop recording and playback
		audio_engine::instance().stop();
		this_thread::sleep_for(chrono::milliseconds(300)); //until better solution
		for(track& t : tracks_)
		{
			t.cleanup();
		}
		audio_engine::instance().unregister_buffers();
	}
}
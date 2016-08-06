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

#include "title.h"
#include "../service/system.h"
#include "../service/signal.h"
#include <iostream>
#include <sigc++-2.0/sigc++/signal.h>

namespace aram
{

	title::title(const string& name) : cursor_(0L)
	{
		name_ = name;
		system::mkdir(system::data_path() + "/" + name);

		for(string track_name : system::directories(system::data_path() + "/" + name))
		{
			tracks_.emplace_back(track_name);
		}

		signal::instance().start_record.connect(sigc::mem_fun(this, &title::start_record));
		signal::instance().stop_record.connect(sigc::mem_fun(this, &title::stop_record));
		signal::instance().frame_ready.connect(sigc::mem_fun(this, &title::increment_cursor));
	}

	title::~title()
	{
	}

	void title::add_track(const string& track_name)
	{
		tracks_.emplace_back(track_name);
		system::mkdir(system::data_path() + "/" + name_ + "/" + track_name);
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

	void title::start_record(const string& track_name)
	{
		//playback previously recorded tracks while recording this new one
		cout << "start record track " << track_name << " for " << name() << endl;
	}

	void title::stop_record()
	{
		//stop recording and playback
	}

	void title::increment_cursor(unsigned i)
	{
		cursor_ += i;
	}

	void title::reset_cursor()
	{
		cursor_ = 0L;
	}
}
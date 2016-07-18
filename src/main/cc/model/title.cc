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
#include <iostream>

namespace aram
{

	title::title(const string& name)
	{
		name_ = name;
		system::mkdir(system::data_path() + "/" + name);

		for(string track_name : system::directories(system::data_path() + "/" + name))
		{
			track track(track_name);
			tracks_.push_back(track);
		}
	}

	title::title(const title& orig)
	{
		name_ = orig.name_;
	}

	title::~title()
	{
	}

	void title::add_track(const string& track_name)
	{
		tracks_.push_back(track(track_name));
		system::mkdir(system::data_path() + "/" + name_ + "/" + track_name);
	}
	
	const vector<track>& title::tracks()
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
}
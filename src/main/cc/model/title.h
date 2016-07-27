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

#ifndef ARAM_TITLE_H
#define	ARAM_TITLE_H

#include "track.h"

#include <vector>
#include <string>
using namespace std;

namespace aram
{

	class title
	{
	public:
		title(const string& name);
		title(const title& orig);
		virtual ~title();

		const string& name();
		const vector<track>& tracks();

		void add_track(const string& track_name);
		void start_record(const string& track_name);
		void stop_record();
		void increment_cursor(unsigned i);
		void reset_cursor();

		static vector<string> find_all();

	private:
		string name_;
		vector<track> tracks_;
		long cursor_;
	};
}

#endif


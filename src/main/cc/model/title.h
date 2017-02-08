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
		virtual ~title();

		const string& name() const;
		const vector<track>& tracks() const;

		void start_recording(const string& track_name);
		void start_playback();
		void stop();
		void export_to_wav(const string& wav_directory_path);

		static vector<string> find_all();

	private:
		string name_;
		vector<track> tracks_;
		
		unsigned sample_rate() const;
		void ck_info_file() const;
	};
}

#endif


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

#include "system.h"
#include <sys/stat.h>
#include <cerrno>
#include <cstring>
#include <stdexcept>

namespace aram
{

	const string system::data_path()
	{
		string home_path = getenv("HOME");
		return home_path + "/.aramv0";
	}

	void system::mkdir(const string& path)
	{
		if(::mkdir(path.c_str(), 0700) != 0 && errno != EEXIST)
		{
			throw runtime_error(::strerror(errno));
		}
	}

	vector<string> system::directories(const string& path)
	{
		glob_t glob_result;
		glob((path + "/*").c_str(), GLOB_TILDE | GLOB_ONLYDIR, NULL, &glob_result);
		vector<string> files;
		for(unsigned int i = 0; i < glob_result.gl_pathc; ++i)
		{
			string dir(glob_result.gl_pathv[i]);
			files.push_back(dir.substr(path.size() + 1));
		}
		globfree(&glob_result);
		return files;
	}
}

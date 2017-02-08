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

#include "helper.h"
#include "../service/system.h"
#include <fstream>

namespace aram
{

	properties::properties(const string& path) :
	path_(path)
	{
		if(*this)
		{
			load();
		}
	}

	properties::operator bool() const
	{
		return system::file_exists(path_);
	}

	void properties::load()
	{
		ifstream file(path_);
		if(file)
		{
			string prop;
			while(file >> prop)
			{
				int index = prop.find('=');
				map_.emplace(prop.substr(0, index), prop.substr(index + 1));
			}
		}
		else
		{
			throw runtime_error(path_ + " no good");
		}
	}

	void properties::save()
	{
		ofstream file(path_);
		if(file)
		{
			for(auto const& entry : map_)
			{
				file << entry.first << "=" << entry.second << "\n";
			}
		}
		else
		{
			throw runtime_error(path_ + " no good");
		}
	}

	unsigned properties::get_unsigned_int(const string& key, unsigned default_val)
	{
		if(map_.find(key) == map_.end())
		{
			return default_val;
		}
		return stoi(map_[key]);
	}

	void properties::put_unsigned_int(const string& key, unsigned val)
	{
		map_[key] = to_string(val);
	}
	
	void assert::equals(int expected, int actual, const string& msg)
	{
		if(actual != expected)
		{
			throw runtime_error(msg + " - Expected: " + to_string(expected) + ", actual: " + to_string(actual));
		}
	}

	void assert::equals(const string& expected, const string& actual, const string& msg)
	{
		if(actual != expected)
		{
			throw runtime_error(msg + " - Expected: " + expected + ", actual: " + actual);
		}
	}
}
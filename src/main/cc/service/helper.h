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

#ifndef ARAM_HELPER_H
#define ARAM_HELPER_H

#include <map>
using namespace std;

namespace aram
{
	class properties
	{
	public:
		properties(const string& path);
		properties(const properties&) = delete;
		properties& operator=(const properties&) = delete;
		operator bool() const;

		void load();
		void save();

		unsigned get_unsigned_int(const string& key, unsigned default_val);
		void put_unsigned_int(const string& key, unsigned val);

	private:
		string path_;
		map<string, string> map_;
	};

	/**
	 * The main() function will initialize the fields
	 * Then any other part of the program gets easy access to them.
	 */
	class program_args
	{
	public:
		static program_args& instance();

		string program_name; //name of executable
		string command; //play, record, import, export or list
		string title; //name of the title
		string track; //name of the track
		string path; //for import and export
		string audio_engine; //jack or silence
		
		void print() const;

	private:
		program_args() = default;
		program_args(const program_args&) = delete;
		program_args& operator=(const program_args&) = delete;
	};
	
	class assert
	{
	public:
		static void equals(int expected, int actual, const string& msg);
		static void equals(const string& expected, const string& actual, const string& msg);
	};
}

#endif


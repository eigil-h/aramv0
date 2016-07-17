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

#include "service/system.h"
using namespace aram;

#include <iostream>
#include <cstring>
using namespace std;

void print_about(char*);

int main(int argc, char** argv)
{
	//make hidden data directory in the user's home folder if it doesn't exist.
	system::mkdir(system::home_path() + "/.aramv0");

	switch(argc)
	{
		case 1:
			print_about(argv[0]);
			break;

		case 2:
			cout << "You must specify an action for title " << argv[1] << ". Options are record [track], play or export" << endl;
			break;

		case 3:
			if(strcmp("play", argv[2]) == 0)
			{
				cout << "About to playback " << argv[1] << endl;
			}
			else if(strcmp("record", argv[2]) == 0)
			{
				cout << "List tracks for " << argv[1] << endl;
			}
			else if(strcmp("export", argv[2]) == 0)
			{
				cout << "Export " << argv[1] << " to wav" << endl;
			}
			else
			{
				print_about(argv[0]);
			}
			break;

		case 4:
			if(strcmp("record", argv[2]) == 0)
			{
				cout << "About to record " << argv[3] << " for " << argv[1] << endl;
				break;
			}
		default:
			print_about(argv[0]);
			break;
	}

	cout << endl;
	return 0;
}

void print_about(char* program_name)
{
	//About
	cout << endl
					<< "===  ARAMv0, the minimalistic Audio Recorder And Music ===" << endl
					<< "Copyright (C) 2016 Eigil Hysvær" << endl
					<< "This program comes with ABSOLUTELY NO WARRANTY." << endl
					<< "This is free software, and you are welcome to redistribute it under certain conditions." << endl
					<< "See LICENSE for details." << endl << endl;

	//List of options
	cout << "Valid options are: " << endl
					<< endl
					<< "$ " << program_name<< " <title> record <track>" << endl
					<< "  - Record one named track and add it to title. If the track exists it will be overwritten." << endl
					<< "    Tip: Use this one to create a new title and record a count-in" << endl
					<< endl
					<< "$ " << program_name << " <title> record" << endl
					<< "  - List tracks for title" << endl
					<< endl
					<< "$ " << program_name << " <title> play" << endl
					<< "  - Playback title" << endl
					<< endl
					<< "$ " << program_name << " <title> export" << endl
					<< "  - Export title to wav" << endl
					<< endl
					<< "$ " << program_name << " <title>" << endl
					<< "  - List actions and tracks for title" << endl
					<< endl
					<< "$ " << program_name << "" << endl
					<< "  - Display this" << endl
					<< endl;

	//List of titles
	cout << "list titles" << endl;
}

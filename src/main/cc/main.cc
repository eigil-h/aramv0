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
#include "model/title.h"
#include "service/audio_engine.h"
using namespace aram;

#include <iostream>
#include <cstring>
#include <csignal>
#include <unistd.h>
using namespace std;

void print_about(char*);

static title* currentTitle = nullptr;


static void sigint_handler(int sig)
{
	if(currentTitle != nullptr) 
	{
		currentTitle->stop();
	}
	exit(sig);
}


int main(int argc, char** argv)
{
	signal(SIGINT, sigint_handler);

	//make hidden data directory in the user's home folder if it doesn't exist.
	system::mkdir(system::data_path());

	switch(argc)
	{
		case 1:
			print_about(argv[0]);
			break;

		case 2:
		{
			cout << "You should specify an action for title " << argv[1]
							<< ". Options are record [track], play or export. "
							<< "Current tracks are:" << endl;

			title title(argv[1]);
			for(const track& track : title.tracks())
			{
				cout << "'" << track.name() << "' ";
			}
			cout << endl;

			break;
		}

		case 3:
			if(strcmp("play", argv[2]) == 0)
			{
				cout << "About to playback " << argv[1] << endl;
				title title(argv[1]);
				currentTitle = &title;
				title.start_playback();

				::pause();
			}
			else if(strcmp("record", argv[2]) == 0)
			{
				cout << "You should specify a track to record for title " << argv[1]
								<< ". You may either overwrite a current track or make a new one. "
								<< "Current tracks are:" << endl;
				title title(argv[1]);
				for(const track& track : title.tracks())
				{
					cout << "'" << track.name() << "' ";
				}
				cout << endl;
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

				title title(argv[1]);
				currentTitle = &title;
				title.start_recording(argv[3]);

				::pause();
			}
			else if(strcmp("export", argv[2]) == 0)
			{
				title title(argv[1]);
				title.export_to_wav(argv[3]);

				cout << argv[1] << " exported to " << argv[3] << "/" << argv[1] << ".wav" << endl;
			}
			break;

		default:
			print_about(argv[0]);
			break;
	}

	cout << endl;
	
	::pause();

	return 0;
}

void print_about(char* program_name)
{
	//About
	cout << endl
					<< "===  ARAMv0, the minimalistic Audio Recorder And Music ===" << endl
					<< "Copyright (C) 2016-2017 Eigil Hysvær" << endl
					<< "This program comes with ABSOLUTELY NO WARRANTY." << endl
					<< "This is free software, and you are welcome to redistribute it under certain conditions." << endl
					<< "See LICENSE for details." << endl << endl;

	//List of options
	cout << "Valid options are: " << endl
					<< endl
					<< "$ " << program_name << " <title> record <track>" << endl
					<< "  - Record one named track and add it to title. If the track exists it will be overwritten." << endl
					<< "    Tip: Use this one to create a new title and record a count-in" << endl
					<< endl
					<< "$ " << program_name << " <title> record" << endl
					<< "  - List tracks for title" << endl
					<< endl
					<< "$ " << program_name << " <title> play" << endl
					<< "  - Playback title" << endl
					<< endl
					<< "$ " << program_name << " <title> export <music directory>" << endl
					<< "  - Export title to wav" << endl
					<< endl
					<< "$ " << program_name << " <title>" << endl
					<< "  - List actions and tracks for title" << endl
					<< endl
					<< "$ " << program_name << "" << endl
					<< "  - Display this" << endl
					<< endl;

	//List of titles
	cout << "Current titles:" << endl;
	for(string title : title::find_all())
	{
		cout << "'" << title << "' ";
	}
	cout << endl;
}

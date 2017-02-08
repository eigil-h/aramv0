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

#include "service/system.h"
#include "model/title.h"
#include "service/audio_engine.h"
using namespace aram;

#include <iostream>
#include <cstring>
#include <csignal>
#include <unistd.h>
using namespace std;

typedef struct
{
	string program_name;
	string command;
	string title;
	string track;
	string path;
} args;

static void sigint_handler(int sig);
static args mk_args(int argc, char* argv[]);
static void print_usage(const string&);
static void print_invalid_command();
static void print_titles();
static void print_tracks(const string&);
static void print_license_short();

static title* currentTitle = nullptr;

int main(int argc, char* argv[])
{
	signal(SIGINT, sigint_handler);
	system::mkdir(system::data_path());
	args args = mk_args(argc, argv);

	if(args.command == "play")
	{
		if(!args.title.empty())
		{
			cout << "About to playback " << args.title << endl;
			title title(args.title);
			currentTitle = &title;
			title.start_playback();
			::pause();
		}
		else
		{
			print_invalid_command();
		}
	}
	else if(args.command == "record")
	{
		if(!(args.title.empty() || args.track.empty()))
		{
			cout << "About to record " << args.track << " for " << args.title << endl;
			title title(args.title);
			currentTitle = &title;
			title.start_recording(args.track);
			::pause();
		}
		else
		{
			print_invalid_command();
		}
	}
	else if(args.command == "export")
	{
		if(!(args.title.empty() || args.path.empty()))
		{
			cout << "About to export " << args.title << " to " << args.path << endl;
			title title(args.title);
			title.export_to_wav(args.path);
		}
		else
		{
			print_invalid_command();
		}
	}
	else if(args.command == "import")
	{
		cout << "import not implemented yet" << endl;
	}
	else if(args.command == "list")
	{
		if(args.title.empty())
		{
			print_titles();
		}
		else
		{
			print_tracks(args.title);
		}
	}
	else if(args.command == "help")
	{
		print_usage(args.program_name);
	}
	else if(args.command == "about")
	{
		print_license_short();
	}
	else
	{
		print_invalid_command();
	}

	return 0;
}

static void sigint_handler(int sig)
{
	if(currentTitle != nullptr)
	{
		currentTitle->stop();
	}
	exit(sig);
}

static args mk_args(int argc, char* argv[])
{
	args args;
	args.program_name = argv[0];

	for(int i = 1; i < argc; i++)
	{
		if(args.command.empty())
		{
			if(strcmp("-h", argv[i]) == 0 || strcmp("--help", argv[i]) == 0)
			{
				args.command = "help";
			}
			else if(strcmp("--about", argv[i]) == 0)
			{
				args.command = "about";
			}
			else
			{
				args.command = argv[i];
			}
		}

		if(args.title.empty())
		{
			if(strcmp("-T", argv[i]) == 0)
			{
				if(++i < argc)
				{
					args.title = argv[i];
				}
			}
		}

		if(args.track.empty())
		{
			if(strcmp("-t", argv[i]) == 0)
			{
				if(++i < argc)
				{
					args.track = argv[i];
				}
			}
		}
	}

	if(args.command == "export" || args.command == "import")
	{
		args.path = argv[argc-1];
	}

	return args;
}

static void print_usage(const string& program_name)
{
	//List of options
	cout << "Usage: " << program_name << " COMMAND <arguments> [path]\n"
					<< "  for commands play, record, import, export and list\n\n"
					<< "Available arguments:\n"
					<< " -T NAME\t\tName of the title. Optional for list.\n"
					<< " -t NAME\t\tName of the track for record and import.\n\n"
					<< "Additional usage: " << program_name << " <arguments>\n\n"
					<< "Available arguments:\n"
					<< " -h, --help\t\tprint this text.\n"
					<< "     --about\t\tprint some general information about " << program_name << ".\n"
					<< endl;
}

static void print_invalid_command()
{
	cout << "Invalid command or missing argument. Try -h or --help for further information" << endl;
}

static void print_titles()
{
	cout << "Titles:\n";
	for(string title : title::find_all())
	{
		cout << "  " << title << "\n";
	}
	cout << endl;
}

static void print_tracks(const string& title_name)
{
	title title(title_name);
	cout << "Tracks:\n";

	for(const track& track : title.tracks())
	{
		cout << "  " << track.name() << "\n";
	}
	cout << endl;
}

static void print_license_short()
{
	cout << endl
					<< "===  ARAMv0, the minimalistic Audio Recorder And Music ===" << endl
					<< "Copyright (C) 2016-2017 Eigil Hysvær" << endl
					<< "This program comes with ABSOLUTELY NO WARRANTY." << endl
					<< "This is free software, and you are welcome to redistribute it under certain conditions." << endl
					<< "See LICENSE for details." << endl << endl;
}

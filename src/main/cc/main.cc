#include <iostream>
#include <cstring>
using namespace std;

int main(int argc, char** argv)
{
	cout << endl << "       === The minimalistic aramv0 ===" << endl << endl;

	switch(argc)
	{
		case 1:
			cout << "Valid options are: " << endl;
			cout << "list projects" << endl;
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
			break;

		case 4:
			if(strcmp("record", argv[2]) == 0)
			{
				cout << "About to record " << argv[3] << " for " << argv[1] << endl;
				break;
			}
		default:
			cout << "Valid options are: " << endl;
			break;
	}
	
	cout << endl;
}

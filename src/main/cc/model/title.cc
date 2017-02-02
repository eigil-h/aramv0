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

#include "title.h"
#include "../service/helper.h"
#include "../service/system.h"
#include "../service/signal.h"
#include "src/main/cc/service/audio_engine.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <exception>
#include <forward_list>
#include <sigc++-2.0/sigc++/signal.h>
#include <limits>

namespace aram
{

	title::title(const string& name)
	{
		name_ = name;
		system::mkdir(system::data_path() + "/" + name);
		string info_file = system::data_path() + "/" + name + "/info";
		unsigned engine_sample_rate = audio_engine::instance().sample_rate();
		properties prop(info_file);
		if(prop)
		{
			sample_rate_ = prop.get_unsigned_int("sample_rate", engine_sample_rate);
			if(sample_rate_ != engine_sample_rate)
			{
				cout << "WARNING: Audio engine sample rate is " << engine_sample_rate <<
								", while title sample rate is " << sample_rate_ << endl;
			}
		}
		else
		{
			prop.put_unsigned_int("sample_rate", engine_sample_rate);
			prop.save();
			sample_rate_ = engine_sample_rate;
		}

		cout << "Initial sample rate for " << name << " was " << sample_rate_ << endl;

		for(string track_name : system::directories(system::data_path() + "/" + name))
		{
			tracks_.emplace_back(track_name, name);
		}

		signal::instance().start_record.connect(sigc::mem_fun(this, &title::start_recording));
		signal::instance().stop_record.connect(sigc::mem_fun(this, &title::stop));
	}

	title::~title()
	{
	}

	const string& title::name() const
	{
		return name_;
	}

	const vector<track>& title::tracks() const
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

	void title::start_recording(const string& track_name)
	{
		cout << "start record track " << track_name << " for " << name() << endl;

		//if it didn't already exist
		tracks_.emplace_back(track_name, name_);

		for(track& t : tracks_)
		{
			if(t.name() == track_name)
			{
				t.prepare_recording();
			}
			else
			{
				t.prepare_playback();
			}
		}

		audio_engine::instance().start();
	}

	void title::start_playback()
	{
		cout << "start playback " << name() << endl;

		for(track& t : tracks_)
		{
			t.prepare_playback();
		}

		cout << "starting audio engine" << endl;

		audio_engine::instance().start();
	}

	void title::stop()
	{
		cout << "stop playback/recording " << name() << endl;
		//stop recording and playback
		audio_engine::instance().stop();
		this_thread::sleep_for(chrono::milliseconds(300)); //until better solution
		for(track& t : tracks_)
		{
			t.cleanup();
		}
		audio_engine::instance().unregister_buffers();
	}

#define BYTES_PER_SAMPLE sizeof(int16_t)
#define BITS_PER_SAMPLE 8*BYTES_PER_SAMPLE

	void title::export_to_wav(const string& wav_directory_path)
	{
		ofstream file(wav_directory_path + "/" + name_ + ".wav", ios::binary);
		if(file)
		{
			file.write("RIFF", 4);
			unsigned file_siz = 0;
			file.write((char*)&file_siz, 4);
			file.write("WAVE", 4);
			file.write("fmt ", 4);
			const unsigned fmt_length = 16;
			file.write((char*)&fmt_length, 4);
			const unsigned short audio_format = 1;
			file.write((char*)&audio_format, 2);
			const unsigned short num_channels = 2;
			file.write((char*)&num_channels, 2);
			file.write((char*)&sample_rate_, 4);
			const unsigned byte_rate = sample_rate_ * num_channels * BYTES_PER_SAMPLE;
			file.write((char*)&byte_rate, 4);
			const unsigned short block_align = num_channels * BYTES_PER_SAMPLE;
			file.write((char*)&block_align, 2);
			const unsigned short bits_per_sample = BITS_PER_SAMPLE;
			file.write((char*)&bits_per_sample, 2);
			file.write("data", 4);

			const unsigned num_samples = max_element(tracks_.begin(), tracks_.end(),
							[](track const& a, track const& b)
							{
								return a.num_samples() < b.num_samples();
							})->num_samples();

			const unsigned chunk_size = num_channels * num_samples * BYTES_PER_SAMPLE;
			file.write((char*)&chunk_size, 4);

			forward_list<ifstream> left_channels;
			forward_list<ifstream> right_channels;
			for(track& t : tracks_)
			{
				left_channels.emplace_front(t.path_to_left_channel(), ios::binary);
				right_channels.emplace_front(t.path_to_right_channel(), ios::binary);
			}
			//write data - left and right channel interleaved
			for(unsigned i = 0; i < num_samples; i++)
			{
				sample_t mixed_sample = 0.0f;
				unsigned num_samples = 0;

				for(ifstream& s : left_channels)
				{
					sample_t sample;
					if(!s.read((char*)&sample, sizeof (sample_t)).eof())
					{
						mixed_sample += sample;
						num_samples++;
					}
				}

				if(num_samples > 0)
				{
					int16_t wav_sample = numeric_limits<int16_t>::max() * (mixed_sample / num_samples);
					file.write((char*)&wav_sample, sizeof (int16_t));
				}

				mixed_sample = 0.0f;
				num_samples = 0;

				for(ifstream& s : right_channels)
				{
					sample_t sample;
					if(!s.read((char*)&sample, sizeof (sample_t)).eof())
					{
						mixed_sample += sample;
						num_samples++;
					}
				}

				if(num_samples > 0)
				{
					int16_t wav_sample = numeric_limits<int16_t>::max() * (mixed_sample / num_samples);
					file.write((char*)&wav_sample, sizeof (int16_t));
				}
			}
			

			file_siz = file.tellp();
			file.seekp(4, ios::beg);
			file.write((char*)&file_siz, 4);
		}
		else
		{
			throw runtime_error("Not able to open " + wav_directory_path + "/" + name_ + " for writing");
		}
	}
}

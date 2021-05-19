/*
 * Video Game Prototype
 * Copyright (C) 2021  Raymond Jennings
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include <stdexcept>

#include "audio.h"

using namespace std;

Mix_Chunk *oceansound; // background sound
Mix_Chunk *clicksound; // the cursor moved
Mix_Chunk *bumpsound; // either the cursor or the zoom hit a barrier
Mix_Chunk *zoomsound; // we zoomed in or out

void load_sounds()
{
	clicksound = Mix_LoadWAV("sound/click.wav");
	zoomsound = Mix_LoadWAV("sound/zoom.wav");
	oceansound = Mix_LoadWAV("sound/ocean.wav");
	bumpsound = Mix_LoadWAV("sound/bump.wav");
}

void setup_mixer()
{
	Mix_Init(MIX_INIT_FLAC);

	if (Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 1, 512) < 0) {
		throw runtime_error(Mix_GetError());
	}

	Mix_AllocateChannels(2);
	// 0 = music
	// 1 = ui

	Mix_Volume(0, MIX_MAX_VOLUME / 2);
}

void start_music()
{
	Mix_PlayChannel(0, oceansound, -1);
}

void play_sound(enum sfx s)
{
	switch(s) {
	case sfx_zoom:
		Mix_PlayChannel(1, zoomsound, 0);
		break;

	case sfx_click:
	case sfx_zoom:
	}
}
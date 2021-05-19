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
#ifndef AUDIO_H
#define AUDIO_H

#include <SDL2/SDL_mixer.h>

extern Mix_Chunk *oceansound; // background sound
extern Mix_Chunk *clicksound; // the cursor moved
extern Mix_Chunk *bumpsound; // either the cursor or the zoom hit a barrier
extern Mix_Chunk *zoomsound; // we zoomed in or out

extern void load_sounds();
extern void setup_mixer();
extern void start_music();

enum sfx
{
	sfx_click,
	sfx_bump,
	sfx_zoom
};

extern void play_sound(enum sfx s);

#endif

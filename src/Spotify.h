/*
hcircify - Spotify Now Playing plugin for HexChat

Copyright (C) 2015  M. Richards
Copyright (C) 2014  K. Leonardsen / Equalify.me

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

Contact info: mrichards@gmail.com
*/
/*
hcircify - Spotify Now Playing plugin for HexChat

Copyright (C) 2015  M. Richards
Copyright (C) 2014  K. Leonardsen / Equalify.me

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

Contact info: mrichards@gmail.com
*/
#ifndef  HCIRCIFY_H_INCLUDE
#define  HCIRCIFY_H_INCLUDE

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <string>
#include "SpotifyLookupApi.h"
#include "hexchat-plugin.h"
#include "version.h" 

#define IRCIFY_CMD "IRCIFY"
#define APIV_CMD "APIV"
#define SAVE_CMD "OUTPUT"
#define PORT_CMD "PORT"

#define PREF_OUTPUT "Output"
#define PREF_MSG "Type"
#define PREF_PORT "Port"

#ifdef _WIN64 
#pragma comment(lib, "./lib/SpotifyLookUpAPIx64.lib")

#else
#pragma comment(lib, "./lib/SpotifyLookUpAPIx86.lib")
#endif

static int port_cb(char *word[], char *word_eol[], void *userdata);
static int spotify_cb(char *word[], char *word_eol[], void *userdata);
static int advert_ver(char *word[], char *word_eol[], void *userdata);

extern int chkStatus();
extern int OutputToIRC(char *out);
extern int CreateOutput(char *out, TRACKINFO *ti);

extern int convert_time(char *out, int sec);
extern int proc_color(char *color, int reverse);
extern int LoadAndSave(int saved);

#endif
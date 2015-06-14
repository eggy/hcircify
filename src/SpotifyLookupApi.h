/*
	Equalify - spotify Track info library
	Copyright (C) 2014 K. Leonardsen / Equalify.me

	Redistribution and use in source and binary forms, without modification, are permitted
	provided that the following conditions are met:
	
	* Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
	* Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
	* Neither the name of Equalify.me nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
	
	Any Software using this library	must make their source code available
	to the public, using a GPL or LGPL compatible license.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
	INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
	IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
	OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
	OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
	OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
	POSSIBILITY OF SUCH DAMAGE.

	Contact info: leo@equalify.me
*/

#ifndef SPOTIFYLOOKUPAPI_H_INCLUDE
#define SPOTIFYLOOKUPAPI_H_INCLUDE

//ifdef is to make sure ti doesn't mess with the plugin api headers!
#ifndef TRACKINFO_LOADED
#define TRACKINFO_LOADED
struct SpotifyInfo {
	int			LocApiVersion;
	char		SpVersion[50];
	int			Playing;
	int			Shuffle;
	int			Repeat;
	int			Online;
	int			Running;
};

 struct TRACKINFO {
	char		name[500];			//trackname
	char		URI[500];
	int			length;				//sec lenght of track
	char		artist[20][500];
	char		artistURI[500];
	int			coartists;
	char		album[500];
	char		albumyear[50];
	char		albumURI[500];
	char		redirect[15][50];	//not really used much anymore
	int			redirects;			
	int			currplay;			//sec played
	double		vol;				//0.0 ->1.0
	int			tracktype;			//0=normal, 1=local, 3=ad
	int			IsPrivate;	
	int			IsExplicit;
	char		Popularity[50];
	int			TrackNr;
	char		AlbumArtURL[255];
	char		AlbumFileName[255];
	SpotifyInfo SpInfo;
};
#endif

int GetSongInfo(TRACKINFO *ti,bool DoMeta);
int TimedGetSongInfo(TRACKINFO *ti,bool DoMeta,int MaxWait = 60);
//GetSongInfo is instant, used to get information as soon as possible
//TimedGetSongInfo connects and returns data as soon as something has changed, volume, track, play status, whatever..
//So!.. GetSongInfo should be used once at the start, then run TimedGetSongInfo in a never ending loop (in a thread!) 
//to always keep the trackinfo up to date!

int MetadataLookup( char *uri, TRACKINFO *ti);
void PlayPause();
void NextTrack();
void PrevTrack();
void VolumeUp();
void VolumeDown();
void PlayURI(char *URI,int sec);
void ShuffleToggle();

int GetAlbumArt(TRACKINFO *ti);



DWORD LookupVersion();

#endif
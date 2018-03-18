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

#include "Spotify.h"

static hexchat_plugin *ph;   /* plugin handle */
static char name[] = VER_INTERNAL_NAME_STR;
static char desc[] = VER_FILE_DESCRIPTION_STR;
static char version[] = VER_FILE_VERSION_STR;

static const char helpmsg[] = "Sends currently playing song in Spotify to the current channel. USAGE: /IRCIFY";
static const char apihelpmsg[] = "Display hcircify version information. USAGE: /APIV";
static const char outputmsg[] = "Configure hcircify output. USAGE: /OUTPUT";
static const char portmsg[] = "Configure lookup port. USAGE: /PORT";

DWORD api = LookupVersion();
static char input[500] = { 0 }; //"nP: .song. - (.artist. (.album.)) - [.pbar.] .uri.";
static char	SavedOutputStr[500] = { 0 };
static char	ColorSaveStr[500] = { 0 };
static char PrefOut[500] = { 0 };
static int usemsg = 1;
static int port = NULL;

static int port_cb(char *word[], char *word_eol[], void *userdata) {
	
	int uport = NULL;

	if (!_stricmp("SET", word[2])) {
		uport = atoi(word[3]);
		SetConnectPort(uport);
		hexchat_printf(ph, "%s: Set port to %i", name, uport);
	} else {
		uport = SetConnectPort(port);
		hexchat_printf(ph, "%s: USAGE: /PORT SET <PORT>", name, uport);
		hexchat_printf(ph, "%s: Current port is %i", name, uport);
	}
	
	return HEXCHAT_EAT_ALL;
}

static int spotify_cb(char *word[], char *word_eol[], void *userdata)
{
	int s = SpStatus();
	if(s == 1)
	{
		TRACKINFO ti = { 0 };
		memset(&ti, 0, sizeof(TRACKINFO)); //make sure its nulled every time!

		int sngnfo = GetSongInfo(&ti, 1);
		if (sngnfo == -1){
			return 1;
		}
		if (ti.tracktype == 2) return 1;

		if ((ti.tracktype == 0) || (ti.tracktype == 1) || (ti.SpInfo.Playing == 1)) {
			proc_color(input, 0);
			CreateOutput(ColorSaveStr, &ti);
			OutputToIRC(SavedOutputStr);
		}
	}
	else if (s == 2) {
		hexchat_printf(ph, "%s: Spotify is paused.", name);
	}
	else
	{
		hexchat_printf(ph, "%s: Spotify is not running.", name);
	}
	return HEXCHAT_EAT_ALL;
}

static int advert_ver(char *word[], char *word_eol[], void *userdata){
	if (usemsg == 0)
		hexchat_commandf(ph, "me is using %s v%s (Lib:%x-DLL:%s-%s) - Get yours at http://eggy.cc/ircify/", name, version, api, GitHash, BuildDate);
	else if (usemsg == 1)
		hexchat_commandf(ph, "say %s v%s (Lib:%x-DLL:%s-%s) - Get yours at http://eggy.cc/ircify/", name, version, api, GitHash, BuildDate);
	else
		hexchat_printf(ph, "%s v%s Lib:%x-DLL:%s-%s", name, version, api, GitHash, BuildDate);
	return HEXCHAT_EAT_ALL;
}

static int output_cb(char *word[], char *word_eol[], void *userdata)
{
	int usesave = 1;

	if (!_stricmp("RELOAD", word[2])) {
		usesave = 0;
	}
	else if (!_stricmp("CONFIG", word[2])) {
		hexchat_printf(ph, "%s: %s\\addon_hcircify.conf", name, hexchat_get_info(ph, "configdir"));
		return HEXCHAT_EAT_ALL;
	} else if (!_stricmp("SET", word[2])) {
		usesave = 2;
		sprintf_s(input, 500, word_eol[3]);
	} else if (!_stricmp("TYPE", word[2])) {
		usesave = 2;
		if (!_stricmp("1", word[3])){
			usemsg = 1;
			hexchat_printf(ph, "%s: Message type set 1, using \"/say\".", name);
		} else {
			usemsg = 0;
			hexchat_printf(ph, "%s: Message type set 0, using \"/me\".", name);
		}
	} else if (!_stricmp("SAVE", word[2])) {
		usesave = 1;
		proc_color(input, 1);
		LoadAndSave(usesave);
		hexchat_printf(ph, "%s: Wrote all settings to the config.", name);
		return HEXCHAT_EAT_ALL;
	} else {
		//Output command help
		hexchat_printf(ph, "%s: OUTPUT COMMAND USAGE:", name);
		hexchat_printf(ph, "%s: /OUTPUT SAVE", name);
		hexchat_printf(ph, "%s: /OUTPUT RELOAD", name);
		hexchat_printf(ph, "%s: /OUTPUT CONFIG", name);
		hexchat_printf(ph, "%s: /OUTPUT SET <Output>", name);
		hexchat_printf(ph, "%s: /OUTPUT TYPE <0 or 1>", name);
		hexchat_printf(ph, "%s: Any modified settings will be automatically be saved upon unlaod/exit.", name);
		return HEXCHAT_EAT_ALL;
	}

	if (usesave == 0)	{
		//Reload the preferences from the config
		LoadAndSave(usesave);
		hexchat_printf(ph, "%s: Reloaded settings from the config.", name);
		return HEXCHAT_EAT_ALL;
	} else if (usesave == 1) {
		//Save the preferences to config
		LoadAndSave(usesave);
		hexchat_printf(ph, "%s: Wrote all settings to the config.", name);
		return HEXCHAT_EAT_ALL;
	 } else {
		//Preferences changed, but not written to config
		hexchat_printf(ph, "%s: Settings have been altered in memory but not saved yet.", name);
		hexchat_printf(ph, "%s: Settings are auto saved on unlaod or exit.", name);
		hexchat_printf(ph, "%s: You can always manually save by using /OUTPUT SAVE", name);
		return HEXCHAT_EAT_ALL;
	}
	return HEXCHAT_EAT_ALL;
}

void hexchat_plugin_get_info(char **plugin_name, char **plugin_desc, char **plugin_version, void **reserved) {
	*plugin_name = name;
	*plugin_desc = desc;
	*plugin_version = version;
	(void)reserved;
}

int hexchat_plugin_init(hexchat_plugin *plugin_handle, char **plugin_name, char **plugin_desc, char **plugin_version, char *arg)
{
	/* we need to save this for use with any hexchat_* functions */
	ph = plugin_handle;

	/* tell HexChat our info */
	*plugin_name = name;
	*plugin_desc = desc;
	*plugin_version = version;

	hexchat_hook_command(ph, IRCIFY_CMD, HEXCHAT_PRI_NORM, spotify_cb, helpmsg, 0);
	hexchat_hook_command(ph, APIV_CMD, HEXCHAT_PRI_NORM, advert_ver, apihelpmsg, 0);
	hexchat_hook_command(ph, SAVE_CMD, HEXCHAT_PRI_NORM, output_cb, outputmsg, 0);
	hexchat_hook_command(ph, PORT_CMD, HEXCHAT_PRI_NORM, port_cb, portmsg, 0);

	LoadAndSave(0);

	hexchat_command(ph, "MENU ADD \"Ircify\"");
	hexchat_command(ph, "MENU ADD \"Ircify/Ircify\" \"IRCIFY\"");
	hexchat_command(ph, "MENU ADD \"Ircify/-");
	hexchat_command(ph, "MENU ADD \"Ircify/About\" \"APIV\"");

	hexchat_printf(ph, "%s v%s (Lib:%x-DLL:%s-%s) loaded", name, version, api, GitHash, BuildDate);

	return 1;	/* return 1 for success */
}

int hexchat_plugin_deinit(hexchat_plugin *plugin_handle)
{
	ph = plugin_handle;

	LoadAndSave(1);
	
	hexchat_command(ph, "MENU DEL \"Ircify\"");
	hexchat_printf(ph, "%s v%s (Lib:%x-DLL:%s-%s) unloaded", name, version, api, GitHash, BuildDate);

	return 1;
}

int SpStatus()
{
	if (port == NULL) {
		hexchat_printf(ph, "%s: Contact the developer for support, lookup port did not get set.", name);
		return 1;
	}

	TRACKINFO ti = { 0 };
	GetSongInfo(&ti, 0);

	if (ti.SpInfo.Running == 1) {
		if (ti.SpInfo.Playing == 1) {
			return 1; //playing
		}
		else {
			return 2; //paused
		}
	}
	else {
		return 0; //spotify not running
	}
	return -1; //This should never happen
}

int OutputToIRC(char *out) {
	char *irc;
	std::string ChanModes;
	const char *cmodes = hexchat_get_info(ph, "modes");

	if (cmodes != NULL)
		ChanModes = cmodes;

		if (ChanModes.find("c") != std::string::npos) {
			char *stripped = hexchat_strip(ph, out, -1, 1 | 2);
			if (stripped != NULL) {
				irc = stripped;
			}
			else
			{
				irc = out;
			}
				
			hexchat_free(ph, stripped);
		}
		else {
			irc = out;
		}

	if (usemsg == 0) {
		hexchat_commandf(ph, "me %s", irc);
		}
	else if (usemsg == 1) {
		hexchat_commandf(ph, "say %s", irc);
	}
	else {
		hexchat_printf(ph, "Output: %s\n", irc);
	}
	return 1;
}

int proc_color(char *color, int reverse)
{
	char colorstr[500] = { 0 };
	sprintf_s(colorstr, 600, color);
	std::string ColorData;
	ColorData = colorstr;

	if (!reverse)
	{
		while (ColorData.find("^B") != std::string::npos) {
			ColorData.replace(ColorData.find("^B"), 2, "\002");
		}
		while (ColorData.find("^U") != std::string::npos) {
			ColorData.replace(ColorData.find("^U"), 1, "\01F");
		}
		while (ColorData.find("^") != std::string::npos) {
			ColorData.replace(ColorData.find("^"), 1, "\003");
		}
		strcpy_s(ColorSaveStr, 600, ColorData.c_str());
	}
	else
	{
		while (ColorData.find("\002") != std::string::npos) {
			ColorData.replace(ColorData.find("\002"), 1, "^B");
		}
		while (ColorData.find("\01F") != std::string::npos) {
			ColorData.replace(ColorData.find("\01F"), 1, "^U");
		}
		while (ColorData.find("\003") != std::string::npos) {
			ColorData.replace(ColorData.find("\003"), 1, "^");
		}
		strcpy_s(PrefOut, 600, ColorData.c_str());
	}

	return 1;
}

int convert_time(char *out, int sec){
	if (sec<1) { sprintf_s(out, 30, "00:00"); return 1; }

	int hours = (sec / 60 / 60) % 24;
	int minutes = (sec / 60) % 60;
	int seconds = sec % 60;

	if (sec>3599)
		sprintf_s(out, 30, "%.2i:%.2i:%.2i", hours, minutes, seconds);
	else
		sprintf_s(out, 30, "%.2i:%.2i", minutes, seconds);

	return 1;
}

int CreateOutput(char *out, TRACKINFO *ti)
{
	char outputstr[500] = { 0 };
	sprintf_s(outputstr, 500, out);

	if (strlen(outputstr) < 4){
		sprintf_s(outputstr, 500, "nP: .song. - .artist. (.album.) :: .uri.");
	}

	std::string OutputData;
	OutputData = outputstr;

	if (ti->IsPrivate) {
		if (OutputData.find(".uri.") != std::string::npos)
			OutputData = "nP: .artist. - .song. (.uri.)";
		else
			OutputData = "nP: .song. (Private Mode)";
	}
	if (OutputData.find(".song.") != std::string::npos)
		OutputData.replace(OutputData.find(".song."), 6, ti->name);

	if (OutputData.find(".artist.") != std::string::npos)
		OutputData.replace(OutputData.find(".artist."), 8, ti->artist[0]);

	if (OutputData.find(".album.") != std::string::npos)
		OutputData.replace(OutputData.find(".album."), 7, ti->album);

	if (OutputData.find(".year.") != std::string::npos)
		OutputData.replace(OutputData.find(".year."), 6, ti->albumyear);

	if (OutputData.find(".explicit.") != std::string::npos) {
		if (ti->IsExplicit)
			OutputData.replace(OutputData.find(".explicit."), 10, "Explicit");
		else
			OutputData.replace(OutputData.find(".explicit."), 10, "");
	}
	if (OutputData.find(".uri.") != std::string::npos) {
		if (ti->tracktype == 1)
			OutputData.replace(OutputData.find(".uri."), 5, ti->URI);
		else if (ti->IsPrivate)
			OutputData.replace(OutputData.find(".uri."), 5, "Private Mode");
		else
			OutputData.replace(OutputData.find(".uri."), 5, "");
	}
	if (OutputData.find(".url.") != std::string::npos) {
		if (ti->tracktype == 1) {
			std::string UriPart = ti->URI;
			unsigned found = UriPart.find_last_of(":");
			std::string laststuff = UriPart.substr(found + 1).c_str();
			char UrlCombine[500] = { 0 };

			sprintf_s(UrlCombine, 500, "http://open.spotify.com/track/%s", laststuff.c_str());
			OutputData.replace(OutputData.find(".url."), 5, UrlCombine);
		}
		else if (ti->IsPrivate)
			OutputData.replace(OutputData.find(".url."), 5, "Private Mode");
		else
			OutputData.replace(OutputData.find(".url."), 5, "");
	}

	if (OutputData.find(".time.") != std::string::npos) {
		char PlayedTmp[30] = { 0 };
		convert_time(PlayedTmp, ti->length);
		OutputData.replace(OutputData.find(".time."), 6, PlayedTmp);
	}
	if (OutputData.find(".played.") != std::string::npos) {
		char PlayedTmp[30] = { 0 };
		convert_time(PlayedTmp, ti->currplay);
		OutputData.replace(OutputData.find(".played."), 8, PlayedTmp);
	}

	if (OutputData.find(".pbar.") != std::string::npos) {
		char BarTmp[30] = { 0 };
		int BarLen = (int)(((float)ti->currplay / (float)ti->length) *10.0f);
		if (BarLen <= 10 && BarLen >= 0) {
			memset(BarTmp, '|', BarLen + 1);
			BarTmp[BarLen + 1] = 3;				//yes i know this isn't the shortest code to do this, but its fast!
			BarTmp[BarLen + 2] = '1';
			BarTmp[BarLen + 3] = '4';
			memset(BarTmp + BarLen + 4, ':', 10 - (BarLen + 1));
			BarTmp[13] = 3;
			OutputData.replace(OutputData.find(".pbar."), 6, BarTmp);
		}
	}
	if (OutputData.find(".popularity.") != std::string::npos) {
		char BarTmp[30] = { 0 };
		int BarLen = (int)((float)atof(ti->Popularity)*(float)10.0f);
		if (BarLen <= 10 && BarLen >= 0) {
			memset(BarTmp, '*', BarLen + 1);
			BarTmp[BarLen + 1] = 3;				//yes i know this isn't the shortest code to do this, but its fast!
			BarTmp[BarLen + 2] = '1';
			BarTmp[BarLen + 3] = '4';
			memset(BarTmp + BarLen + 4, '*', 10 - (BarLen + 1));
			BarTmp[13] = 3;
			OutputData.replace(OutputData.find(".popularity."), 12, BarTmp);
		}
	}

	if (OutputData.find(".shuffle.") != std::string::npos) {
		if (ti->SpInfo.Shuffle)
			OutputData.replace(OutputData.find(".shuffle."), 9, "");
		else
			OutputData.replace(OutputData.find(".shuffle."), 9, "");
	}

	if (OutputData.find(".coartists.") != std::string::npos) {
		if (ti->coartists > 1) {
			char CoArtistTmp[300] = { 0 };
			strcat_s(CoArtistTmp, " feat. ");
			strcat_s(CoArtistTmp, ti->artist[1]);
			int i = 1;

			do {
				i++;
				if (i == ti->coartists) { break; }
				strcat_s(CoArtistTmp, ", ");
				strcat_s(CoArtistTmp, ti->artist[i]);
			} while (i < 5);

			if (ti->coartists>4)
				strcat_s(CoArtistTmp, "...");

			OutputData.replace(OutputData.find(".coartists."), 11, CoArtistTmp);
		}
		else {
			OutputData.replace(OutputData.find(".coartists."), 11, "");
		}
	}

	if (OutputData.find("|") != std::string::npos) {
		OutputData.replace(OutputData.find("|"), 1, "\002\002|");		// Replacing some chars that can cause issues
	}																// in mirc if they arent replaced.. Just adding 2x bold 
	if (OutputData.find("%") != std::string::npos) {									// infront of them to make mirc not evaluate them, $eval(...,0) caused issues..
		OutputData.replace(OutputData.find("%"), 1, "\002\002%");
	}
	if (OutputData.find("$") != std::string::npos) {
		OutputData.replace(OutputData.find("$"), 1, "\002\002$");
	}

	strcpy_s(SavedOutputStr, 600, OutputData.c_str());
	return 1;
}

int LoadAndSave(int saved) {
	//Loading and Saving to and from the plugin config
	// 0 = Load, 1 = Save
	char buf[500] = { 0 };
	int prefmsg = 0;
	int lport = NULL;

	if (!saved) {
		//Load from the config
		if (!hexchat_pluginpref_get_str(ph, PREF_OUTPUT, buf))
		{
			sprintf_s(input, 500, "^BnP^B: .song. - .artist. (.album.) :: .uri.");
		}
		else
		{
			if (strlen(buf) < 4)
			{
				sprintf_s(input, 500, "^BnP^B: .song. - .artist. (.album.) :: .uri.");
			}
			else
			{
				sprintf_s(input, 500, buf);
			}
		}

		prefmsg = hexchat_pluginpref_get_int(ph, PREF_MSG);
		if (prefmsg == -1) {
			usemsg = 1;
			hexchat_printf(ph, "%s: Message type set to 1, using \"say\" by default.", name);
		}
		
		lport = hexchat_pluginpref_get_int(ph, PREF_PORT);
		if (lport == -1) {
			lport = SetConnectPort(NULL);
			port = lport;
			hexchat_printf(ph, "%s: Using the default port %i.", name, lport);
		}
		return 1;
	}
	else {
		//Save to the config
		if (strlen(ColorSaveStr) == 0){
			sprintf_s(PrefOut, 500, input);
		}
		else {
			proc_color(input, 1);
		}
		if (!hexchat_pluginpref_set_str(ph, PREF_OUTPUT, PrefOut) ||
			!hexchat_pluginpref_set_int(ph, PREF_MSG, usemsg) ||
			!hexchat_pluginpref_set_int(ph, PREF_PORT, port) )
		{
			hexchat_printf(ph, "%s: failed to save settings", name);
			return 0;
		}
		return 1;
	}
	hexchat_printf(ph, "%s: Something went wrong with saving or loading.", name);
	return 0;
}
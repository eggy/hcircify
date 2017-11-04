hcircify - The HexChat port of "Ircify"
==============================
Created by
===============
Eggy: Porting Ircify to HexChat on Windows
Notrace: Original author of Ircify for mIRC 
      Equalify - Spotify Track Info Library  

Questions/Comments?
Visit us @ irc.equalify.me #equalify          

==============================
NOTE: The HexChat port is only available on Windows, due to current API limitations.

1. Install
===============
	zip contains two DLL files:
		hcircify.dll - 32bit DLL
		hcircify_x64.dll - 64bit DLL
		
	NOTE: Make sure you unload any old versions of DLL before loading the new one!
	( /unload hcircify )
	
	If you get the following error:
	'...\HexChat\addons\hcircify.dll': %1 is not a valid Win32 application. 
	You have installed the wrong version of the DLL; please make sure you have copied the 64bit version of the DLL.
	
	If this DLL does not load... 
	You might need to download & install the Microsoft Visual Studio 2015 Runtimes.
	However, you should have already installed the required versions to run HexChat on Windows.
	

2. Usage
===============
	To use type /ircify in a channel
	or you can use the popupmenu (ircify->ircify) 
	
	Output is customizable either by editing the plugin's conf file (Usually found in '%AppData%\Roaming\HexChat') 
	or via the /OUTPUT SET command 
	
	Output command usage:
	/OUTPUT SAVE => Saves currently set output to the conf.
	/OUTPUT RELOAD => Reloads settings from the conf file.
	/OUTPUT CONFIG => Displays the path to the config file for easy editing.
	/OUTPUT SET <Output> => Changes the Spotify announcement output.
				(HexChat's color codes also work; using ctl+k, or ctrl+b.. etc)
				Example: /OUTPUT SET ^BnP^B: .song. - .artist. (.album.) :: .uri.
	/OUTPUT TYPE <type> => Changes the message type used in the channel.
				(0 = Action, 1 = Message) Example /OUTPUT TYPE 1
		
		
Available tags in the output customizer:

	.song. = Track Title
	.artist. =Track Artist
	.coartists. = Track Coartists (up to 6)
	.album. = Track Album Name
	.year.= Album Release Date
	.uri. = Track URI (spotify:track:...)
	.url. = Track URL (http://open.spotify.com/...)
	.time. = Track Length
	.played. = Track played time
	.pbar. = Track played bar (|||:::::::)
	.popularity. = Track Popularity (**********)
	.shuffle. = Shuffle Status		

3. CHAGELOG
===============
v1.0.4 - Added the ability to change the Spotify port
v1.0.3 - Fix the "Private Message" crash reported
v1.0.2 - Maintenance Release
v1.0.1 - Fixed Private Mode
v1.0 - Public Release
v0.1 - Private Beta Build

4. Thanks to
===============
	Everyone in IRC
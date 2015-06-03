Wagic's source code is currently hosted at [Googlecode](http://code.google.com/p/wagic/source/list).

You might want to compile the game from the sources for various reasons. You want to play the game with the newest features which aren't present in the official release yet, or perhaps you want to add features to the game, or you want to see how to create a game with the JGE library, or you're trying to port Wagic to another platform, or trying to create a Pokemon card game, who knows ...

Compiling Wagic shouldn't be difficult, however you might need to have some basic in PSP programming or at least in bash or DOS shell commands to compile it for the PSP.

## Compiling on Windows ##

This part of the article deals with compilation under a Windows environment.

Thanks to JGE++, Wagic can be compiled for both Windows and the PSP. Compiling a Windows version helps a lot for debugging.

### Compiling for Windows ###

  1. Download and install Microsoft's [Visual C++ Express 2010](http://www.microsoft.com/express/Downloads/) (freeware)
  1. Download and install an SVN client - [Tortoise](http://tortoisesvn.tigris.org) is a popular one, and step 4 of this guide assumes that you're using either Tortoise or another SVN client that integrates with Windows Explorer.
  1. Somewhere on your hard disk, make a new folder for the Wagic source code. I'll call that folder "WagTheSource" in this guide, but you can (and may even want to ;) ) choose a different name.
  1. In Windows Explorer, right-click on the folder "WagTheSource". A context menu pops up. Click on "SVN Checkout...".
  1. A dialogue box pops up. In this dialogue box, under "URL of repository", enter the following address: http://wagic.googlecode.com/svn/trunk/ . Don't change any other settings, and click on "Ok".
  1. Tortoise will now download the latest revision of the Wagic source code to the folder "WagTheSource". _Note: If you use any other method to download the source, make sure that you get the source code for Wagic **and** the source code for JGE. If you've followed these instructions, then you don't have to worry about that._
  1. Start Visual C++ Express. In the top menu bar, click on "File -> Open -> Project/Solution". Navigate to the file "WagTheSource\projects\mtg\mtg\_vs2010.sln" and open it.
  1. Visual C++ Express will now read the structure of the source code files and folders, and display that structure in the "Solution Explorer" to the left of the screen.
  1. In the Solution Explorer, right-click on the topmost entry (which should read "Solution 'mtg' (2 projects)"). A context menu opens, click on "Clean Solution". The Output Window at the bottom on the screen should now show the information "Clean: 2 succeeded, 0 failed, 0 skipped"
  1. Still in the Solution Explorer, right-click on the folder "mtg" and choose "Build". Wait while Visual C++ compiles the source code. You will probably see some warnings (which you can ignore at this stage). When the process has finished, the bottom line in the output window should start with "Build: 2 succeeded, 0 failed". If the build has failed instead, then copy the error from the output window and ask for help in the [forum](http://wololo.net/forum/viewforum.php?f=6).
  1. You have now successfully built the program from the SVN sources. To actually play it, you have several alternatives:
  1. #  "**Clean**" method: Create a Wagic installation solely for playing the game. Copy everything from "WagTheSource\projects\mtg\bin" (including folders and subfolders) to a different location. There, start the file "template.exe" that has been created by the steps above. this "template.exe" is your Wagic executable. You can now play Wagic. Whenever you update the sources from the SVN, and/or compile them again, you have to repeat this step, to update the files in your "playing" installation.
  1. #  "**Easy**" method: Just start the file "template.exe" that has been created in "WagTheSource\projects\mtg\bin". This means that you use the files in your WagTheSource folder for playing. This has a small risk that your files get overwritten if you update the sources - for example if you have added a new AI deck from the forums, gave it the name deck51.txt, and we add a differrent deck51.txt to the SVN. Tortoise will detect such conflicts and either try to merge the files, or inform you that it can't solve the conflict.
  1. #  "**Debug**" method: You can further simplify the process by just clicking on the green "play" symbol (a green triangle pointing to the right) in VC 2008's icon bar. (If it doesn't work the first time, try it again, sometimes the program tries to build JGE and Wagic in the wrong order, but it will then work on the second attempt. Also, you'll have to specify Wagic's working directory - to do so, right-click on "mtg" in the solution explorer, click on "Properties" in the context menu that pops up, then (in the left pane of the dialogue that appears) double-click on "Configuration Properties", then click on "Debugging" slightly below, then (in the right pane) click into the empty space to the right of "Working Directory", and enter "bin" (_without_ the quotes), then close the dialog with "Ok". If you don't do this, Wagic will not find its files and crash.) This will start Wagic in "Debug" mode. This is the same as option (b), with the additional advantage that _if_ something goes wrong, you'll get a more meaningful error message, and the Visual C++ editor will show you the file and the line of code that caused the problem. All three are valuable pieces of information for the programmers, so please add them to your bug report if you post one.
  1. Enjoy the bleeding edge of Wagic development. Whenever you want to update Wagic, just right-click on the "WagTheSource" folder in Windows Explorer, and click on "SVN Update". Tortoise will then automatically update all files that need to. Then repeat steps 10 and 11. If Wagic does not work afterwards, repeat steps 9 to 11. If nothing else helps, one thing to try is to rename the WagTheSource folder, create a new one, and checkout the complete sources again as per step (4).

_Note: The SVN builds are always a work in progress, and therefore the exe may have problems, or may not even start in the first place. If this happens to you, first reduce potential error sources by testing your new template.exe in a fresh unmodded install of Wagic, instead of a using a modded installation. If the problem persists, ask in the [forum](http://wololo.net/forum/) for help._

_Note 2: Compiling the sources for the first time may take quite long. Compiling them again after an update will be much quicker, since the compiler will only update the files that are affected by the changes._

### Compiling for the PSP ###

To run Wagic on the PSP, you need to compile a new EBOOT.PBP. The method described here has successfully been tested on Vista and Windows XP machines (with version 0.11.1 of the minimalist PSP SDK). Please report any issues or success!

  1. Obtain the Wagic source  code as described in steps 2-6 of the "Compiling for Windows" guide. Make sure that you get both the "JGE" and the "projects" directory.
  1. Download and install a PSPSDK environment for windows, such as the [Minimalist PSP SDK](http://sourceforge.net/project/showfiles.php?group_id=223830) by Heimdall. Let the PSPSDK add its path to the system paths (the Minimalist SDK has an option for that which is on by default, just don't turn it off).
  1. Using a DOS command line shell, navigate to the JGE directory, and type "make clean" (and ENTER). This is to remove the old JGE library. (_Note: If you get a message "command not found" at this stage, then the PSPSDK's path information has not been installed correctly; repeat step (2)._)
  1. In the same directory, type "make" (and ENTER). It should create some files. The important one is named libjge300.a in the lib/psp/ folder. If there is no lib/psp/libjge300.a file in your JGE directory, or if this file is old, then something went bad.
  1. Go to the directory projects/mtg with a DOS shell
  1. Type "make clean" (and ENTER)
  1. Type "make" (and ENTER). This should create an EBOOT.PBP file in your projects/mtg folder. That's it!

_Note: If there haven't been any changes to the JGE library itself, then you obviously don't need to recreate it each time, so steps 3 and 4 are not necessary in this case. If you're not sure whether you need to compile JGE, then it's better to do it, to be on the safe side._

_Note 2: The "make clean" step itself is usually not necessary, but recommended if you are not a C++ guru._

_Note 3: If you don't know what a DOS shell is and how to use it, here are some quick instructions:_
  * _To open the DOS shell, click on "Start" in the Windows task bar, then click on "Run", type **cmd** and click on "Ok". This will open a black DOS window with a command line prompt._
  * _To navigate to another drive, type that drive's letter, followed by a colon, and press Enter._
  * _To navigate to a specific directory, type **cd**, press space, then type a **\** sign, and then type the name that directory and press Enter._
  * _So, if you want to navigate to a directory called Games\WagTheSource\JGE, which sits on drive E:, you have to first type **E:** (and press Enter) and then type **cd \Games\WagTheSource\JGE** (and press Enter)._

## Compiling on GNU/Linux ##

This part describes how to compile on GNU/Linux. It may possible to compile on BSD systems by following a similar pattern (update this page if you manage to do it).

This comes from info from Jean on the forum and my personnal experience.

Target audience: users that know a bit how to use command line tools and have notions of what a versionning system is. (I’m not sure I’m correct there)

### Common Steps for all Platforms ###

The steps to do:
  1. Have a compile environment on Linux (standard c libs, gcc and subversion command line tools installed)
  1. Installing Wagic dependancies (freetype, jpeg, gif, png, zip, glut & fmod3.75)
  1. Grab the Wagic sources from SVN

The steps I did on Ubuntu:
  1. Have a compile environment on Linux
  1. **I already had that but I guess:
  1.**   # “apt-get install gcc” should do the trick
  1. **#  “apt-get install subversion” to get SVN command line tool
  1. Installing Wagic dependancies
  1. #  apt-get install libfreetype-dev libjpeg-dev libgif-dev libpng-dev libz-dev libglut-dev
  1. # If you are running a newer version of Ubuntu you might have to do sudo apt-get install libfreetype6-dev libjpeg-dev libgif-dev libpng-dev libz-dev freeglut3-dev instead of what is above
  1. #  download fmod3.75 and put it in /usr/lib
  1. Grab the Wagic sources from SVN
  1. #  “svn checkout http://wagic.googlecode.com/svn/trunk/ wagic-read-only“ as written in googlecode Web site.**

### Compiling for GNU/Linux ###

The steps to do:
  1. Compile JGE
  1. Compile Wagic

The steps I did on Ubuntu:
  1. Compile JGE
  1. #  If I compiled for another platform before, I usually do a “make clean”
  1. #  I went to JGE dir (cd JGE) and launch a “make linux“ (“make” works if no PSP SDK is installed)
  1. Compile Wagic
  1. #  If I compiled for another platform before, I usually do a “make clean”
  1. #  I went to Wagic dir (cd projects/mtg) and launch a “make linux“ (“make” works if no PSP SDK is installed)

Troubleshooting:
  1. Sometimes, the compiler might just not complete the compilation because of WARNING messages getting treated as ERRORS. If you come across one of those warnings, report it on the forums.
  1. To continue the compilation, edit the "Makefile", and then search for "-Werror", removing it from anywhere you find it.
  1. Sometimes, when generating the executable, the console may output a message saying that it skipped an incompatible "libboost\_thread.a". This is generally not an issue. None the less, in some 64 bits systems, the compiled executable will give a "Segmentation Fault" error when trying to run it.

### Compiling for PSP ###

The steps to do:
  1. Installing PSP SDK
  1. Compile JGE
  1. Compile Wagic

The steps in details:
  1. Installing PSP SDK
  1. #  Go to [Minimalist PSPSDK Project Download Page on sourceforge](http://sourceforge.net/projects/minpspw/files/SDK%20%2B%20devpak/pspsdk%200.11.1/) and then grab the correct RPM or DEB that matches your system. Once you have the file, install it manually with your system installer (dpkg for Debian software packages). On some distributions, double clicking the RPM or DEB file gives you the option to install the package.
  1. #  Once installed, you need to add several environment variables.
  1. #   **In case the lines already exist on your file, be sure then they are uncommented and pointing to your PSP SDK installation.
  1. #**  <p>If you are using bash and want to set the variable for only one user you can use a text editor (vi, kate, nano, gedit, or any other alike) to and edit the "~/.profile" or "~/.bash_rc" file. At the end of one of those file, add those lines:<br>
</li></ul><blockquote>export PSPDEV="/opt/pspsdk"
> export PSPSDK="$PSPDEV/psp/sdk"
> export PATH="$PATH:$PSPDEV/bin:$PSPSDK/bin"
:::  **If you are using zsh, edit ~/.zshrc if you want to set the variables for a single user.
:::**  If you use a different shell, search in your shell manual to know which file you need to edit.
<ol start='2'>
<li>Compile JGE<br>
<ol><li>If I compiled for another platform before, I usually do a “make clean”<br>
</li><li>I went to JGE dir (cd JGE) and launch a “make“<br>
</li><li>If you get a ERROR while compiling, telling you that the file "vram.h" is missing, head then to the "JGE/include/" folder, and copy the "vram.h" file from there on "JGE/src/", it will solve the problem.<br>
</li><li>Compile Wagic<br>
</li><li>If I compiled for another platform before, I usually do a “make clean”<br>
</li><li>I went to Wagic dir (cd projects/mtg) and launch a “make“<br>
</li></ol></li></ol>

### Signing the EBOOT file on Linux ###

Software required:
  1. wine. Install it typing "sudo apt-get install wine" on your console.

Steps to do:
  1. Once the PSP EBOOT has finished compiling, you need to copy the "wagic.prx" file generated on your "projects/mtg/" folder to "pspsdk/" (inside your checked out source code of wagic). There you'll see a file named "signing.zip", unpack the zip file there. Once done, move the "wagic.prx" onto the bin folder, and on the bin folder remember to set the "execution" permission on the two exe files there.
  1. On your console, head to the bin folder ("pspsdk/bin", or either press F4 if your File Manager supports the Terminal feature) and then execute the next commands:
> wine fix-relocations.exe wagic.prx
> wine prxEncrypter.exe wagic.prx

  1. The two commands will show little, or no feedback, but they will generate an additional file. Then, copy the "wagic.prx" and "data.psp" back to the "projects/mtg/" folder, overriding any previous files.
  1. On your console then, into the "projects/mtg/" folder, execute the next command:
> pack-pbp EBOOT.PBP PARAM.SFO icon.png NULL pic0.png pic1.png NULL  data.psp NULL
    1. It will pack the EBOOT.PBP file with the signing on it.

### Compiling for Windows ###

That should be doable with g++ but the make need to be changed and I don’t know how to do that.

## General Notes ##

This is probably obvious, but you don't need two different copies of the sources if you want to create both a Windows/Linux and a PSP version.

It is more than recommended to develop using the Windows/Linux version, and test with the PSP version from time to time.

If you have any issue compiling, please report your problems [in the forums](http://wololo.net/forum/viewforum.php?f=16), with _as much precision_ as possible. If you get an error in Visual Studio, search the output window for the first occurrence of the term "error", and post that line in the forum. If you get an error while compiling for the PSP, copy the last lines of the compiler output and paste them into your forum post.

### Playing compiled source ###
See windows info, make it more general (same thing for linux too, only exec name changes)
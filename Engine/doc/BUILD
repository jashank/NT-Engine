BUILD
-------------------
Introduction:
-------------------
This will go through the building process on multiple operating systems to save
anyone wanting to build the project from scratch having issues. Within the
Engine folder we have provided a standard makefile which should build across 
all make types (tested on BSDmake and GNUmake).

Dependencies
-------------------
The following are required for the application to run:
    * Boost.1.43 - Header files for Bind and Function
    * SFML.1.5 - sfml-graphics, sfml-window sfml-audio sfml-system
    * liblua.5.1.4 - Required by Lunar. 

Debian / Ubuntu
-------------------
sudo apt-get install subversion
svn checkout http://noobtoob-kickle.googlecode.com/svn/trunk/ 
             noobtoob-kickle-read-only
sudo apt-get install libboost-dev liblua5.1-0-dev libsfml-audio1.5 
                     libsfml-graphics1.5 libsfml-system1.5 libsfml-window1.5 
cd noobtoob-kickle-read-only/Engine
make

You should have been able to build the target at this point. If not please
contact us on the mailing list and we will resolve the issue.

BSD
-------------------
Dependencies from the port tree: 
lang/lua
devel/boost-libs
devel/sfml 
svn checkout http://noobtoob-kickle.googlecode.com/svn/trunk/ 
             noobtoob-kickle-read-only 
cd noobtoob-kickle-read-only/Engine make (If you get error you may have to
add /usr/local/lib and usr/local/include to the make path)

Mac
-------------------
    * Download liblua5.1 source and build.
    * Download and build boost.1.43 we will be including the header 
      files in the project soon. so this step will not be needed.
    * Download SFML-1.5 dmg and install
    * Convert all instances of sfml- to -framework sfml- 

Windows
-------------------
TODO

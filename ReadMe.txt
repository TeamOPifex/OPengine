========================================
== OPifex Entertainment
========================================

This is a C/C++ based, data-oriented, OpenGL engine.

Garrett Hoofman - garrett@teamopifex.com
Kirk Roerig - kirk@teamopifex.com


========================================
== Requirements
========================================

* CMake v2.8

--- Linux (Ubuntu 12.04 LTS) ---

* OpenGL development headers + lib (GL 2.0 capable drivers)
* OpenAL development headers + lib
* GCC & G++

--- Windows ---

* OpenAL Redistributable (oalinst)
* OpenAL SDK (OpenAL11CoreSDK)
* Visual Studio 2010/2012

--- Android ---

* Android SDK
* Android NDK
* Ant


========================================
== Installation Instructions
========================================

--- Linux (Ubuntu) ---

	* Install Mercurial
		* sudo apt-get install mercurial
	* Install Cmake
		* sudo apt-get install cmake
	* Install OpenGL libraries
		* sudo apt-get install mesa-common-dev xorg-dev libglu1-mesa-dev
		* sudo apt-get install freeglut3-dev
	* Install OpenAL libraries
		* sudo apt-get install libopenal1 libopenal-dev
	* Install libogg
		*  cd /tmp && wget -O libogg-1.3.0.tar.gz http://goo.gl/vHzMn
		* tar zxvf libogg-1.3.0.tar.gz && cd libogg-1.3.0
		* ./configure && make
		* sudo make install
	* Install libvorbis
		* cd /tmp && wget -O libvorbis-1.3.3.tar.gz http://goo.gl/nSDmc
		* tar zxvf libvorbis-1.3.3.tar.gz && cd libvorbis-1.3.3
		* ./configure && make
		* sudo make install
	* Install compiler tools
		* sudo apt-get install build-essential -y
		
		
	* Commiting Code
		* Mecurial will need to be configured
			* Save a file to your root directory (~) with the filename ".hgrc" and the contents:
				[ui]
				# Name data to appear in commits
				username = Garrett Hoofman <gambitsunob@gmail.com[ui]

		
		
========================================
== Release Mode Instructions
========================================

* CMake - Check OPIFEX_RELEASE
* Build in Visual Studio under Release Mode
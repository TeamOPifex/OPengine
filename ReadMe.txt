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

---------------------------------
--- Linux (Ubuntu)  ---
---------------------------------

* OpenGL development headers + lib (GL 2.0 capable drivers)
* OpenAL development headers + lib
* GCC & G++

---------------------------------
--- Windows 				  ---
---------------------------------

* OpenAL Redistributable (oalinst)
* OpenAL SDK (OpenAL11CoreSDK)
* Visual Studio 2010/2012

---------------------------------
--- Android	API 16			  ---
---------------------------------

* Android SDK
* Android NDK
* Ant


========================================
== Installation Instructions
========================================

---------------------------------
--- Windows		 			  ---
---------------------------------

	* Install OpenAL
		* http://www.openal.org/creative-installers/
	* Install Cmake
		* http://www.cmake.org/download/
	

---------------------------------
--- Linux (Ubuntu) 			  ---
---------------------------------

	* Install Git
		* sudo apt-get install git
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

				
		---------------------------------
		--- Android on Linux (Ubuntu) ---
		---------------------------------
		
			* Install Ant
				* sudo apt-get install ant
			* Download Android SDK
				* https://developer.android.com/sdk/installing/index.html?pkg=tools
			* Download Android NDK
				- Make sure you download the correct Platform Target (32 bit for Ouya)
				* https://developer.android.com/tools/sdk/ndk/index.html#Installing
			* Extract Android SDK and Android NDK
			* Add directories to your PATH
				* export PATH=$PATH:[/path/to/android-ndk]
				* export PATH=$PATH:[/path/to/android-sdk]/sdk/tools/
				* export PATH=$PATH:[/path/to/android-sdk]/sdk/platform-tools/
			* Install JDK
				* sudo apt-get install openjdk-7-jre 
				* sudo apt-get install openjdk-7-jdk
			* Install 64bit dependencies (not needed for 32 bit)
				* sudo dpkg --add-architecture i386
				* sudo apt-get update
				* sudo apt-get install libncurses5:i386 libstdc++6:i386 zlib1g:i386
			* Install adb tools
				* sudo apt-get install android-tools-adb
			* Run Cmake for Android
				* cmake -D OPIFEX_OS:string=OPIFEX_ANDROID
			* android update project --target 1 --subprojects --path .
			* ndk-build
			* ant debug
			* adb install -r bin/OPengine-debug.apk
		
========================================
== Release Mode Instructions
========================================

* CMake - Check OPIFEX_RELEASE
* Build in Visual Studio under Release Mode
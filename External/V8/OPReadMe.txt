Download the V8 Repository

Use this command (from the V8 Repository) to build the project:
third_party\python_26\python.exe build\gyp_v8 -G msvs_version=2010 -Dtarget_arch=x64 -D"component=shared_library"

'target_arch' options:
	x64
	ia32

It will create a build directory, copy and paste the files into the External lib into the appropriate build folder.

	* icui18n.dll
	* ucuuc.dll
	* v8.dll
	* v8.lib




SET DEPOT_TOOLS_WIN_TOOLCHAIN=0
SET GYP_DEFINES=target_arch=x64
SET GYP_MSVS_VERSION=2015
gclient sync
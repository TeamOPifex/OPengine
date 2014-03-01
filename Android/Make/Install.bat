call android update project --path . --subprojects
call ndk-build
call copy obj\local\armeabi\libopifex-core.a Binaries\android\
call copy obj\local\armeabi\libopifex-data.a Binaries\android\
call copy obj\local\armeabi\libopifex-math.a Binaries\android\
call copy obj\local\armeabi\libopifex-performance.a Binaries\android\
call copy obj\local\armeabi\libopifex-human.a Binaries\android\
call copy obj\local\armeabi\libopifex-scripting.a Binaries\android\
call copy obj\local\armeabi\libopifex-pipeline.a Binaries\android\
call copy obj\local\armeabi\libogg.a Binaries\android\
call copy obj\local\armeabi\libvorbis.a Binaries\android\
call copy obj\local\armeabi\liblodepng.a Binaries\android\
call ant debug
call adb install -r bin\OPEngine-debug.apk
call adb logcat -c
call adb logcat > log.txt
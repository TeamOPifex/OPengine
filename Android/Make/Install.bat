call android update project --path . --subprojects
call ndk-build
call ant debug
call adb install -r bin\OPEngine-debug.apk
call adb logcat -c
call adb logcat > log.txt
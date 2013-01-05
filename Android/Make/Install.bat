call android update project --target 3 --path . --subprojects
call ndk-build
call ant debug
call adb install -r bin\SMRF-debug.apk
call adb logcat > log.txt
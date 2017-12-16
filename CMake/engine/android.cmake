option(OPIFEX_ANDROID_SETUP "Setup Android Project" OFF)

if(${OPIFEX_ANDROID_SETUP})

STRING(REGEX REPLACE "\\\\" "/" OPIFEX_ENGINE_REPOSITORY ${OPIFEX_ENGINE_REPOSITORY})
if(${OPIFEX_BINARIES} STREQUAL "")
else()
	STRING(REGEX REPLACE "\\\\" "/" OPIFEX_BINARIES ${OPIFEX_BINARIES}) 
endif()

# Copy Android Studio sln to build directory

SET(PLATFORM_VERSION_STRING "\${platformVersion}")

copy_source_group( "${PROJECT_SOURCE_DIR}/Android/app/src" ${PROJECT_BINARY_DIR}/Android/app/src )
# configure_file( "${PROJECT_SOURCE_DIR}/Android/app/app.iml.in" "${PROJECT_BINARY_DIR}/Android/app/app.iml" )
configure_file( "${PROJECT_SOURCE_DIR}/Android/app/proguard-rules.pro" "${PROJECT_BINARY_DIR}/Android/app/proguard-rules.pro" )
# copy_source_group( "${PROJECT_SOURCE_DIR}/Android/gradle" ${PROJECT_BINARY_DIR}/Android/gradle )
configure_file( "${PROJECT_SOURCE_DIR}/Android/Android.iml" "${PROJECT_BINARY_DIR}/Android/Android.iml" COPYONLY )
configure_file( "${PROJECT_SOURCE_DIR}/Android/build.gradle" "${PROJECT_BINARY_DIR}/Android/build.gradle" COPYONLY )
configure_file( "${PROJECT_SOURCE_DIR}/Android/gradle.properties" "${PROJECT_BINARY_DIR}/Android/gradle.properties" COPYONLY )
configure_file( "${PROJECT_SOURCE_DIR}/Android/gradlew" "${PROJECT_BINARY_DIR}/Android/gradlew" COPYONLY )
configure_file( "${PROJECT_SOURCE_DIR}/Android/gradlew.bat" "${PROJECT_BINARY_DIR}/Android/gradlew.bat" COPYONLY )
configure_file( "${PROJECT_SOURCE_DIR}/Android/settings.gradle" "${PROJECT_BINARY_DIR}/Android/settings.gradle" COPYONLY )

configure_file( ${PROJECT_SOURCE_DIR}/Android/app/build.gradle.in ${PROJECT_BINARY_DIR}/Android/app/build.gradle )


copy_source_group( "${PROJECT_SOURCE_DIR}/Assets/" ${PROJECT_BINARY_DIR}/Android/app/src/main/assets/ )

endif()
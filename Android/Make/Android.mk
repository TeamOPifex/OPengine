# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

# the purpose of this sample is to demonstrate how one can
# generate two distinct shared libraries and have them both
# uploaded in
#

LOCAL_PATH:= $(call my-dir)

PROJECT_PATH:= $(LOCAL_PATH)/../@OPIFEX_REPOSITORY@

NDK_APP_OUT := $(LOCAL_PATH)/../Binaries/android
BINARIES := $(NDK_APP_OUT)


##############
# LIBOGG
##############
include $(CLEAR_VARS)
LOCAL_MODULE    := libogg
LOCAL_SRC_FILES := ../$(NDK_APP_OUT)/libogg.a
include $(PREBUILT_STATIC_LIBRARY)


##############
# LIBVORBIS
##############
include $(CLEAR_VARS)
LOCAL_MODULE    := libvorbis
LOCAL_SRC_FILES := ../$(NDK_APP_OUT)/libvorbis.a
LOCAL_STATIC_LIBRARIES := libogg

include $(PREBUILT_STATIC_LIBRARY)




##############
# LODEPNG
##############
include $(CLEAR_VARS)

LOCAL_MODULE := lodepng

LOCAL_C_INCLUDES :=$(PROJECT_PATH)

MY_LOCAL_SRC_FILES := $(wildcard $(PROJECT_PATH)/External/LodePNG/src/*.cpp)

LOCAL_SRC_FILES := $(subst jni/, , $(MY_LOCAL_SRC_FILES))

LOCAL_LDLIBS := -lstdc++

include $(BUILD_STATIC_LIBRARY)


##############
# 1_CORE
# core lib, which will be built statically
##############
include $(CLEAR_VARS)
LOCAL_MODULE    := libopifex-core
LOCAL_CFLAGS := -D@OPIFEX_OS@

LOCAL_C_INCLUDES :=$(PROJECT_PATH)
#CORE_LIST_C := $(wildcard $(PROJECT_PATH)/Core/src/*.c)

MY_LOCAL_SRC_FILES := $(wildcard $(PROJECT_PATH)/Core/src/*.c)
LOCAL_SRC_FILES := $(subst jni/, , $(MY_LOCAL_SRC_FILES))

#LOCAL_SRC_FILES := \
#       $(CORE_LIST_C:$(PROJECT_PATH)/%=%)
        
LOCAL_STATIC_LIBRARIES := android_native_app_glue
include $(BUILD_STATIC_LIBRARY)



##############
# 2_DATA
# data lib, which will be built statically
##############
include $(CLEAR_VARS)
LOCAL_MODULE    := libopifex-data

LOCAL_C_INCLUDES :=$(PROJECT_PATH)
LOCAL_CFLAGS := -D@OPIFEX_OS@

MY_LOCAL_SRC_FILES := $(wildcard $(PROJECT_PATH)/Data/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Data/src/*.c)
LOCAL_SRC_FILES := $(subst jni/, , $(MY_LOCAL_SRC_FILES))

LOCAL_STATIC_LIBRARIES := libopifex-core
include $(BUILD_STATIC_LIBRARY)


##############
# 3_MATH
# math lib, which will be built statically
##############
include $(CLEAR_VARS)
LOCAL_MODULE    := libopifex-math

LOCAL_C_INCLUDES :=$(PROJECT_PATH)
LOCAL_CFLAGS := -D@OPIFEX_OS@

MY_LOCAL_SRC_FILES := $(wildcard $(PROJECT_PATH)/Math/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Math/src/*.c)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Math/src/*.cpp)
LOCAL_SRC_FILES := $(subst jni/, , $(MY_LOCAL_SRC_FILES))

LOCAL_STATIC_LIBRARIES := libopifex-data
include $(BUILD_STATIC_LIBRARY)


##############
# 4_PERFORMANCE
# performance lib, which will be built statically
##############
include $(CLEAR_VARS)
LOCAL_MODULE    := libopifex-performance
LOCAL_CFLAGS := -D@OPIFEX_OS@ -DOPIFEX_PHYSICS

LOCAL_C_INCLUDES :=$(PROJECT_PATH)
LOCAL_C_INCLUDES +=$(PROJECT_PATH)/External/PhysX/IncludeAndroid

MY_LOCAL_SRC_FILES := $(wildcard $(PROJECT_PATH)/Performance/src/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Performance/src/*.c)
LOCAL_SRC_FILES := $(subst jni/, , $(MY_LOCAL_SRC_FILES))

LOCAL_STATIC_LIBRARIES := libopifex-math

include $(BUILD_STATIC_LIBRARY)



##############
# 5_HUMAN
# human lib, which will be built statically
##############
include $(CLEAR_VARS)
LOCAL_MODULE    := libopifex-human
LOCAL_LDLIBS    := -llog -lGLESv2 -lOpenSLES
LOCAL_CFLAGS    := -Werror
LOCAL_CFLAGS 	+= -D@OPIFEX_OS@ -DOPIFEX_OPENGL_ES -DOPIFEX_OPENGL_ES_2

LOCAL_C_INCLUDES :=$(PROJECT_PATH)
LOCAL_C_INCLUDES +=$(PROJECT_PATH)/External/Ogg/include
LOCAL_C_INCLUDES +=$(PROJECT_PATH)/External/Vorbis/include

MY_LOCAL_SRC_FILES := $(wildcard $(PROJECT_PATH)/Human/src/Audio/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Human/src/Audio/src/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Human/src/Input/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Human/src/Math/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Human/src/Rendering/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Human/src/Rendering/GL/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Human/src/Rendering/Font/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Human/src/Rendering/Primitives/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Human/src/Rendering/Skinning/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Human/src/Rendering/Sprite/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Human/src/Resources/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Human/src/Resources/Buffer/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Human/src/Resources/Material/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Human/src/Resources/Material/Shader/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Human/src/Resources/Material/Shader/Params/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Human/src/Resources/Material/State/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Human/src/Resources/Model/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Human/src/Resources/Sound/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Human/src/Resources/Texture/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Human/src/Systems/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Human/src/Utilities/*.cpp)

LOCAL_SRC_FILES := $(subst jni/, , $(MY_LOCAL_SRC_FILES))

LOCAL_STATIC_LIBRARIES := libopifex-performance libvorbis -lstdc++ lodepng
LOCAL_STATIC_LIBRARIES += android_native_app_glue

include $(BUILD_STATIC_LIBRARY)

##############
# 6_Communication
# scripting lib, which will be built statically
##############
include $(CLEAR_VARS)
LOCAL_MODULE    := libopifex-communication
LOCAL_LDLIBS    := -llog -lGLESv2 -lOpenSLES
LOCAL_CFLAGS    := -Werror
LOCAL_CFLAGS 	+= -D@OPIFEX_OS@ -DOPIFEX_OPENGL_ES -DOPIFEX_OPENGL_ES_2

LOCAL_C_INCLUDES :=$(PROJECT_PATH)

MY_LOCAL_SRC_FILES := $(wildcard $(PROJECT_PATH)/Communication/src/*.cpp)
LOCAL_SRC_FILES := $(subst jni/, , $(MY_LOCAL_SRC_FILES))

LOCAL_STATIC_LIBRARIES := libopifex-human

include $(BUILD_STATIC_LIBRARY)

##############
# 7_SCRIPTING
# scripting lib, which will be built statically
##############
include $(CLEAR_VARS)
LOCAL_MODULE    := libopifex-scripting
LOCAL_LDLIBS    := -llog -lGLESv2 -lOpenSLES
LOCAL_CFLAGS    := -Werror
LOCAL_CFLAGS 	+= -D@OPIFEX_OS@ -DOPIFEX_OPENGL_ES -DOPIFEX_OPENGL_ES_2

LOCAL_C_INCLUDES :=$(PROJECT_PATH)
LOCAL_C_INCLUDES +=$(PROJECT_PATH)/Communication
LOCAL_C_INCLUDES +=$(PROJECT_PATH)/External/Ogg/include
LOCAL_C_INCLUDES +=$(PROJECT_PATH)/External/Vorbis/include
LOCAL_C_INCLUDES +=$(PROJECT_PATH)/External/V8/
LOCAL_C_INCLUDES +=$(PROJECT_PATH)/External/V8/include

MY_LOCAL_SRC_FILES := $(wildcard $(PROJECT_PATH)/Scripting/src/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Scripting/src/wrappers/*.cpp)
LOCAL_SRC_FILES := $(subst jni/, , $(MY_LOCAL_SRC_FILES))

LOCAL_STATIC_LIBRARIES := libopifex-pipeline

include $(BUILD_STATIC_LIBRARY)


##############
# 8_PIPELINE
# pipeline lib, which will be built statically
##############
include $(CLEAR_VARS)
LOCAL_MODULE    := libopifex-pipeline
LOCAL_CFLAGS 	:= -D@OPIFEX_OS@ -DOPIFEX_OPENGL_ES -DOPIFEX_OPENGL_ES_2

LOCAL_C_INCLUDES :=$(PROJECT_PATH)
LOCAL_C_INCLUDES +=$(PROJECT_PATH)/External/Ogg/include
LOCAL_C_INCLUDES +=$(PROJECT_PATH)/External/Vorbis/include

MY_LOCAL_SRC_FILES := $(wildcard $(PROJECT_PATH)/Pipeline/src/*.cpp)
LOCAL_SRC_FILES := $(subst jni/, , $(MY_LOCAL_SRC_FILES))

LOCAL_STATIC_LIBRARIES := libopifex-communication

include $(BUILD_STATIC_LIBRARY)


##############
# 9_APPLICATION
# second lib, which will depend on and include the first one
##############
include $(CLEAR_VARS)
LOCAL_MODULE    := libopengine
LOCAL_LDLIBS    := -llog -lGLESv2 -landroid -lOpenSLES -lEGL

TARGET_PLATFORM := 16

LOCAL_LDFLAGS := -Wl,--start-group \
	$(BINARIES)/libPhysX3Common.a \
	$(BINARIES)/libPvdRuntime.a \
	$(BINARIES)/libSimulationController.a \
	$(BINARIES)/libSceneQuery.a \
	$(BINARIES)/libLowLevel.a \
	$(BINARIES)/libLowLevelCloth.a \
	$(BINARIES)/libPhysX3.a \
	$(BINARIES)/libPhysX3Vehicle.a \
	$(BINARIES)/libPhysX3Cooking.a \
	$(BINARIES)/libPhysX3Extensions.a \
	$(BINARIES)/libPhysX3CharacterKinematic.a \
	$(BINARIES)/libPhysXProfileSDK.a \
	$(BINARIES)/libPxTask.a \
	$(BINARIES)/libPhysXVisualDebuggerSDK.a \
	-Wl,--end-group
	
LOCAL_ALLOW_UNDEFINED_SYMBOLS := false

LOCAL_C_INCLUDES :=$(PROJECT_PATH)
LOCAL_C_INCLUDES +=$(PROJECT_PATH)/Application
LOCAL_C_INCLUDES +=$(PROJECT_PATH)/Application/Examples
LOCAL_C_INCLUDES +=$(PROJECT_PATH)/External/Ogg/include
LOCAL_C_INCLUDES +=$(PROJECT_PATH)/External/Vorbis/include
LOCAL_C_INCLUDES +=$(PROJECT_PATH)/External/V8/
LOCAL_C_INCLUDES +=$(PROJECT_PATH)/External/V8/include
LOCAL_C_INCLUDES +=$(PROJECT_PATH)/External/PhysX/IncludeAndroid

LOCAL_CFLAGS += -D@OPIFEX_OS@ -DOPIFEX_OPENGL_ES -DOPIFEX_OPENGL_ES_2

MY_LOCAL_SRC_FILES := $(wildcard $(PROJECT_PATH)/Application/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Application/Examples/*.cpp)

LOCAL_SRC_FILES := $(subst jni/, , $(MY_LOCAL_SRC_FILES))
LOCAL_STATIC_LIBRARIES := android_native_app_glue libopifex-scripting
include $(BUILD_SHARED_LIBRARY)
$(call import-module,android/native_app_glue)

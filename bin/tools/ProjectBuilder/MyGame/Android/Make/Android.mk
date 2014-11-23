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
ENGINE_PATH:= $(LOCAL_PATH)/../@OPIFEX_ENGINE_REPOSITORY@
BINARIES_PATH:= $(LOCAL_PATH)/../@OPIFEX_BINARIES@

##############
# LIBOGG
##############
include $(CLEAR_VARS)
LOCAL_MODULE    := libogg
LOCAL_SRC_FILES := ../$(BINARIES_PATH)/android/libogg.a
include $(PREBUILT_STATIC_LIBRARY)

##############
# LIBVORBIS
##############
include $(CLEAR_VARS)
LOCAL_MODULE    := libvorbis
LOCAL_SRC_FILES := ../$(BINARIES_PATH)/android/libvorbis.a
LOCAL_STATIC_LIBRARIES := libogg

include $(PREBUILT_STATIC_LIBRARY)


##############
# LODEPNG
##############
include $(CLEAR_VARS)

LOCAL_MODULE := lodepng

LOCAL_C_INCLUDES :=$(ENGINE_PATH)

MY_LOCAL_SRC_FILES := $(wildcard $(ENGINE_PATH)/External/LodePNG/src/*.cpp)

LOCAL_SRC_FILES := $(subst jni/, , $(MY_LOCAL_SRC_FILES))

LOCAL_LDLIBS := -lstdc++

include $(BUILD_STATIC_LIBRARY)


##############
# 1_CORE
# core lib, which will be built statically
##############
include $(CLEAR_VARS)
LOCAL_MODULE    := libopifex-core 
LOCAL_SRC_FILES := ../$(BINARIES_PATH)/android/libopifex-core.a

include $(PREBUILT_STATIC_LIBRARY)



##############
# 2_DATA
# data lib, which will be built statically
##############
include $(CLEAR_VARS)
LOCAL_MODULE    := libopifex-data 
LOCAL_SRC_FILES := ../$(BINARIES_PATH)/android/libopifex-data.a
LOCAL_STATIC_LIBRARIES := libopifex-core

include $(PREBUILT_STATIC_LIBRARY)


##############
# 3_MATH
# math lib, which will be built statically
##############
include $(CLEAR_VARS)
LOCAL_MODULE    := libopifex-math 
LOCAL_SRC_FILES := ../$(BINARIES_PATH)/android/libopifex-math.a
LOCAL_STATIC_LIBRARIES := libopifex-data

include $(PREBUILT_STATIC_LIBRARY)

##############
# 4_PERFORMANCE
# performance lib, which will be built statically
##############
include $(CLEAR_VARS)
LOCAL_MODULE    := libopifex-performance 
LOCAL_SRC_FILES := ../$(BINARIES_PATH)/android/libopifex-performance.a
LOCAL_STATIC_LIBRARIES := libopifex-math

include $(PREBUILT_STATIC_LIBRARY)


##############
# 5_HUMAN
# human lib, which will be built statically
##############
include $(CLEAR_VARS)
LOCAL_MODULE    := libopifex-human 
LOCAL_SRC_FILES := ../$(BINARIES_PATH)/android/libopifex-human.a
LOCAL_STATIC_LIBRARIES := libopifex-performance libvorbis lodepng

include $(PREBUILT_STATIC_LIBRARY)



##############
# 6_Communication
# communication lib, which will be built statically
##############
include $(CLEAR_VARS)
LOCAL_MODULE    := libopifex-communication 
LOCAL_SRC_FILES := ../$(BINARIES_PATH)/android/libopifex-communication.a
LOCAL_STATIC_LIBRARIES := libopifex-human

include $(PREBUILT_STATIC_LIBRARY)


##############
# 7_SCRIPTING
# scripting lib, which will be built statically
##############
include $(CLEAR_VARS)
LOCAL_MODULE    := libopifex-scripting 
LOCAL_SRC_FILES := ../$(BINARIES_PATH)/android/libopifex-scripting.a
LOCAL_STATIC_LIBRARIES := libopifex-communication

include $(PREBUILT_STATIC_LIBRARY)


##############
# 8_PIPELINE
# pipeline lib, which will be built statically
##############
include $(CLEAR_VARS)
LOCAL_MODULE    := libopifex-pipeline 
LOCAL_SRC_FILES := ../$(BINARIES_PATH)/android/libopifex-pipeline.a
LOCAL_STATIC_LIBRARIES := libopifex-scripting

include $(PREBUILT_STATIC_LIBRARY)


##############
# 9_APPLICATION
# second lib, which will depend on and include the first one
##############
include $(CLEAR_VARS)
LOCAL_MODULE    := libopengine
LOCAL_LDLIBS    := -llog -lGLESv2 -landroid -lOpenSLES

LOCAL_ALLOW_UNDEFINED_SYMBOLS := false

LOCAL_C_INCLUDES :=$(PROJECT_PATH)
LOCAL_C_INCLUDES +=$(ENGINE_PATH)
LOCAL_C_INCLUDES +=$(ENGINE_PATH)/External/Ogg/include
LOCAL_C_INCLUDES +=$(ENGINE_PATH)/External/Vorbis/include
LOCAL_CFLAGS += -D@OPIFEX_OS@ -DOPIFEX_OPENGL_ES -DOPIFEX_OPENGL_ES_2 -frtti

MY_LOCAL_SRC_FILES := $(wildcard $(PROJECT_PATH)/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/src/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/src/Effects/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/src/Entities/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/src/Handlers/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/src/States/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/src/Utilities/*.cpp)

LOCAL_SRC_FILES := $(subst jni/, , $(MY_LOCAL_SRC_FILES))

LOCAL_STATIC_LIBRARIES := libopifex-pipeline

include $(BUILD_SHARED_LIBRARY)

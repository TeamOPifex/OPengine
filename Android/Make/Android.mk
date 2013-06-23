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

##############
# LIBOGG
##############
include $(CLEAR_VARS)

LOCAL_MODULE := libogg

LOCAL_C_INCLUDES :=$(PROJECT_PATH)

MY_LOCAL_SRC_FILES := $(wildcard $(PROJECT_PATH)/Human/Utilities/Ogg/*.c)
LOCAL_SRC_FILES := $(subst jni/, , $(MY_LOCAL_SRC_FILES))

include $(BUILD_STATIC_LIBRARY)

##############
# LIBTREMOR
##############
include $(CLEAR_VARS)
LOCAL_MODULE := libtremor

LOCAL_STATIC_LIBRARIES := libogg

#LOCAL_C_INCLUDES :=$(PROJECT_PATH)/Human/Utilities/include-all
LOCAL_CFLAGS := -I $(PROJECT_PATH)/Human/Utilities/

 
MY_LOCAL_SRC_FILES := $(wildcard $(PROJECT_PATH)/Human/Utilities/Tremor/*.c)
#LOCAL_SRC_FILES := $(subst jni/, , $(MY_LOCAL_SRC_FILES))

include $(BUILD_STATIC_LIBRARY)

##############
# LIBOGG
##############
include $(CLEAR_VARS)

LOCAL_MODULE := libfuckit
#LOCAL_CFLAGS += -I$(PROJECT_PATH)/External/Ogg/include -ffast-math -fsigned-char
#LOCAL_CFLAGS += -march=armv6 -marm -mfloat-abi=softfp -mfpu=vfp
#LOCAL_SRC_FILES := $(wildcard $(PROJECT_PATH)/External/Ogg/src/*.c)
   LOCAL_SRC_FILES := Ogg/lib/android/libogg.so
   include $(PREBUILT_SHARED_LIBRARY)

#include $(BUILD_STATIC_LIBRARY)

##############
# LIBVORBIS
##############
include $(CLEAR_VARS)

LOCAL_MODULE := libvorbis
#LOCAL_CFLAGS += -I$(PROJECT_PATH)/External/Vorbis/include -ffast-math -fsigned-char
#LOCAL_CFLAGS += -march=armv6 -marm -mfloat-abi=softfp -mfpu=vfp
LOCAL_SHARED_LIBRARIES := libogg
#LOCAL_SRC_FILES := $(wildcard $(PROJECT_PATH)/External/Vorbis/*.c)
   LOCAL_SRC_FILES := Vorbis/lib/android/libvorbis.so
   include $(PREBUILT_SHARED_LIBRARY)

#include $(BUILD_STATIC_LIBRARY)

##############
# 1_CORE
# core lib, which will be built statically
##############
include $(CLEAR_VARS)
LOCAL_MODULE    := libopifex-core

LOCAL_C_INCLUDES :=$(PROJECT_PATH)
#CORE_LIST_C := $(wildcard $(PROJECT_PATH)/Core/src/*.c)

MY_LOCAL_SRC_FILES := $(wildcard $(PROJECT_PATH)/Core/src/*.c)
LOCAL_SRC_FILES := $(subst jni/, , $(MY_LOCAL_SRC_FILES))

#LOCAL_SRC_FILES := \
#       $(CORE_LIST_C:$(PROJECT_PATH)/%=%)
        
include $(BUILD_STATIC_LIBRARY)



##############
# 2_DATA
# data lib, which will be built statically
##############
include $(CLEAR_VARS)
LOCAL_MODULE    := libopifex-data

LOCAL_C_INCLUDES :=$(PROJECT_PATH)

MY_LOCAL_SRC_FILES := $(wildcard $(PROJECT_PATH)/Data/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Data/src/*.c)
LOCAL_SRC_FILES := $(subst jni/, , $(MY_LOCAL_SRC_FILES))

LOCAL_STATIC_LIBRARIES := libopifex-core
include $(BUILD_STATIC_LIBRARY)



##############
# 4_HUMAN
# human lib, which will be built statically
##############
include $(CLEAR_VARS)
LOCAL_MODULE    := libopifex-human
LOCAL_LDLIBS    := -llog -lGLESv2 -lOpenSLES
LOCAL_CFLAGS    := -Werror

LOCAL_C_INCLUDES :=$(PROJECT_PATH)
LOCAL_C_INCLUDES +=$(PROJECT_PATH)/External/Ogg/include
LOCAL_C_INCLUDES +=$(PROJECT_PATH)/External/Vorbis/include

MY_LOCAL_SRC_FILES := $(wildcard $(PROJECT_PATH)/Human/Audio/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Human/Audio/src/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Human/Input/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Human/Math/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Human/Rendering/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Human/Rendering/GL/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Human/Resources/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Human/Resources/Buffer/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Human/Resources/Material/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Human/Resources/Material/Shader/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Human/Resources/Material/Shader/Params/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Human/Resources/Material/State/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Human/Resources/Model/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Human/Resources/Sound/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Human/Resources/Texture/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Human/Utilities/*.cpp)
LOCAL_SRC_FILES := $(subst jni/, , $(MY_LOCAL_SRC_FILES))

LOCAL_STATIC_LIBRARIES := libopifex-data libtremor
include $(BUILD_STATIC_LIBRARY)



##############
# 7_APPLICATION
# second lib, which will depend on and include the first one
##############
include $(CLEAR_VARS)
LOCAL_MODULE    := libsmrf
LOCAL_LDLIBS    := -llog -lGLESv2 -landroid -lOpenSLES

LOCAL_C_INCLUDES :=$(PROJECT_PATH)
LOCAL_C_INCLUDES +=$(PROJECT_PATH)/External/Ogg/include
LOCAL_C_INCLUDES +=$(PROJECT_PATH)/External/Vorbis/include

MY_LOCAL_SRC_FILES := $(wildcard $(PROJECT_PATH)/Application/*.cpp)
LOCAL_SRC_FILES := $(subst jni/, , $(MY_LOCAL_SRC_FILES))
LOCAL_STATIC_LIBRARIES := libopifex-human
include $(BUILD_SHARED_LIBRARY)
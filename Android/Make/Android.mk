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

LOCAL_CFLAGS := -I $(PROJECT_PATH)/Human/include/Utilities/
MY_LOCAL_SRC_FILES := $(wildcard $(PROJECT_PATH)/Human/src/Utilities/*.c)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Human/src/Utilities/ogg/*.c)
LOCAL_SRC_FILES := $(subst jni/, , $(MY_LOCAL_SRC_FILES))

include $(BUILD_STATIC_LIBRARY)

##############
# LIBVORBIS
##############
include $(CLEAR_VARS)
LOCAL_MODULE := libvorbis

LOCAL_STATIC_LIBRARIES := libogg

LOCAL_CFLAGS := -I $(PROJECT_PATH)/Human/include/Utilities/
LOCAL_CFLAGS += -I $(PROJECT_PATH)/Human/include/Utilities/vorbis/
MY_LOCAL_SRC_FILES := $(wildcard $(PROJECT_PATH)/Human/include/Utilities/vorbis/*.c)
LOCAL_SRC_FILES := $(subst jni/, , $(MY_LOCAL_SRC_FILES))

include $(BUILD_STATIC_LIBRARY)

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
# 3_MATH
# data lib, which will be built statically
##############
include $(CLEAR_VARS)
LOCAL_MODULE    := libopifex-math

LOCAL_C_INCLUDES :=$(PROJECT_PATH)

MY_LOCAL_SRC_FILES := $(wildcard $(PROJECT_PATH)/Math/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Math/src/*.c)
LOCAL_SRC_FILES := $(subst jni/, , $(MY_LOCAL_SRC_FILES))

LOCAL_STATIC_LIBRARIES := libopifex-data
include $(BUILD_STATIC_LIBRARY)



##############
# 5_HUMAN
# human lib, which will be built statically
##############
include $(CLEAR_VARS)
LOCAL_MODULE    := libopifex-human
LOCAL_LDLIBS    := -llog -lGLESv2 -lOpenSLES
LOCAL_CFLAGS    := -Werror

LOCAL_C_INCLUDES :=$(PROJECT_PATH)
LOCAL_C_INCLUDES +=$(PROJECT_PATH)/Human/include/Utilities/
LOCAL_C_INCLUDES +=$(PROJECT_PATH)/Human/include/Utilities/vorbis/
LOCAL_C_INCLUDES +=$(PROJECT_PATH)/Human/include/Utilities/vorbis/modes/
LOCAL_C_INCLUDES +=$(PROJECT_PATH)/Human/include/Utilities/vorbis/books/

MY_LOCAL_SRC_FILES := $(wildcard $(PROJECT_PATH)/Human/src/Audio/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Human/src/Audio/src/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Human/src/Input/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Human/src/Math/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Human/src/Rendering/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Human/src/Rendering/GL/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Human/src/Resources/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Human/src/Resources/Buffer/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Human/src/Resources/Material/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Human/src/Resources/Material/Shader/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Human/src/Resources/Material/Shader/Params/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Human/src/Resources/Material/State/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Human/src/Resources/Model/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Human/src/Resources/Sound/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Human/src/Resources/Texture/*.cpp)
MY_LOCAL_SRC_FILES += $(wildcard $(PROJECT_PATH)/Human/src/Utilities/*.cpp)

LOCAL_SRC_FILES := $(subst jni/, , $(MY_LOCAL_SRC_FILES))

LOCAL_STATIC_LIBRARIES := libopifex-math libvorbis
include $(BUILD_STATIC_LIBRARY)



##############
# 8_APPLICATION
# second lib, which will depend on and include the first one
##############
include $(CLEAR_VARS)
LOCAL_MODULE    := libsmrf
LOCAL_LDLIBS    := -llog -lGLESv2 -landroid -lOpenSLES

LOCAL_ALLOW_UNDEFINED_SYMBOLS := false

LOCAL_C_INCLUDES :=$(PROJECT_PATH)
LOCAL_CFLAGS := -I $(PROJECT_PATH)/Human/include/Utilities/
LOCAL_CFLAGS += -I $(PROJECT_PATH)/Human/include/Utilities/vorbis/

MY_LOCAL_SRC_FILES := $(wildcard $(PROJECT_PATH)/Application/*.cpp)

LOCAL_SRC_FILES := $(subst jni/, , $(MY_LOCAL_SRC_FILES))
LOCAL_STATIC_LIBRARIES := libopifex-human
include $(BUILD_SHARED_LIBRARY)

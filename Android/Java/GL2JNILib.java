/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.opifex.smrf;


import android.content.res.AssetManager;
import tv.ouya.console.api.OuyaFacade;

// Wrapper for native library
public class GL2JNILib {

     static {
         System.loadLibrary("smrf");
     }

    /**
     * @param width the current view width
     * @param height the current view height
     */
     public static native void init(int width, int height, AssetManager assetManager);
     public static native void step();
     public static native void setControllerButton(int player, int button, int state);
     public static native void setControllerAxes(int player, int axes, float position);

    public static final String DEVELOPER_ID = "8e0c74ec-c52c-4de4-89f0-3944b7145489";

     public static String Purchase(String text) {
        OuyaFacade ouyaFacade = OuyaFacade.getInstance();
        //ouyaFacade.init(this, DEVELOPER_ID);
        if(ouyaFacade.isRunningOnOUYAHardware()) {
            return "OUYA";
        }
         return  "Not OUYA";
     }
}

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

package com.opifex;

import android.content.res.AssetManager;
import tv.ouya.console.api.OuyaController;
import tv.ouya.console.api.OuyaFacade;

// Wrapper for native library
public class GL2JNILib {

     static {
         System.loadLibrary("opengine");
     }

    /**
     * @param width the current view width
     * @param height the current view height
     */
     public static native void start();
     public static native void init(int width, int height, AssetManager assetManager);
     public static native int step(AssetManager assetManager);
     public static native void destroy();
	 public static native void touch(int evt, float x, float y);

    //public static final String DEVELOPER_ID = "8e0c74ec-c52c-4de4-89f0-3944b7145489";

     public static GL2JNIView Instance;

     public static String Purchase(String text) {
        
        if(Instance.IsOuya()) {
           return "OUYA";
        }
         return  "Not OUYA";
     }

     public static int IsButton(){
        return Instance.ButtonState();
        // OuyaController c = OuyaController.getControllerByPlayer(0);
        // if(c == null){
        //     return -1;
        // }
        // return c.getButton(OuyaController.BUTTON_O) ? 1 : 0;
     }
}

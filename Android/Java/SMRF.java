/*
 * Copyright (C) 2009 The Android Open Source Project
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
import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.WindowManager;
import android.view.*;
import android.nfc.Tag;

public class SMRF extends Activity {
	GL2JNIView mView;
	
	@Override protected void onCreate(Bundle icicle) {
		super.onCreate(icicle);
		AssetManager assetManager = getAssets();
		mView = new GL2JNIView(getApplication(), assetManager);
		setContentView(mView);
	}
	
	@Override protected void onPause(){
		super.onPause();
		mView.onPause();
	}
	
	@Override protected void onResume(){
		super.onResume();
		mView.onResume();
	}

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        return mView.onKeyDown(keyCode,  event);
    }

    @Override
    public boolean onKeyUp(int keyCode, KeyEvent event) {
        return mView.onKeyUp(keyCode, event);
    }

    @Override
    public boolean onGenericMotionEvent(MotionEvent event) {
        if((event.getSource() & InputDevice.SOURCE_CLASS_JOYSTICK) == 0){
            //Not a joystick movement, so ignore it.
            return false;
        }
        return mView.onGenericMotionEvent(event);
    }
}

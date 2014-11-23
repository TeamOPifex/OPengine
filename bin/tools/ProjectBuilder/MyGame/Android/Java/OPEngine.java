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
package com.opifex;

import android.content.res.AssetManager;
import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.WindowManager;
import android.view.*;
import android.nfc.Tag;

import tv.ouya.console.api.OuyaFacade;
import tv.ouya.console.api.OuyaController;

public class OPEngine extends Activity {
	GL2JNIView mView;
	
	int playerOne = -1;
	int playerTwo = -1;
	int playerThree = -1;
	int playerFour = -1;
	
	public static final String DEVELOPER_ID = "8e0c74ec-c52c-4de4-89f0-3944b7145489";

	@Override protected void onCreate(Bundle icicle) {
		OuyaFacade instance = OuyaFacade.getInstance();
		instance.init(this, DEVELOPER_ID);

		super.onCreate(icicle);
		AssetManager assetManager = getAssets();
		mView = new GL2JNIView(getApplication(), assetManager, instance);
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
    protected void onStop() {
        super.onStop();
        // The activity is no longer visible (it is now "stopped")
		mView.onStop();
    }
	
    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
    	boolean handled = OuyaController.onKeyDown(keyCode, event);
    	return handled || super.onKeyDown(keyCode, event);
        //return mView.onKeyDown(keyCode, getPlayerIndex(event), event);
    }

    @Override
    public boolean onKeyUp(int keyCode, KeyEvent event) {
	    boolean handled = OuyaController.onKeyUp(keyCode, event);
	    return handled || super.onKeyUp(keyCode, event);
        //return mView.onKeyUp(keyCode, getPlayerIndex(event), event);
    }

    @Override
    public boolean onGenericMotionEvent(MotionEvent event) {
	    boolean handled = OuyaController.onGenericMotionEvent(event);
	    return handled || super.onGenericMotionEvent(event);

        // if((event.getSource() & InputDevice.SOURCE_CLASS_JOYSTICK) == 0){
        //     //Not a joystick movement, so ignore it.
        //     return false;
        // }
        // return mView.onGenericMotionEvent(getPlayerIndex(event), event);
    }
	
	@Override
	public boolean onTouchEvent(MotionEvent me) {

		int action = me.getAction();
		
		if(action == MotionEvent.ACTION_DOWN) {
			mView.DownAction(me.getX(), me.getY());
		}

		if (action == MotionEvent.ACTION_MOVE) {
			mView.MoveAction(me.getX(), me.getY());
		}

		return true;
	}
	
	public int getPlayerIndex(InputEvent event){
		final int deviceId = event.getDeviceId();
		
		if(playerOne < 0) {
			playerOne = deviceId;
		}
		if(playerOne == deviceId) {
			return 1;
		}
		
		if(playerTwo < 0) {
			playerTwo = deviceId;
		}
		if(playerTwo == deviceId) {
			return 2;
		}
				
		if(playerThree < 0) {
			playerThree = deviceId;
		}
		if(playerThree == deviceId) {
			return 3;
		}
				
		if(playerFour < 0) {
			playerFour = deviceId;
		}
		if(playerFour == deviceId) {
			return 4;
		}
		
		return -1;
	}
}

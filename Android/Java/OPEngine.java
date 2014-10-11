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

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;

import tv.ouya.console.api.OuyaController;
import tv.ouya.console.api.OuyaFacade;


public class OPEngine extends Activity {

	 
	public static final String DEVELOPER_ID = "8e0c74ec-c52c-4de4-89f0-3944b7145489";
	
    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);

        Intent intent = new Intent(OPEngine.this, android.app.NativeActivity.class);
        OPEngine.this.startActivity(intent);
    }
}

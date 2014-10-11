package com.opifex;

public class OPEngine extends android.app.NativeActivity {
    static {
       System.loadLibrary("opengine");
    }
 }
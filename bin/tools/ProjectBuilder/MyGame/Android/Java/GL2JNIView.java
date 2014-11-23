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
/*
 * Copyright (C) 2008 The Android Open Source Project
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


import android.content.res.AssetManager;
import android.content.Context;
import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.opengles.GL10;

import tv.ouya.console.api.OuyaController;
import tv.ouya.console.api.OuyaFacade;

/**
 * A simple GLSurfaceView sub-class that demonstrate how to perform
 * OpenGL ES 2.0 rendering into a GL Surface. Note the following important
 * details:
 *
 * - The class must use a custom context factory to enable 2.0 rendering.
 *   See ContextFactory class definition below.
 *
 * - The class must use a custom EGLConfigChooser to be able to select
 *   an EGLConfig that supports 2.0. This is done by providing a config
 *   specification to eglChooseConfig() that has the attribute
 *   EGL10.ELG_RENDERABLE_TYPE containing the EGL_OPENGL_ES2_BIT flag
 *   set. See ConfigChooser class definition below.
 *
 * - The class must select the surface's format, then choose an EGLConfig
 *   that matches it exactly (with regards to red/green/blue/alpha channels
 *   bit depths). Failure to do so would result in an EGL_BAD_MATCH error.
 */

class GL2JNIView extends GLSurfaceView {
    private static String TAG = "GL2JNIView";
    private static final boolean DEBUG = false;
	private static AssetManager _assetManager;
	private static boolean lIgnore;
	private static boolean rIgnore;
	private static Renderer rend;
    private static OuyaFacade facade;
	
	
    public GL2JNIView(Context context, AssetManager assetManager, OuyaFacade instance) {
        super(context);
		_assetManager = assetManager;
        init(false, 1, 0);
        GL2JNILib.Instance = this;
        OuyaController.init(context);

        facade = instance;
    }

	public void DownAction(float x, float y) {
		rend.Touch(1, x, y);
	}
	
	public void MoveAction(float x, float y) {
		rend.Touch(2, x, y);
	}
	
    public int ButtonState(){
        OuyaController c = OuyaController.getControllerByPlayer(0);
        if(c == null){
            return -1;
        }
        return c.getButton(OuyaController.BUTTON_O) ? 1 : 0;
    }

    public boolean IsOuya() {
        return facade.isRunningOnOUYAHardware();
    }

    public GL2JNIView(Context context, boolean translucent, int depth, int stencil, AssetManager assetManager) {
        super(context);
		_assetManager = assetManager;
        init(translucent, depth, stencil);
    }
	
    public boolean onKeyDown(int keyCode, int player, KeyEvent event) {
        if(keyCode == OuyaController.BUTTON_O) {
			rend.setButton(player, GamePadButtons.A, GamePadButtonState.DOWN);
            return true;
        } else if(keyCode == OuyaController.BUTTON_U) {
			rend.setButton(player, GamePadButtons.X, GamePadButtonState.DOWN);
            return true;
        } else if(keyCode == OuyaController.BUTTON_Y) {
			rend.setButton(player, GamePadButtons.Y, GamePadButtonState.DOWN);
            return true;
        } else if(keyCode == OuyaController.BUTTON_A) {
			rend.setButton(player, GamePadButtons.B, GamePadButtonState.DOWN);
            return true;
        } else if(keyCode == OuyaController.BUTTON_DPAD_DOWN) {
			rend.setButton(player, GamePadButtons.DPAD_DOWN, GamePadButtonState.DOWN);
            return true;
        } else if(keyCode == OuyaController.BUTTON_DPAD_LEFT) {
			rend.setButton(player, GamePadButtons.DPAD_LEFT, GamePadButtonState.DOWN);
            return true;
        } else if(keyCode == OuyaController.BUTTON_DPAD_UP) {
			rend.setButton(player, GamePadButtons.DPAD_UP, GamePadButtonState.DOWN);
            return true;
        } else if(keyCode == OuyaController.BUTTON_DPAD_RIGHT) {
			rend.setButton(player, GamePadButtons.DPAD_RIGHT, GamePadButtonState.DOWN);
            return true;
        } else if(keyCode == OuyaController.BUTTON_R1) {
			rend.setButton(player, GamePadButtons.RIGHT_SHOULDER, GamePadButtonState.DOWN);
            return true;
        } else if(keyCode == OuyaController.BUTTON_L1) {
			rend.setButton(player, GamePadButtons.LEFT_SHOULDER, GamePadButtonState.DOWN);
            return true;
        } else if (keyCode == OuyaController.BUTTON_R3) {
			rend.setButton(player, GamePadButtons.RIGHT_THUMB, GamePadButtonState.DOWN);
            return true;
        } else if (keyCode == OuyaController.BUTTON_L3) {
			rend.setButton(player, GamePadButtons.LEFT_THUMB, GamePadButtonState.DOWN);
            return true;
        } else if (keyCode == OuyaController.BUTTON_MENU) {
			rend.setButton(player, GamePadButtons.START, GamePadButtonState.DOWN);
            return true;
        }
		
        return super.onKeyDown(keyCode, event);        
    }

    public boolean onKeyUp(int keyCode, int player, KeyEvent event) {
	
		if(keyCode == OuyaController.BUTTON_O) {
			rend.setButton(player, GamePadButtons.A, GamePadButtonState.UP);
            return true;
        } else if(keyCode == OuyaController.BUTTON_U) {
			rend.setButton(player, GamePadButtons.X, GamePadButtonState.UP);
            return true;
        } else if(keyCode == OuyaController.BUTTON_Y) {
			rend.setButton(player, GamePadButtons.Y, GamePadButtonState.UP);
            return true;
        } else if(keyCode == OuyaController.BUTTON_A) {
			rend.setButton(player, GamePadButtons.B, GamePadButtonState.UP);
            return true;
        } else if(keyCode == OuyaController.BUTTON_DPAD_DOWN) {
			rend.setButton(player, GamePadButtons.DPAD_DOWN, GamePadButtonState.UP);
            return true;
        } else if(keyCode == OuyaController.BUTTON_DPAD_LEFT) {
			rend.setButton(player, GamePadButtons.DPAD_LEFT, GamePadButtonState.UP);
            return true;
        } else if(keyCode == OuyaController.BUTTON_DPAD_UP) {
			rend.setButton(player, GamePadButtons.DPAD_UP, GamePadButtonState.UP);
            return true;
        } else if(keyCode == OuyaController.BUTTON_DPAD_RIGHT) {
			rend.setButton(player, GamePadButtons.DPAD_RIGHT, GamePadButtonState.UP);
            return true;
        } else if(keyCode == OuyaController.BUTTON_R1) {
			rend.setButton(player, GamePadButtons.RIGHT_SHOULDER, GamePadButtonState.UP);
            return true;
        } else if(keyCode == OuyaController.BUTTON_L1) {
			rend.setButton(player, GamePadButtons.LEFT_SHOULDER, GamePadButtonState.UP);
            return true;
		} else if (keyCode == OuyaController.BUTTON_R3) {
			rend.setButton(player, GamePadButtons.RIGHT_THUMB, GamePadButtonState.UP);
            return true;
        } else if (keyCode == OuyaController.BUTTON_L3) {
			rend.setButton(player, GamePadButtons.LEFT_THUMB, GamePadButtonState.UP);
            return true;
        } else if (keyCode == OuyaController.BUTTON_MENU) {
			rend.setButton(player, GamePadButtons.START, GamePadButtonState.UP);
            return true;
        }
		
        return super.onKeyUp(keyCode, event);        
    }
	
    public boolean onGenericMotionEvent(int player, MotionEvent event) {
		rend.setAxes(player, GamePadAxes.LS_X, event.getAxisValue(OuyaController.AXIS_LS_X));
		rend.setAxes(player, GamePadAxes.LS_Y, event.getAxisValue(OuyaController.AXIS_LS_Y));
		rend.setAxes(player, GamePadAxes.RS_X, event.getAxisValue(OuyaController.AXIS_RS_X));
		rend.setAxes(player, GamePadAxes.RS_Y, event.getAxisValue(OuyaController.AXIS_RS_Y));
		rend.setAxes(player, GamePadAxes.L2, event.getAxisValue(OuyaController.AXIS_L2));
		rend.setAxes(player, GamePadAxes.R2, event.getAxisValue(OuyaController.AXIS_R2));
        return true;
    }
	
    private void init(boolean translucent, int depth, int stencil) {

        /* By default, GLSurfaceView() creates a RGB_565 opaque surface.
         * If we want a translucent one, we should change the surface's
         * format here, using PixelFormat.TRANSLUCENT for GL Surfaces
         * is interpreted as any 32-bit surface with alpha by SurfaceFlinger.
         */
        if (translucent) {
            this.getHolder().setFormat(PixelFormat.TRANSLUCENT);
        }

        /* Setup the context factory for 2.0 rendering.
         * See ContextFactory class definition below
         */
        setEGLContextFactory(new ContextFactory());

        /* We need to choose an EGLConfig that matches the format of
         * our surface exactly. This is going to be done in our
         * custom config chooser. See ConfigChooser class definition
         * below.
         */
        setEGLConfigChooser( translucent ?
                             new ConfigChooser(8, 8, 8, 8, depth, stencil) :
                             new ConfigChooser(5, 6, 5, 0, depth, stencil) );

        /* Set the renderer responsible for frame rendering */
		rend = new Renderer(_assetManager);
        setRenderer(rend);
    }
	
	public void onStop(){
		rend.Destroy();
	}
	
    private static class ContextFactory implements GLSurfaceView.EGLContextFactory {
        private static int EGL_CONTEXT_CLIENT_VERSION = 0x3098;
        public EGLContext createContext(EGL10 egl, EGLDisplay display, EGLConfig eglConfig) {
            Log.w(TAG, "creating OpenGL ES 2.0 context");
            checkEglError("Before eglCreateContext", egl);
            int[] attrib_list = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL10.EGL_NONE };
            EGLContext context = egl.eglCreateContext(display, eglConfig, EGL10.EGL_NO_CONTEXT, attrib_list);
            checkEglError("After eglCreateContext", egl);
            return context;
        }

        public void destroyContext(EGL10 egl, EGLDisplay display, EGLContext context) {
            egl.eglDestroyContext(display, context);
        }
    }

    private static void checkEglError(String prompt, EGL10 egl) {
        int error;
        while ((error = egl.eglGetError()) != EGL10.EGL_SUCCESS) {
            Log.e(TAG, String.format("%s: EGL error: 0x%x", prompt, error));
        }
    }

    private static class ConfigChooser implements GLSurfaceView.EGLConfigChooser {

        public ConfigChooser(int r, int g, int b, int a, int depth, int stencil) {
            mRedSize = r;
            mGreenSize = g;
            mBlueSize = b;
            mAlphaSize = a;
            mDepthSize = depth;
            mStencilSize = stencil;
        }

        /* This EGL config specification is used to specify 2.0 rendering.
         * We use a minimum size of 4 bits for red/green/blue, but will
         * perform actual matching in chooseConfig() below.
         */
        private static int EGL_OPENGL_ES2_BIT = 4;
        private static int[] s_configAttribs2 =
        {
            EGL10.EGL_RED_SIZE, 4,
            EGL10.EGL_GREEN_SIZE, 4,
            EGL10.EGL_BLUE_SIZE, 4,
            EGL10.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL10.EGL_NONE
        };

        public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display) {

            /* Get the number of minimally matching EGL configurations
             */
            int[] num_config = new int[1];
            egl.eglChooseConfig(display, s_configAttribs2, null, 0, num_config);

            int numConfigs = num_config[0];

            if (numConfigs <= 0) {
                throw new IllegalArgumentException("No configs match configSpec");
            }

            /* Allocate then read the array of minimally matching EGL configs
             */
            EGLConfig[] configs = new EGLConfig[numConfigs];
            egl.eglChooseConfig(display, s_configAttribs2, configs, numConfigs, num_config);

            if (DEBUG) {
                 printConfigs(egl, display, configs);
            }
            /* Now return the "best" one
             */
            return chooseConfig(egl, display, configs);
        }

        public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display,
                EGLConfig[] configs) {
            for(EGLConfig config : configs) {
                int d = findConfigAttrib(egl, display, config,
                        EGL10.EGL_DEPTH_SIZE, 0);
                int s = findConfigAttrib(egl, display, config,
                        EGL10.EGL_STENCIL_SIZE, 0);

                // We need at least mDepthSize and mStencilSize bits
                if (d < mDepthSize || s < mStencilSize)
                    continue;

                // We want an *exact* match for red/green/blue/alpha
                int r = findConfigAttrib(egl, display, config,
                        EGL10.EGL_RED_SIZE, 0);
                int g = findConfigAttrib(egl, display, config,
                            EGL10.EGL_GREEN_SIZE, 0);
                int b = findConfigAttrib(egl, display, config,
                            EGL10.EGL_BLUE_SIZE, 0);
                int a = findConfigAttrib(egl, display, config,
                        EGL10.EGL_ALPHA_SIZE, 0);

                if (r == mRedSize && g == mGreenSize && b == mBlueSize && a == mAlphaSize)
                    return config;
            }
            return null;
        }

        private int findConfigAttrib(EGL10 egl, EGLDisplay display,
                EGLConfig config, int attribute, int defaultValue) {

            if (egl.eglGetConfigAttrib(display, config, attribute, mValue)) {
                return mValue[0];
            }
            return defaultValue;
        }

        private void printConfigs(EGL10 egl, EGLDisplay display,
            EGLConfig[] configs) {
            int numConfigs = configs.length;
            Log.w(TAG, String.format("%d configurations", numConfigs));
            for (int i = 0; i < numConfigs; i++) {
                Log.w(TAG, String.format("Configuration %d:\n", i));
                printConfig(egl, display, configs[i]);
            }
        }

        private void printConfig(EGL10 egl, EGLDisplay display,
                EGLConfig config) {
            int[] attributes = {
                    EGL10.EGL_BUFFER_SIZE,
                    EGL10.EGL_ALPHA_SIZE,
                    EGL10.EGL_BLUE_SIZE,
                    EGL10.EGL_GREEN_SIZE,
                    EGL10.EGL_RED_SIZE,
                    EGL10.EGL_DEPTH_SIZE,
                    EGL10.EGL_STENCIL_SIZE,
                    EGL10.EGL_CONFIG_CAVEAT,
                    EGL10.EGL_CONFIG_ID,
                    EGL10.EGL_LEVEL,
                    EGL10.EGL_MAX_PBUFFER_HEIGHT,
                    EGL10.EGL_MAX_PBUFFER_PIXELS,
                    EGL10.EGL_MAX_PBUFFER_WIDTH,
                    EGL10.EGL_NATIVE_RENDERABLE,
                    EGL10.EGL_NATIVE_VISUAL_ID,
                    EGL10.EGL_NATIVE_VISUAL_TYPE,
                    0x3030, // EGL10.EGL_PRESERVED_RESOURCES,
                    EGL10.EGL_SAMPLES,
                    EGL10.EGL_SAMPLE_BUFFERS,
                    EGL10.EGL_SURFACE_TYPE,
                    EGL10.EGL_TRANSPARENT_TYPE,
                    EGL10.EGL_TRANSPARENT_RED_VALUE,
                    EGL10.EGL_TRANSPARENT_GREEN_VALUE,
                    EGL10.EGL_TRANSPARENT_BLUE_VALUE,
                    0x3039, // EGL10.EGL_BIND_TO_TEXTURE_RGB,
                    0x303A, // EGL10.EGL_BIND_TO_TEXTURE_RGBA,
                    0x303B, // EGL10.EGL_MIN_SWAP_INTERVAL,
                    0x303C, // EGL10.EGL_MAX_SWAP_INTERVAL,
                    EGL10.EGL_LUMINANCE_SIZE,
                    EGL10.EGL_ALPHA_MASK_SIZE,
                    EGL10.EGL_COLOR_BUFFER_TYPE,
                    EGL10.EGL_RENDERABLE_TYPE,
                    0x3042 // EGL10.EGL_CONFORMANT
            };
            String[] names = {
                    "EGL_BUFFER_SIZE",
                    "EGL_ALPHA_SIZE",
                    "EGL_BLUE_SIZE",
                    "EGL_GREEN_SIZE",
                    "EGL_RED_SIZE",
                    "EGL_DEPTH_SIZE",
                    "EGL_STENCIL_SIZE",
                    "EGL_CONFIG_CAVEAT",
                    "EGL_CONFIG_ID",
                    "EGL_LEVEL",
                    "EGL_MAX_PBUFFER_HEIGHT",
                    "EGL_MAX_PBUFFER_PIXELS",
                    "EGL_MAX_PBUFFER_WIDTH",
                    "EGL_NATIVE_RENDERABLE",
                    "EGL_NATIVE_VISUAL_ID",
                    "EGL_NATIVE_VISUAL_TYPE",
                    "EGL_PRESERVED_RESOURCES",
                    "EGL_SAMPLES",
                    "EGL_SAMPLE_BUFFERS",
                    "EGL_SURFACE_TYPE",
                    "EGL_TRANSPARENT_TYPE",
                    "EGL_TRANSPARENT_RED_VALUE",
                    "EGL_TRANSPARENT_GREEN_VALUE",
                    "EGL_TRANSPARENT_BLUE_VALUE",
                    "EGL_BIND_TO_TEXTURE_RGB",
                    "EGL_BIND_TO_TEXTURE_RGBA",
                    "EGL_MIN_SWAP_INTERVAL",
                    "EGL_MAX_SWAP_INTERVAL",
                    "EGL_LUMINANCE_SIZE",
                    "EGL_ALPHA_MASK_SIZE",
                    "EGL_COLOR_BUFFER_TYPE",
                    "EGL_RENDERABLE_TYPE",
                    "EGL_CONFORMANT"
            };
            int[] value = new int[1];
            for (int i = 0; i < attributes.length; i++) {
                int attribute = attributes[i];
                String name = names[i];
                if ( egl.eglGetConfigAttrib(display, config, attribute, value)) {
                    Log.w(TAG, String.format("  %s: %d\n", name, value[0]));
                } else {
                    // Log.w(TAG, String.format("  %s: failed\n", name));
                    while (egl.eglGetError() != EGL10.EGL_SUCCESS);
                }
            }
        }

        // Subclasses can adjust these values:
        protected int mRedSize;
        protected int mGreenSize;
        protected int mBlueSize;
        protected int mAlphaSize;
        protected int mDepthSize;
        protected int mStencilSize;
        private int[] mValue = new int[1];
    }

    private static class Renderer implements GLSurfaceView.Renderer {
		static AssetManager _assetManager;
		
		public Renderer(AssetManager assetManager){
			_assetManager = assetManager;
			GL2JNILib.start();
			//GL2JNILib.setControllerButton(1, 0, 0);
		}
		
        public void setButton(int player, GamePadButtons button, GamePadButtonState state) {
			//GL2JNILib.setControllerButton(player, button.getCode(), state.getCode());
		}
		
        public void setAxes(int player, GamePadAxes axes, float position) {
			//GL2JNILib.setControllerAxes(player, axes.getCode(), position);
		}
		
        public void onDrawFrame(GL10 gl) {
            if(GL2JNILib.step(_assetManager) > 0) {
				GL2JNILib.destroy();
				System.exit(0);
			}
        }

        public void onSurfaceChanged(GL10 gl, int width, int height) {
            GL2JNILib.init(width, height, _assetManager);
        }

        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
            // Do nothing.
        }
		
		public void Touch(int evt, float x, float y) {
			GL2JNILib.touch(evt, x, y);
		}
		
		public void Destroy() {
			GL2JNILib.destroy();
			System.exit(0);
		}
    }
}

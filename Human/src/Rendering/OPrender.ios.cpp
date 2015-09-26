#include "./Human/include/Rendering/OPrender.h"

#if defined(OPIFEX_IOS)

#include "./Core/include/OPcore.h"

i8 OPRENDER_INITIALIZED = 0;
i32 OPRENDER_WIDTH = 1280;
i32 OPRENDER_HEIGHT = 720;
i32 OPRENDER_SCREEN_WIDTH = 1280;
i32 OPRENDER_SCREEN_HEIGHT = 720;
i32 OPRENDER_SCALED_WIDTH = 1280;
i32 OPRENDER_SCALED_HEIGHT = 720;
f32 OPRENDER_SCREEN_WIDTH_SCALE = 1;
f32 OPRENDER_SCREEN_HEIGHT_SCALE = 1;
OPint OPRENDER_FULLSCREEN = false;
OPint OPRENDER_HAS_FOCUS = 1;
OPint glfwInitialized = 0;
OPuint OPRENDER_VAO = 0;


OPint OPrenderInit(i32 width, i32 height){
    return 0;
}

void  OPrenderSetScreenSize(OPuint width, OPuint height) {
    OPRENDER_SCREEN_WIDTH = width;
    OPRENDER_SCREEN_HEIGHT = height;
}

void OPrenderCull(OPint state) {
    if (state) {
        glEnable(GL_CULL_FACE);
    }
    else {
        glDisable(GL_CULL_FACE);
    }
}

void OPrenderCullMode(OPint state) {
    if (state) {
        glCullFace(GL_FRONT);
    }
    else {
        glCullFace(GL_BACK);
    }
}
//-----------------------------------------------------------------------------
void  OPrenderClear(f32 r, f32 g, f32 b, f32 a){
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//-----------------------------------------------------------------------------
void  OPrenderSetViewport(OPuint x, OPuint y, OPuint width, OPuint height){
    glViewport(x, y, width, height);
    OPRENDER_WIDTH  = width;
    OPRENDER_HEIGHT = height;
}

void OPrenderResetViewport() {
    OPrenderSetViewport(0, 0, OPRENDER_SCREEN_WIDTH, OPRENDER_SCREEN_HEIGHT);
}

//-----------------------------------------------------------------------------
OPint OPrenderGetWidth(){
    return OPRENDER_WIDTH;
}
//-----------------------------------------------------------------------------
OPint OPrenderGetHeight(){
    return OPRENDER_HEIGHT;
}
//-----------------------------------------------------------------------------
OPfloat OPrenderGetAspectRatio(){
    ASSERT(OPrenderGetHeight() > 0, "Height was not greater than 0, there was problem getting width and height");
    return OPrenderGetWidth() / (OPfloat)OPrenderGetHeight();
}

OPfloat aspect;
//-----------------------------------------------------------------------------
OPfloat OPrenderGetWidthAspectRatio(){
    ASSERT(OPRENDER_WIDTH > 0, "Height was not greater than 0, there was problem getting width and height");
    aspect = OPRENDER_HEIGHT / (OPfloat)OPRENDER_WIDTH;
    return aspect > 1.0f ? 1.0f : aspect;
}
//-----------------------------------------------------------------------------
OPfloat OPrenderGetHeightAspectRatio(){
    ASSERT(OPRENDER_HEIGHT > 0, "Height was not greater than 0, there was problem getting width and height");
    aspect = OPRENDER_WIDTH / (OPfloat)OPRENDER_HEIGHT;
    return aspect > 1.0f ? 1.0f : aspect;
}

//-----------------------------------------------------------------------------
void  OPrenderSwapBuffer(){
    //eglSwapBuffers(display, surface);
}
//-----------------------------------------------------------------------------
void  OPrenderPresent(){
    //eglSwapBuffers(display, surface);
}
//-----------------------------------------------------------------------------
void  OPrenderDepth(OPint state){
    if(state)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);
}

void OPrenderDepthWrite(OPint state) {
    if (state)
        glDepthMask(GL_TRUE);
    else
        glDepthMask(GL_FALSE);
}



void OPrenderBlend(OPint state) {
    if (state)
        glEnable(GL_BLEND);
    else
        glDisable(GL_BLEND);
}
//-----------------------------------------------------------------------------
void  OPrenderShutdown(){
    
}


#endif

//
//  GameViewController.m
//  OPengine
//
//  Created by Garrett Hoofman on 2/8/16.
//  Copyright © 2016 Garrett Hoofman. All rights reserved.
//

#import "GameViewController.h"
#import <OpenGLES/ES2/glext.h>
#include "./Core/include/OPlog.h"
#include "./Core/include/OPcore.h"
#include "./Human/include/Rendering/OPrender.h"


///////////////////////////////
// YOUR APPLICATION HEADER FILE GOES HERE
///////////////////////////////
#include "./Application/Main.h"

@interface GameViewController () {

}

@property (strong, nonatomic) EAGLContext *context;
@property (strong, nonatomic) GLKBaseEffect *effect;

- (void)setupGL;
- (void)tearDownGL;

@end

@implementation GameViewController

- (void)viewDidLoad
{
    [super viewDidLoad];

    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];

    if (!self.context) {
        NSLog(@"Failed to create ES context");
    }

    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;

    [self setupGL];



    OPRENDER_WIDTH = self.view.bounds.size.width;
    OPRENDER_HEIGHT = self.view.bounds.size.height;

    ///////////////////////////////
    // YOUR APPLICATION SETUP FUNCTION GOES HERE
    ///////////////////////////////
    ApplicationSetup();

    OPstart(0, NULL);
}

- (void)dealloc
{
    [self tearDownGL];

    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];

    if ([self isViewLoaded] && ([[self view] window] == nil)) {
        self.view = nil;

        [self tearDownGL];

        if ([EAGLContext currentContext] == self.context) {
            [EAGLContext setCurrentContext:nil];
        }
        self.context = nil;
    }

    // Dispose of any resources that can be recreated.
}

- (BOOL)prefersStatusBarHidden {
    return YES;
}

- (void)setupGL
{
    [EAGLContext setCurrentContext:self.context];

}

- (void)tearDownGL
{
    [EAGLContext setCurrentContext:self.context];

    self.effect = nil;
}

#pragma mark - GLKView and GLKViewController delegate methods

- (void)update
{
    // iOS needs to control the update/render loop because of the view
    OPstartUpdate();
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    // The view does the OPrenderPresent
    OPstartRender();
}

#pragma mark -  OpenGL ES 2 shader compilation

@end

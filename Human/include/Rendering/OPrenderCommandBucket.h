#pragma once

struct OPrenderCommand;
struct OPrenderCommandBucketKey;
struct OPrenderCommandBucket;

typedef struct OPrenderCommand OPrenderCommand;
typedef struct OPrenderCommandBucketKey OPrenderCommandBucketKey;
typedef struct OPrenderCommandBucket OPrenderCommandBucket;

#include "./Human/include/Rendering/OPcam.h"
#include "./Human/include/Rendering/OPframeBuffer.h"
#include "./Human/include/Rendering/OPmodel.h"
#include "./Human/include/Rendering/Commands/OPrenderCommandDrawIndexed.h"
#include "./Data/include/OPradixSort.h"
#include "./Data/include/OPallocLinear.h"

//-----------------------------------------------------------------------------
//   _____ _                   _
//  / ____| |                 | |
// | (___ | |_ _ __ _   _  ___| |_ ___
//  \___ \| __| '__| | | |/ __| __/ __|
//  ____) | |_| |  | |_| | (__| |_\__ \
// |_____/ \__|_|   \__,_|\___|\__|___/

struct OPrenderCommand {
	// TODO: (garrett) allow linked list of commands
	// For example, you might want to copy data to the gpu
	// and then render that data. The sort key would be the
	// same so it should be a linked list of commands to draw
	void* next;
	void* data; // The data for the draw command
				// This is a function pointer to the function that will handle
				// the draw call for the *data
	void(*dispatch)(void*, OPcam*);
};

// A key for the OPcommandBucket that will be sorted for rendering order
struct OPrenderCommandBucketKey {
	ui64 key; // Key which will sort the draw call
	OPrenderCommand* command; // Pointer to the data to render
};

struct OPrenderCommandBucket {
	// Number of draw calls this bucket can support
	OPuint bucketSize = 0;
	// The linear allocator used for this creating Command data
	OPallocator* allocator = NULL;
	OPallocatorLinear* internalAllocator = NULL;

	// The current count of commands/keys
    ui32 keyIndex = 0;
	// The keys & ordering of the draw calls
    OPrenderCommandBucketKey* keys = NULL;
	// A duplicate data segment used for sorting (Radix)
    OPrenderCommandBucketKey* copykeys = NULL;
	// An array of the actual data for the draw commands
	OPrenderCommand* commands = NULL;

	// The camera should not change for a command bucket
    OPcam** camera = NULL;
	// The target framebuffers will not change for a command bucket
    OPframeBuffer* frameBuffer[4];

	// Simple wrapper functions
	// These are just convenient functions for calling the C style functions

	void Init(OPuint bucketSize, OPcam** camera);
	void Init(OPuint bucketSize, OPcam** camera, OPallocator* allocator);
	void Submit(ui64 key, void(*dispatch)(void*, OPcam*), void* data, void* next);
	void Sort();
	void Flush(bool keep);
	void Render();
	void Submit(OPmodel* model, OPmat4* world, OPmaterialInstance** material);
	void Submit(OPmodel* model, OPmat4* world, OPmaterialInstance* material);
	void Submit(OPmesh* mesh, OPmat4* world, OPmaterialInstance* material);
	void Destroy();

	// Helper draw commands already in the engine
	// Users will be able to define their own, just won't be a helper function
	// in the struct itself unless they modify the OPengine source itself
	OPrenderCommandDrawIndexed* CreateDrawIndexed();

	inline void Submit(ui64 key, void(*dispatch)(void*, OPcam*), void* data) {
		Submit(key, dispatch, data, NULL);
	}
	inline void Flush() { Flush(false); }

	static OPrenderCommandBucket* Create(OPuint bucketSize, OPcam** camera);
	static OPrenderCommandBucket* Create(OPuint bucketSize, OPcam** camera, OPallocator* allocator);
};

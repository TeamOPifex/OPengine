#ifndef OPENGINE_HUMAN_RENDERING_COMMAND_BUCKET
#define OPENGINE_HUMAN_RENDERING_COMMAND_BUCKET

#include "./Human/include/Rendering/OPcam.h"
#include "./Human/include/Rendering/OPframeBuffer.h"
#include "./Data/include/OPradixSort.h"
#include "./Data/include/OPallocLinear.h"

//-----------------------------------------------------------------------------
//  _____                     _____                  _____  _               _   _
// |  __ \                   |  __ \                |  __ \(_)             | | (_)
// | |__) _ __ ___   ______  | |__) _ __ ___   ___  | |  | |_ _ __ ___  ___| |_ ___   _____ ___
// |  ___| '__/ _ \ |______| |  ___| '__/ _ \ / __| | |  | | | '__/ _ \/ __| __| \ \ / / _ / __|
// | |   | | |  __/          | |   | | | (_) | (__  | |__| | | | |  __| (__| |_| |\ V |  __\__ \
// |_|   |_|  \___|          |_|   |_|  \___/ \___| |_____/|_|_|  \___|\___|\__|_| \_/ \___|___/

struct OPcommandDrawCommand;
struct OPcommandBucketKey;
struct OPcommandBucket;
typedef struct OPcommandDrawCommand OPcommandDrawCommand;
typedef struct OPcommandBucketKey OPcommandBucketKey;
typedef struct OPcommandBucket OPcommandBucket;

#include "./Human/include/Rendering/Commands/OPcommandDrawIndexed.h"

//-----------------------------------------------------------------------------
//   _____ _                   _
//  / ____| |                 | |
// | (___ | |_ _ __ _   _  ___| |_ ___
//  \___ \| __| '__| | | |/ __| __/ __|
//  ____) | |_| |  | |_| | (__| |_\__ \
// |_____/ \__|_|   \__,_|\___|\__|___/

struct OPcommandDrawCommand {
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
struct OPcommandBucketKey {
	ui64 key; // Key which will sort the draw call
	OPcommandDrawCommand* command; // Pointer to the data to render
};

struct OPcommandBucket {
	// Number of draw calls this bucket can support
	OPuint bucketSize;
	// The linear allocator used for this creating Command data
    OPallocator* allocator;

	// The current count of commands/keys
    ui32 keyIndex;
	// The keys & ordering of the draw calls
    OPcommandBucketKey* keys;
	// A duplicate data segment used for sorting (Radix)
    OPcommandBucketKey* copykeys;
	// An array of the actual data for the draw commands
    OPcommandDrawCommand* commands;

	// The camera should not change for a command bucket
    OPcam* camera;
	// The target framebuffers will not change for a command bucket
    OPframeBuffer* frameBuffer[4];

	// Determines whether the command bucket will free the allocator
	// on OPcommandBucketDestroy/Free
	i8 controlOfAllocator;

	// Simple wrapper functions
	// These are just convenient functions for calling the C style functions

	void Init(OPuint bucketSize, OPcam* camera);
	void Init(OPuint bucketSize, OPcam* camera, OPallocator* allocator);
	void Submit(ui64 key, void(*dispatch)(void*, OPcam*), void* data, void* next);
	void Sort();
	void Flush();
	void Render();
	void CreateDrawIndexedSubmit(OPmodel* model, OPmaterial* material, OPtexture* texture);
	void CreateDrawIndexedSubmit(OPmodelTextured* model, OPmaterial* material);

	// Helper draw commands already in the engine
	// Users will be able to define their own, just won't be a helper function
	// in the struct itself unless they modify the OPengine source itself
	OPcommandDrawIndexed* CreateDrawIndexed();

	inline void Submit(ui64 key, void(*dispatch)(void*, OPcam*), void* data) {
		Submit(key, dispatch, data, NULL);
	}

	static OPcommandBucket* Create(OPuint bucketSize, OPcam* camera);
	static OPcommandBucket* Create(OPuint bucketSize, OPcam* camera, OPallocator* allocator);
};

#endif

#include "./Core/include/Assert.h"
#include "./Data/include/File.h"
#include "./Data/include/ContentManager.h"
#include "./Human/include/Utilities/ImagePNG.h"
#include "./Human/include/Rendering/Sprite/SpriteSheet.h"

OPint OPspriteSheetLoad(const OPchar* filename, OPspriteSheet** ss){
	OPlog("OPspriteSheetLoad() - entered");
	OPstream* str = OPreadFileLarge(filename, 1024);
	i32 width, height;
	i32 sprites, frames;
	OPspriteFrame* frameData;
	OPtexture *sheet, *temp;

	ASSERT(OP_CMAN_ASSETLOADERS,
		"OPspriteSheetLoad() - OP_CMAN_HASHMAP null, is content man initialized?"
	);

	// allocate space for the texture before it's created
	// this will allow us to refer to its location without
	// yet having loaded it.
	sheet = (OPtexture*)OPalloc(sizeof(OPtexture));

	// read the dimensions of the image
	width  = OPreadi32(str);
	height = OPreadi32(str);

	OPlog("Sprite sheet %dx%d", width, height);

	// read the sprite, and frame counts
	sprites = OPreadi32(str);
	frames  = OPreadi32(str);

	OPlog("Sprites: %d, Frames: %d", sprites, frames);

	// allocate memory for the sprite sheet struct
	*ss = (OPspriteSheet*)OPalloc(sizeof(OPspriteSheet));
	(*ss)->Names = (OPchar**)OPalloc(sizeof(OPchar*) * sprites);

	// allocate contiguious frame memory segment
	frameData = (OPspriteFrame*)OPalloc(sizeof(OPspriteSheet) * frames);

	{ // Load all sprites and frames
		OPint frameNum = 0;
		for(OPint i = sprites; i--;){
			// read the name, and the sprite flags
			// setup a proxy var for frame data
			OPasset* assetBucket = NULL;
			char* name = (*ss)->Names[i] = OPreadstring(str);
			ui32 flags = OPreadi32(str);
			ui32 spriteFrames = 1;			
			OPspriteFrame* spriteFrameData = frameData + frameNum;
			OPsprite* sprite = (OPsprite*)OPalloc(sizeof(OPsprite));

			OPlog("Sprite '%s' @ %x", name, sprite);
			
			// zero the sprite structure out
			OPbzero(sprite, sizeof(OPsprite));

			if(flags){
				// this sprite has animation frames	
				spriteFrames = OPreadi32(str);
			}

			OPlog("\tLoading...");			

			// Load all the frames
			for(OPint j = 0; j < spriteFrames; j++){
				i32 x, y, w, h;
				// read the sprite dimensions, scale them
				// to fit within UV space
				OPvec2 offset = {
					(x = OPreadi32(str)) / (OPfloat)width,
					(y = OPreadi32(str)) / (OPfloat)height
				};
				OPvec2 size = {
					(w = OPreadi32(str)) / (OPfloat)width,
					(h = OPreadi32(str)) / (OPfloat)height
				};

				OPlog("Y offset %f", y / (OPfloat)height);
				OPlog("Height %f", h / (OPfloat)height);
				OPlog("(1 - %f) - %f = %f", offset.y, size.y, (1-offset.y)-size.y);

				//offset.y = (1 - offset.y) - size.y;
				//size.y *= -1.0f;

				// setup frame structure, copy into the frame buffer
				OPspriteFrame frame = {
					offset,
					size,
					1 // TODO
				};

				spriteFrameData[j] = frame;
				OPlog("\t(%d,%d) (%d,%d)",
					x, y,
					w, h
				); 		
				OPlog("\t(%f,%f) (%f,%f)",
					offset.x, offset.y, 
					/*spriteFrameData[j].Offset.x, spriteFrameData[j].Offset.y,*/
					spriteFrameData[j].Size.x, spriteFrameData[j].Size.y
				); 
			}

			// set the frame count, and the pointer to
			// to this sprites's frame data.
			sprite->FrameCount = spriteFrames;
			sprite->Frames     = spriteFrameData;
			sprite->Sheet      = sheet;	

			// increment the frame number for the next sprite
			frameNum += spriteFrames;
	                
			// Insert the sprite into the content manager's hashmap
        	        OPlog("Inserting sprite '%s'", name);

			// create the asset to insert into the hashmap
			if(!(assetBucket = (OPasset*)OPalloc(sizeof(OPasset))))
				return OP_CMAN_BUCKET_ALLOC_FAILED;
			assetBucket->Asset = (void*)sprite;
			assetBucket->Unload = NULL;
			assetBucket->Dirty = 0;
			OPhashMapPut(&OP_CMAN_HASHMAP, name, assetBucket);
		}
	}

	// load the png image data
	OPimagePNGLoadStream(str, str->_pointer, &temp);
	OPlog("Loaded PNG!");
	
	// copy the texture's data into the pre allocated texture
	// then clean up the temp texture object
	OPmemcpy(sheet, temp, sizeof(OPtexture));
	OPfree(temp);

	OPlog("Done!");

	return 1;
}

OPint OPspriteSheetUnload(void* ss){
	return 1;
}

#include "./Human/include/Rendering/Sprite/OPspriteSheet.h"
#include "./Human/include/Utilities/ImagePNG.h"
#include "./Data/include/OPfile.h"
#include "./Data/include/OPcman.h"
#include "./Data/include/OPstring.h"
#include "./Core/include/Assert.h"

void __opSpriteScaleFrames(OPtexture* tex, OPspriteSheet* ss) {
	ASSERT(tex, "__opSpriteScaleFrames() - texture null");
	ASSERT(tex, "__opSpriteScaleFrames() - spritesheet null");
	OPint i = 0;
	OPlog("Texture Size: %d, %d", tex->Description.Width, tex->Description.Height);
	OPvec2 size = { tex->Description.Width, tex->Description.Height };

	for (i = ss->Sprites; i--;){
		OPsprite* s = (OPsprite*)OPcmanGet(ss->Names[i]);
		OPint j = s->FrameCount;

#ifdef _DEBUG
		OPlog("Resizing %s", ss->Names[i]);
#endif

			for (; j--;){

#ifdef _DEBUG
				OPlog("\t(%f,%f) (%f,%f)",
					s->Frames[j].Offset.x, s->Frames[j].Offset.y,
					s->Frames[j].Size.x, s->Frames[j].Size.y
					);
#endif

				s->Frames[j].Offset.x /= size.x;
				s->Frames[j].Offset.y /= size.y;
				s->Frames[j].Size /= size;
#ifdef _DEBUG
			OPlog("\t(%f,%f) (%f,%f)",
				s->Frames[j].Offset.x, s->Frames[j].Offset.y,
				s->Frames[j].Size.x, s->Frames[j].Size.y
				);
#endif
		}
	}
}

OPint OPspriteSheetLoad(const OPchar* filename, OPspriteSheet** ss){
	OPstream* str = OPreadFileLarge(filename, 1024);
	i32 width, height;
	i32 sprites, frames;
	OPspriteFrame* frameData;
	OPtexture *sheet, *temp;

	ui32 filenameLength = strlen(filename);
	ui32 filenameLengthWithoutExtension = filenameLength - 5 - 8 + 1;

	OPchar* filenameWithoutExtension = (OPchar*)OPalloc(sizeof(OPchar)* filenameLengthWithoutExtension);
	OPmemcpy(filenameWithoutExtension, filename + 8, filenameLengthWithoutExtension * sizeof(OPchar));
	filenameWithoutExtension[filenameLengthWithoutExtension - 1] = '\0';

	ASSERT(OP_CMAN_ASSETLOADERS,
		"OPspriteSheetLoad() - OP_CMAN_HASHMAP null, is content man initialized?"
		);

	// allocate space for the texture before it's created
	// this will allow us to refer to its location without
	// yet having loaded it.
	sheet = (OPtexture*)OPalloc(sizeof(OPtexture));

	// read the dimensions of the image
	width = OPreadi32(str);
	height = OPreadi32(str);

#ifdef _DEBUG
	OPlog("Sprite sheet %dx%d", width, height);
#endif

	// read the sprite, and frame counts
	sprites = OPreadi32(str);
	frames = OPreadi32(str);

#ifdef _DEBUG
	OPlog("Sprites: %d, Frames: %d", sprites, frames);
#endif

	// allocate memory for the sprite sheet struct
	*ss = (OPspriteSheet*)OPalloc(sizeof(OPspriteSheet));
	(*ss)->Names = (OPchar**)OPalloc(sizeof(OPchar*)* sprites);
	(*ss)->Sprites = sprites;
	(*ss)->Sheet = sheet;

	// allocate contiguious frame memory segment
	frameData = (OPspriteFrame*)OPalloc(sizeof(OPspriteSheet)* frames);

	{ // Load all sprites and frames
		OPint frameNum = 0;
		for (OPint i = sprites; i--;){
			// read the name, and the sprite flags
			// setup a proxy var for frame data
			OPasset* assetBucket = NULL;

			OPchar* nameData = OPreadstring(str);
			OPchar* name = (OPchar*)nameData;
			ui32 nameDataLength = strlen(name);

			// OPchar* finalName = (OPchar*)OPalloc(filenameLengthWithoutExtension + 1 + nameDataLength);
			// OPmemcpy(finalName, filenameWithoutExtension, filenameLengthWithoutExtension);
			// finalName[filenameLengthWithoutExtension] = '/';
			// OPmemcpy(finalName + filenameLengthWithoutExtension + 1, name, nameDataLength);
			// finalName[filenameLengthWithoutExtension + 1 + nameDataLength] = NULL;

			OPchar* finalNameSep = OPstringCreateMerged(filenameWithoutExtension, "/");
			OPchar* finalName = OPstringCreateMerged(finalNameSep, name);
			OPfree(finalNameSep);
			OPlog("Final Name! %s", finalName);

			(*ss)->Names[i] = finalName;
			OPfree(nameData);
			ui32 flags = OPreadi32(str);
			ui32 spriteFrames = 1;
			OPspriteFrame* spriteFrameData = frameData + frameNum;
			OPsprite* sprite = (OPsprite*)OPalloc(sizeof(OPsprite));

#ifdef _DEBUG
			OPlog("Sprite '%s' @ %x", finalName, sprite);
#endif

			// zero the sprite structure out
			OPbzero(sprite, sizeof(OPsprite));

			if (flags){
				// this sprite has animation frames	
				spriteFrames = OPreadi32(str);
			}

#ifdef _DEBUG
			OPlog("\tLoading...");
#endif

			// Load all the frames
			for (OPint j = 0; j < spriteFrames; j++){
				i32 x, y, w, h;
				// read the sprite dimensions, scale them
				// to fit within UV space
				OPvec2 offset = {
					(x = OPreadi32(str)),
					(y = OPreadi32(str))
				};
				OPvec2 size = {
					(w = OPreadi32(str)),
					(h = OPreadi32(str))
				};

				// setup frame structure, copy into the frame buffer
				OPspriteFrame frame = {
					offset,
					size,
					1 // TODO
				};

				spriteFrameData[j] = frame;
#ifdef _DEBUG
				OPlog("\t(%f,%f) (%f,%f)",
					spriteFrameData[j].Offset.x, spriteFrameData[j].Offset.y,
					spriteFrameData[j].Size.x, spriteFrameData[j].Size.y
					);
#endif
			}

			// set the frame count, and the pointer to
			// to this sprites's frame data.
			sprite->FrameCount = spriteFrames;
			sprite->Frames = spriteFrameData;
			sprite->Sheet = sheet;

			// increment the frame number for the next sprite
			frameNum += spriteFrames;

			// Insert the sprite into the content manager's hashmap
#ifdef _DEBUG
			OPlog("Inserting sprite '%s'", finalName);
#endif

			// create the asset to insert into the hashmap
			if (!(assetBucket = (OPasset*)OPalloc(sizeof(OPasset))))
				return OP_CMAN_BUCKET_ALLOC_FAILED;

			assetBucket->Asset = (void*)sprite;
			assetBucket->Unload = NULL;
			assetBucket->Dirty = 0;

#if defined(_DEBUG) && defined(OPIFEX_WINDOWS)
			assetBucket->Reload = NULL;
			assetBucket->AbsolutePath = NULL;
			assetBucket->FullPath = NULL;
			assetBucket->LastChange = NULL;
#endif
			OPhashMapPut(&OP_CMAN_HASHMAP, finalName, assetBucket);

#ifdef _DEBUG
			if(OPcmanIsLoaded(finalName)) {
				OPlog("OPSS %s loaded", finalName);
			} else {
				OPlog("OPSS FAILED TO LOAD %s", finalName);
			}
#endif
		}
	}

	// load the png image data
	OPimagePNGLoadStream(str, str->_pointer, &temp);
#ifdef _DEBUG
	OPlog("Loaded PNG!");
#endif

	// copy the texture's data into the pre allocated texture
	// then clean up the temp texture object
	OPmemcpy(sheet, temp, sizeof(OPtexture));
#ifdef _DEBUG
	OPlog("Copied SpriteSheet!");
#endif
	OPfree(temp);

#ifdef _DEBUG
	OPlog("Scale the frames...");
#endif
	__opSpriteScaleFrames(sheet, *ss);

#ifdef _DEBUG
	OPlog("Done!");
#endif

	OPfree(filenameWithoutExtension);


	return 1;
}

OPint OPspriteSheetUnload(void* ss){
	OPspriteSheet* sheet = (OPspriteSheet*)ss;
	OPint i = sheet->Sprites;

	// delete all frames for this sheet
	OPfree(((OPsprite*)OPcmanGet(sheet->Names[i-1]))->Frames);

	// free all the sprites
	for(;i--;){
		OPsprite* s = (OPsprite*)OPcmanGet(sheet->Names[i]);
		OPfree(s);
	}

	// free the names list
	OPfree(sheet->Names);

	// clean up the texture and sprite sheet object
	//OPtextureDestroy(sheet->Sheet);

	OPfree(ss);

	return 1;
}

OPvec2 OPspriteCurrentFrameSize(OPsprite* sprite) {
	f32 sheetWidth = sprite->Sheet->Description.Width;
	f32 sheetHeight = sprite->Sheet->Description.Height;
	f32 frameWidth = (sprite->Frames[sprite->Frame].Size.x * sheetWidth);
	f32 frameHeight = (sprite->Frames[sprite->Frame].Size.y * sheetHeight);
	return OPvec2Create(frameWidth, frameHeight);
}
#include "./Human/include/Rendering/Sprite/OPspriteSheet.h"
#include "./Human/include/Rendering/OPimage.h"
#include "./Human/include/Utilities/OPimagePNG.h"
#include "./Data/include/OPfile.h"
#include "./Data/include/OPcman.h"
#include "./Data/include/OPstring.h"
#include "./Core/include/Assert.h"

void __opSpriteScaleFrames(OPspriteSheet* ss) {
	ASSERT(ss->Texture, "__opSpriteScaleFrames() - texture null");
	ASSERT(ss, "__opSpriteScaleFrames() - spritesheet null");

	OPvec2 size = OPvec2(
        (f32)ss->Texture->textureDesc.width,
        (f32)ss->Texture->textureDesc.height);

	for (OPint i = ss->SpriteCount; i--;){
		OPsprite* s = (OPsprite*)OPCMAN.Get(ss->Names[i]);
		OPint j = s->FrameCount;
		for (; j--;){
			s->Frames[j].Offset.x /= size.x;
			s->Frames[j].Offset.y /= size.y;
			s->Frames[j].Size.x /= size.x;
			s->Frames[j].Size.y /= size.y;
		}
	}
}

OPint OPspriteSheetLoad(OPstream* str, OPspriteSheet** ss){
	//OPstream* str = OPreadFileLarge(filename, 1024);
	i32 width, height;
	i32 sprites, frames;
	// OPspriteFrame* frameData;
	//OPtexture *sheet;

	ui32 filenameLength = (ui32)strlen(str->Source);
	ui32 filenameLengthWithoutExtension = filenameLength - 5 - 8 + 1;

	OPchar* filenameWithoutExtension = (OPchar*)OPalloc(sizeof(OPchar)* filenameLengthWithoutExtension);
	OPmemcpy(filenameWithoutExtension, str->Source + 8, filenameLengthWithoutExtension * sizeof(OPchar));
	filenameWithoutExtension[filenameLengthWithoutExtension - 1] = '\0';

	// allocate space for the texture before it's created
	// this will allow us to refer to its location without
	// yet having loaded it.
	//sheet = (OPtexture*)OPalloc(sizeof(OPtexture));

	// read the dimensions of the image
	width = str->I32();
	height = str->I32();

#ifdef _DEBUG
	OPlog("Sprite sheet %dx%d", width, height);
#endif

	// read the sprite, and frame counts
	sprites = str->I32();
	frames = str->I32();

#ifdef _DEBUG
	OPlog("Sprites: %d, Frames: %d", sprites, frames);
#endif

	// allocate memory for the sprite sheet struct
	*ss = OPNEW(OPspriteSheet());
	(*ss)->Names = OPALLOC(OPchar*, sprites);
	(*ss)->SpriteCount = sprites;
	(*ss)->Sprites = OPALLOC(OPsprite*, sprites);

	// allocate contiguious frame memory segment
	(*ss)->frameData = OPALLOC(OPspriteFrame, frames);

	{ // Load all sprites and frames
		OPint frameNum = 0;
		for (OPint i = sprites; i--;){
			// read the name, and the sprite flags
			// setup a proxy var for frame data

			OPchar* nameData = str->String();
			ui32 nameDataLength = (ui32)strlen(nameData);

			// OPchar* finalName = (OPchar*)OPalloc(filenameLengthWithoutExtension + 1 + nameDataLength);
			// OPmemcpy(finalName, filenameWithoutExtension, filenameLengthWithoutExtension);
			// finalName[filenameLengthWithoutExtension] = '/';
			// OPmemcpy(finalName + filenameLengthWithoutExtension + 1, name, nameDataLength);
			// finalName[filenameLengthWithoutExtension + 1 + nameDataLength] = NULL;

			OPstring* strWithName = OPstring::Create(filenameWithoutExtension);
			strWithName->Add("/");
			strWithName->Add(nameData);
			OPfree(nameData);

			(*ss)->Names[i] = strWithName->C_Str();
			OPfree(strWithName);


			ui32 flags = str->I32();
			ui32 spriteFrames = 1;
			OPspriteFrame* spriteFrameData = &(*ss)->frameData[frameNum];
			OPsprite* sprite = OPNEW(OPsprite);
			(*ss)->Sprites[i] = sprite;

			// zero the sprite structure out
			OPbzero(sprite, sizeof(OPsprite));

#ifdef _DEBUG
			OPlog("Sprite '%s' @ %x", (*ss)->Names[i], sprite);
#endif

			if (flags){
				// this sprite has animation frames
				spriteFrames = str->I32();
			}

#ifdef _DEBUG
			OPlog("\tLoading...");
#endif

			// Load all the frames
			for (OPint j = 0; j < spriteFrames; j++){
				// read the sprite dimensions, scale them
				// to fit within UV space
				OPvec2 offset;
				offset.x = (OPfloat)str->I32();
				offset.y = (OPfloat)str->I32();
				OPvec2 size;
				size.x = (OPfloat)str->I32();
				size.y = (OPfloat)str->I32();

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
			sprite->SpriteSheet = (*ss);

			// increment the frame number for the next sprite
			frameNum += spriteFrames;

			// Insert the sprite into the content manager's hashmap
#ifdef _DEBUG
			OPlog("Inserting sprite '%s'", (*ss)->Names[i]);
#endif

			OPCMAN.Add((*ss)->Names[i], sprite, NULL, NULL);

#ifdef _DEBUG
			if(OPCMAN.IsLoaded((*ss)->Names[i])) {
				OPlog("OPSS %s loaded", (*ss)->Names[i]);
			} else {
				OPlog("OPSS FAILED TO LOAD %s", (*ss)->Names[i]);
			}
#endif
		}
	}

	// load the png image data
	OPimagePNGLoadStream(str, str->_pointer, &(*ss)->Texture, OPtextureFilter::NEAREST);
#ifdef _DEBUG
	OPlog("Loaded PNG!");
#endif

	// copy the texture's data into the pre allocated texture
	// then clean up the temp texture object
	// OPmemcpy(sheet, temp, sizeof(OPtexture));
#ifdef _DEBUG
	OPlog("Copied SpriteSheet!");
#endif
	//OPfree(temp);

#ifdef _DEBUG
	OPlog("Scale the frames...");
#endif
	__opSpriteScaleFrames(*ss);

#ifdef _DEBUG
	OPlog("Done!");
#endif

	OPfree(filenameWithoutExtension);


	return 1;
}

OPint OPspriteSheetUnload(void* ss){
	OPspriteSheet* sheet = (OPspriteSheet*)ss;

	// free all the sprites
	for (OPint i = 0; i < sheet->SpriteCount; i++) {
		OPfree(sheet->Sprites[i]);
	}

	OPfree(sheet->Sprites);
	OPfree(sheet->frameData);

	// free the names list
	for (OPint i = 0; i < sheet->SpriteCount; i++) {
		OPfree(sheet->Names[i]);
	}
	OPfree(sheet->Names);

	// clean up the texture and sprite sheet object
	sheet->Texture->Destroy(); 
#ifdef _DEBUG
	OPfree(sheet->Texture->source);
#endif
	OPfree(sheet->Texture);

	OPfree(ss);

	return 1;
}

OPassetLoader OPASSETLOADER_OPSS = {
	".opss",
	"Sprites/",
	sizeof(OPspriteSheet),
	(OPint(*)(OPstream*, void**))OPspriteSheetLoad,
	(OPint(*)(void*))OPspriteSheetUnload,
	NULL
};

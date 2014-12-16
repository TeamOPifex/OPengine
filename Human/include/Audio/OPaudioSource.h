#ifndef OPENGINE_HUMAN_AUDIO_SOURCE
#define OPENGINE_HUMAN_AUDIO_SOURCE
#include "OPaudio.h"


#define WAVE_HEADER 44
//-----------------------------------------------------------------------------
//   _____ _                   _       
//  / ____| |                 | |      
// | (___ | |_ _ __ _   _  ___| |_ ___ 
//  \___ \| __| '__| | | |/ __| __/ __|
//  ____) | |_| |  | |_| | (__| |_\__ \
// |_____/ \__|_|   \__,_|\___|\__|___/

typedef struct{
	ui64  Length;
	ui32 SamplesPerSecond;
	ui16 BitsPerSample;
	ui16 Channels;
	ui16  Format;
}OPaudioDescription;

struct sOPaudioSource;
struct sOPaudioSource{
	// Reads the next 'len' bytes from the data source and advances
	// position by that number of bytes. Fills dest with bytes read.
	OPint (*Read)(struct sOPaudioSource* src, ui64* position, ui8* dest, ui32 len);

	// Sets position to 'pos' makes appropriate changes for data source
	OPint (*Seek)(struct sOPaudioSource* src, ui64* pos);

	OPaudioDescription Description;
	void* DataSource;
};
typedef struct sOPaudioSource OPaudioSource;
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
//   ____                   _               ______                      
//  / __ \                 (_)             |  ____|                     
// | |  | |_ __   ___ _ __  _ _ __   __ _  | |__ _   _ _ __   ___ ___   
// | |  | | '_ \ / _ \ '_ \| | '_ \ / _` | |  __| | | | '_ \ / __/ __|  
// | |__| | |_) |  __/ | | | | | | | (_| | | |  | |_| | | | | (__\__ \_ 
//  \____/| .__/ \___|_| |_|_|_| |_|\__, | |_|   \__,_|_| |_|\___|___(_)
//        | |                        __/ |                              
//        |_|                       |___/                               
OPint OPaudOpenWave(const OPchar* filename, OPaudioSource** source);
OPint OPaudOpenOgg (const OPchar* filename, OPaudioSource** source);
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
//   _____ _           _               ______                      
//  / ____| |         (_)             |  ____|                     
// | |    | | ___  ___ _ _ __   __ _  | |__ _   _ _ __   ___ ___   
// | |    | |/ _ \/ __| | '_ \ / _` | |  __| | | | '_ \ / __/ __|  
// | |____| | (_) \__ \ | | | | (_| | | |  | |_| | | | | (__\__ \_ 
//  \_____|_|\___/|___/_|_| |_|\__, | |_|   \__,_|_| |_|\___|___(_)
//                              __/ |                              
//                             |___/                               
OPint OPaudCloseWave(OPaudioSource* src);
OPint OPaudCloseOgg (OPaudioSource* src);
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
//  _____                _ _               ______                _   _                 
// |  __ \              | (_)             |  ____|              | | (_)                
// | |__) |___  __ _  __| |_ _ __   __ _  | |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
// |  _  // _ \/ _` |/ _` | | '_ \ / _` | |  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
// | | \ \  __/ (_| | (_| | | | | | (_| | | |  | |_| | | | | (__| |_| | (_) | | | \__ \
// |_|  \_\___|\__,_|\__,_|_|_| |_|\__, | |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
//                                  __/ |                                              
//                                 |___/                                               
OPint OPaudReadWave(OPaudioSource* src, ui64* position, ui8* dest, ui32 len);
OPint OPaudReadOgg (OPaudioSource* src, ui64* position, ui8* dest, ui32 len);
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
//   _____           _    _               ______                _   _                 
//  / ____|         | |  (_)             |  ____|              | | (_)                
// | (___   ___  ___| | ___ _ __   __ _  | |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
//  \___ \ / _ \/ _ \ |/ / | '_ \ / _` | |  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//  ____) |  __/  __/   <| | | | | (_| | | |  | |_| | | | | (__| |_| | (_) | | | \__ \
// |_____/ \___|\___|_|\_\_|_| |_|\__, | |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
//                                 __/ |                                              
//                                |___/                                               
OPint OPaudSeekWave(OPaudioSource* src, ui64* pos);
OPint OPaudSeekOgg (OPaudioSource* src, ui64* pos);
//-----------------------------------------------------------------------------

#endif
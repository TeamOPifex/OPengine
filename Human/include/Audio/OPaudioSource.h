#pragma once

#include "OPaudio.h"
#include "./Core/include/OPtypes.h"

#define WAVE_HEADER 44

struct OPaudioDescription {
	ui64  Length;
	ui32 SamplesPerSecond;
	ui16 BitsPerSample;
	ui16 Channels;
	ui16  Format;
};
typedef struct OPaudioDescription OPaudioDescription;

struct sOPaudioSource {
	// Reads the next 'len' bytes from the data source and advances
	// position by that number of bytes. Fills dest with bytes read.
	OPint (*Read)(struct sOPaudioSource* src, ui64* position, ui8* dest, ui32 len);

	// Sets position to 'pos' makes appropriate changes for data source
	OPint (*Seek)(struct sOPaudioSource* src, ui64* pos);

	OPaudioDescription Description;
	void* DataSource;
};
typedef struct sOPaudioSource OPaudioSource;

OPint OPaudOpenWave(const OPchar* filename, OPaudioSource** source);
OPint OPaudOpenOgg (const OPchar* filename, OPaudioSource** source);

OPint OPaudCloseWave(OPaudioSource* src);
OPint OPaudCloseOgg (OPaudioSource* src);

OPint OPaudReadWave(OPaudioSource* src, ui64* position, ui8* dest, ui32 len);
OPint OPaudReadOgg (OPaudioSource* src, ui64* position, ui8* dest, ui32 len);

OPint OPaudSeekWave(OPaudioSource* src, ui64* pos);
OPint OPaudSeekOgg (OPaudioSource* src, ui64* pos);

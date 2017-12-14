#pragma once

struct OPrendererType {
	enum Enum {
		OPENGL,
		DIRECTX
	};
};

// TODO: (garrett) move to its own file
struct OPframeBufferMode {
	enum Enum {
		BOTH,
		READ,
		DRAW
	};
};
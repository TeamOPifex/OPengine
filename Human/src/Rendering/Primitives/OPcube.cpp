#include "./Human/include/Rendering/Primitives/OPcube.h"
#include "./Human/include/Rendering/Primitives/OPvertex.h"
#include "./Human/include/Rendering/OPmeshBuilder.h"

OPmesh OPcubeCreate(OPvec3 color) {
	OPmeshBuilder builder;
	ui16 vertexSize = sizeof(OPvertexColor);
	OPvertexColor verts[4];

	for(OPint i = 0; i < 4; i++) {
		verts[i].Color = color;
	}

	OPvertexLayoutBuilder vertexLayoutBuilder;
	vertexLayoutBuilder.Init()->Add(OPattributes::POSITION)->Add(OPattributes::COLOR);
	builder.Init(vertexLayoutBuilder.Build());

	{ // Back
		verts[0].Position = OPvec3Create(0.5, 0.5, -0.5);
		verts[1].Position = OPvec3Create(-0.5, 0.5, -0.5);
		verts[2].Position = OPvec3Create(-0.5, -0.5, -0.5);
		verts[3].Position = OPvec3Create(0.5, -0.5, -0.5);
		builder.Add(&verts[0], &verts[1], &verts[2], &verts[3]);
	}

	{ // Front
		verts[0].Position = OPvec3Create(-0.5, 0.5, 0.5);
		verts[1].Position = OPvec3Create(0.5, 0.5, 0.5);
		verts[2].Position = OPvec3Create(0.5, -0.5, 0.5);
		verts[3].Position = OPvec3Create(-0.5, -0.5, 0.5);
		builder.Add(&verts[0], &verts[1], &verts[2], &verts[3]);
	}

	{ // Top
		verts[0].Position = OPvec3Create(-0.5, 0.5, 0.5);
		verts[1].Position = OPvec3Create(0.5, 0.5, 0.5);
		verts[2].Position = OPvec3Create(0.5, 0.5, -0.5);
		verts[3].Position = OPvec3Create(-0.5, 0.5, -0.5);
		builder.Add(&verts[0], &verts[1], &verts[2], &verts[3]);
	}

	{ // Bottom
		verts[0].Position = OPvec3Create(0.5, -0.5, -0.5);
		verts[1].Position = OPvec3Create(-0.5, -0.5, -0.5);
		verts[2].Position = OPvec3Create(-0.5, -0.5, 0.5);
		verts[3].Position = OPvec3Create(0.5, -0.5, 0.5);
		builder.Add(&verts[0], &verts[1], &verts[2], &verts[3]);
	}

	{ // Left
		verts[0].Position = OPvec3Create(-0.5, 0.5, -0.5);
		verts[1].Position = OPvec3Create(-0.5, 0.5, 0.5);
		verts[2].Position = OPvec3Create(-0.5, -0.5, 0.5);
		verts[3].Position = OPvec3Create(-0.5, -0.5, -0.5);
		builder.Add(&verts[0], &verts[1], &verts[2], &verts[3]);
	}

	{ // Right
		verts[0].Position = OPvec3Create(0.5, 0.5, 0.5);
		verts[1].Position = OPvec3Create(0.5, 0.5, -0.5);
		verts[2].Position = OPvec3Create(0.5, -0.5, -0.5);
		verts[3].Position = OPvec3Create(0.5, -0.5, 0.5);
		builder.Add(&verts[0], &verts[1], &verts[2], &verts[3]);
	}
	
	OPmesh result = builder.Build();
	builder.Destroy();

	return result;
}
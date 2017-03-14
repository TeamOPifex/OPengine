#include "./Human/include/Rendering/Primitives/OPcube.h"
#include "./Human/include/Rendering/Primitives/OPvertex.h"
#include "./Human/include/Rendering/OPmeshBuilder.h"

OPmodel* OPcubeCreate(OPvec3 color) {
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
	
	OPmodel* result = builder.Build();
	builder.Destroy();

	return result;
}

struct OPvertexTexNormal {
	OPvec3 Position;
	OPvec3 Normal;
	OPvec3 Tangent;
	OPvec3 BiTangent;
	OPvec2 TexCoord;
};

OPmodel* OPcubeCreate(OPvec3 offset, OPvec3 scale) {
	OPmeshBuilder builder;
	ui16 vertexSize = 0;

	OPvertexTexNormal verts[4];
	verts[0].TexCoord = OPvec2(0, 0);
	verts[1].TexCoord = OPvec2(1, 0);
	verts[2].TexCoord = OPvec2(1, 1);
	verts[3].TexCoord = OPvec2(0, 1);


	OPvertexLayoutBuilder vertexLayoutBuilder;
	vertexLayoutBuilder.Init()
		->Add(OPattributes::POSITION)
		->Add(OPattributes::NORMAL)
		->Add(OPattributes::TANGENT)
		->Add(OPattributes::BITANGENT)
		->Add(OPattributes::UV);
	builder.Init(vertexLayoutBuilder.Build());

	{ // Back
		verts[2].Position = (OPvec3Create(0.5, 0.5, -0.5) + offset) * scale;
		verts[1].Position = (OPvec3Create(-0.5, 0.5, -0.5) + offset) * scale;
		verts[0].Position = (OPvec3Create(-0.5, -0.5, -0.5) + offset) * scale;
		verts[3].Position = (OPvec3Create(0.5, -0.5, -0.5) + offset) * scale;
		verts[0].Normal = verts[1].Normal = verts[2].Normal = verts[3].Normal = OPvec3Create(0, 0, -1);
		builder.Add(&verts[0], &verts[1], &verts[2], &verts[3]);
	}

	{ // Front
		verts[2].Position = (OPvec3Create(-0.5, 0.5, 0.5) + offset) * scale;
		verts[1].Position = (OPvec3Create(0.5, 0.5, 0.5) + offset) * scale;
		verts[0].Position = (OPvec3Create(0.5, -0.5, 0.5) + offset) * scale;
		verts[3].Position = (OPvec3Create(-0.5, -0.5, 0.5) + offset) * scale;
		verts[0].Normal = verts[1].Normal = verts[2].Normal = verts[3].Normal = OPvec3Create(0, 0, 1);
		builder.Add(&verts[0], &verts[1], &verts[2], &verts[3]);
	}

	{ // Top
		verts[0].Position = (OPvec3Create(-0.5, 0.5, 0.5) + offset) * scale;
		verts[1].Position = (OPvec3Create(0.5, 0.5, 0.5) + offset) * scale;
		verts[2].Position = (OPvec3Create(0.5, 0.5, -0.5) + offset) * scale;
		verts[3].Position = (OPvec3Create(-0.5, 0.5, -0.5) + offset) * scale;
		verts[0].Normal = verts[1].Normal = verts[2].Normal = verts[3].Normal = OPvec3Create(0, 1, 0);
		builder.Add(&verts[0], &verts[1], &verts[2], &verts[3]);
	}

	{ // Bottom
		verts[2].Position = (OPvec3Create(0.5, -0.5, -0.5) + offset) * scale;
		verts[1].Position = (OPvec3Create(-0.5, -0.5, -0.5) + offset) * scale;
		verts[0].Position = (OPvec3Create(-0.5, -0.5, 0.5) + offset) * scale;
		verts[3].Position = (OPvec3Create(0.5, -0.5, 0.5) + offset) * scale;
		verts[0].Normal = verts[1].Normal = verts[2].Normal = verts[3].Normal = OPvec3Create(0, -1, 0);
		builder.Add(&verts[0], &verts[1], &verts[2], &verts[3]);
	}

	{ // Left
		verts[2].Position = (OPvec3Create(-0.5, 0.5, -0.5) + offset) * scale;
		verts[1].Position = (OPvec3Create(-0.5, 0.5, 0.5) + offset) * scale;
		verts[0].Position = (OPvec3Create(-0.5, -0.5, 0.5) + offset) * scale;
		verts[3].Position = (OPvec3Create(-0.5, -0.5, -0.5) + offset) * scale;
		verts[0].Normal = verts[1].Normal = verts[2].Normal = verts[3].Normal = OPvec3Create(-1, 0, 0);
		builder.Add(&verts[0], &verts[1], &verts[2], &verts[3]);
	}

	{ // Right
		verts[2].Position = (OPvec3Create(0.5, 0.5, 0.5) + offset) * scale;
		verts[1].Position = (OPvec3Create(0.5, 0.5, -0.5) + offset) * scale;
		verts[0].Position = (OPvec3Create(0.5, -0.5, -0.5) + offset) * scale;
		verts[3].Position = (OPvec3Create(0.5, -0.5, 0.5) + offset) * scale;
		verts[0].Normal = verts[1].Normal = verts[2].Normal = verts[3].Normal = OPvec3Create(1, 0, 0);
		builder.Add(&verts[0], &verts[1], &verts[2], &verts[3]);
	}
	
	OPmodel* result = builder.Build();
	builder.Destroy();

	return result;
}
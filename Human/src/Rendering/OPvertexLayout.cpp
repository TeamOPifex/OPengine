#include "./Human/include/Rendering/OPvertexLayout.h"
#include "./Human/include/Rendering/OPeffect.h"

void OPvertexLayout::Init(ui16 attributeCount, OPchar** names, OPattributeTypes* types, ui8* counts) {

	count = attributeCount;
	attributes = (OPshaderAttribute*)OPalloc(sizeof(OPshaderAttribute) * count);

	ui32 offset = 0;
	stride = 0;
	ui32 attributeSize;
	for (ui16 i = 0; i < count; i++) {
		attributes[i].Name = names[i];
		attributes[i].Type = GL_FLOAT;// types[i];
		attributes[i].Elements = counts[i];
		attributes[i].Offset = offset;
		attributes[i].Location = i;
		attributeSize = counts[i] * sizeof(f32);
		offset += attributeSize;
		stride += attributeSize;
	}
}

void OPvertexLayout::Init(OPshaderAttribute* attributes, ui16 count) {
	count = count;
	attributes = attributes;

	stride = 0;
	for (ui16 i = 0; i < count; i++) {
		stride += attributes[i].Elements * sizeof(f32);
	}
}

void OPvertexLayout::SetOffsets(OPeffect* effect) {
	for (OPuint i = 0; i < count; i++) {
		OPRENDERER_ACTIVE->ShaderAttribute.SetOffset(&attributes[i], effect);
	}
}

void OPvertexLayoutBuilder::Init() {
	index = 0;
}

void OPvertexLayoutBuilder::Init(ui32 features) {
	if ((features & (OPuint)OPattributes::POSITION) > 0) Add(OPattributes::POSITION);
	if((features & (OPuint)OPattributes::NORMAL) > 0) Add(OPattributes::NORMAL);
	if((features & (OPuint)OPattributes::TANGENT) > 0) Add(OPattributes::TANGENT);
	if((features & (OPuint)OPattributes::UV) > 0) Add(OPattributes::UV);
	if((features & (OPuint)OPattributes::BONES) > 0) Add(OPattributes::BONES);
	if((features & (OPuint)OPattributes::COLOR) > 0) Add(OPattributes::COLOR);
}

void OPvertexLayoutBuilder::Add(OPattributes attribute) {
	switch (attribute) {
	case OPattributes::POSITION: {
			names[index] = "aPosition";
			types[index] = OPATTR_TYPE_FLOAT;
			counts[index] = 3;
			index++;
			break;
		}
		case OPattributes::NORMAL: {
			names[index] = "aNormal";
			types[index] = OPATTR_TYPE_FLOAT;
			counts[index] = 3;
			index++;
			break;
		}
		case OPattributes::TANGENT: {
			names[index] = "aTangent";
			types[index] = OPATTR_TYPE_FLOAT;
			counts[index] = 3;
			index++;
			break;
		}
		case OPattributes::COLOR: {
			names[index] = "aColor";
			types[index] = OPATTR_TYPE_FLOAT;
			counts[index] = 3;
			index++;
			break;
		}
		case OPattributes::UV: {
			names[index] = "aUV";
			types[index] = OPATTR_TYPE_FLOAT;
			counts[index] = 2;
			index++;
			break;
		}
		case OPattributes::BONES: {
			names[index] = "aBones";
			types[index] = OPATTR_TYPE_FLOAT;
			counts[index] = 4;
			index++;
			names[index] = "aWeights";
			types[index] = OPATTR_TYPE_FLOAT;
			counts[index] = 4;
			index++;
			break;
		}
	}
}

OPvertexLayout OPvertexLayoutBuilder::Build() {
	OPvertexLayout result;
	result.Init(index, names, types, counts);
	return result;
}
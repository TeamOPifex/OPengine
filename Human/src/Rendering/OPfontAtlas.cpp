#pragma once

#include "./Human/include/Rendering/OPfontAtlas.h"
#include "./Human/include/Resources/Texture/ImagePNG.h"

OPfontAtlas* OPfontAtlasCreate(OPint width, OPint height, OPint depth) {
	OPfontAtlas* self = (OPfontAtlas*)OPalloc(sizeof(OPfontAtlas));
	OPfontAtlasNode node = { 1, 1, width - 2 };
	self->nodes = OPvectorCreate(sizeof(OPfontAtlasNode));
	self->used = 0;
	self->width = width;
	self->height = height;
	self->depth = depth;
	OPvectorPush(self->nodes, (ui8*)&node);
	self->data = (ui8*)OPalloc(width*height*depth*sizeof(ui8));
	OPbzero(self->data, width*height*depth*sizeof(ui8));
	return self;
}

void OPfontAtlasDestroy(OPfontAtlas* atlas){
	OPvectorDestroy(atlas->nodes);
	if (atlas->data){
		OPfree(atlas->data);
	}
	OPfree(atlas);
}

void OPfontAtlasSetRegion(OPfontAtlas* atlas, OPint x, OPint y, OPint width, OPint height, ui8* data, OPint stride) {
	OPint i, depth, charsize;

	depth = atlas->depth;
	charsize = sizeof(ui8);
	for (i = 0; i < height; ++i){
		OPmemcpy(atlas->data + ((y + i)*atlas->width + x) * charsize * depth,
			data + (i * stride) * charsize, width * charsize * depth);
	}
}

OPint OPfontAtlasFit(OPfontAtlas* atlas, OPint index, OPint width, OPint height){
	OPfontAtlasNode* node;
	OPint x, y, width_left, i;

	node = (OPfontAtlasNode*)(OPvectorGet(atlas->nodes, index));
	x = node->x;
	y = node->y;
	width_left = width;
	i = index;

	if ((x + width) > (atlas->width - 1)) {
		return -1;
	}

	y = node->y;

	while (width_left > 0) {
		node = (OPfontAtlasNode*)(OPvectorGet(atlas->nodes, i));
		if (node->y > y){
			y = node->y;
		}
		if ((y + height) > atlas->height - 1){
			return -1;
		}
		width_left -= node->z;
		++i;
	}
	return y;
}

void OPfontAtlasMerge(OPfontAtlas* atlas)
{
	OPfontAtlasNode *node, *next;
	OPint i;
	
	for (i = 0; i< atlas->nodes->_size - 1; ++i)
	{
		node = (OPfontAtlasNode*)(OPvectorGet(atlas->nodes, i));
		next = (OPfontAtlasNode*)(OPvectorGet(atlas->nodes, i + 1));
		if (node->y == next->y)
		{
			node->z += next->z;
			OPvectorErase(atlas->nodes, i + 1);
			--i;
		}
	}
}

OPfontAtlasRegion OPfontAtlasGetRegion(OPfontAtlas* atlas, OPint width, OPint height)
{
	OPint y, best_height, best_width, best_index;
	OPfontAtlasNode *node, *prev;
	OPfontAtlasRegion region = { 0, 0, width, height };
	OPint i;

	best_height = INT_MAX;
	best_index = -1;
	best_width = INT_MAX;

	for (i = 0; i<atlas->nodes->_size; ++i)
	{
		y = OPfontAtlasFit(atlas, i, width, height);
		if (y >= 0)
		{
			node = (OPfontAtlasNode*)OPvectorGet(atlas->nodes, i);
			if (((y + height) < best_height) ||
				(((y + height) == best_height) && (node->z < best_width)))
			{
				best_height = y + height;
				best_index = i;
				best_width = node->z;
				region.x = node->x;
				region.y = y;
			}
		}
	}

	if (best_index == -1)
	{
		region.x = -1;
		region.y = -1;
		region.width = 0;
		region.height = 0;
		return region;
	}

	node = (OPfontAtlasNode *)OPalloc(sizeof(OPfontAtlasNode));
	if (node == NULL)
	{
		exit(EXIT_FAILURE);
	}

	node->x = region.x;
	node->y = region.y + height;
	node->z = width;
	OPvectorInsert(atlas->nodes, best_index, (ui8*)node);
	OPfree(node);

	for (i = best_index + 1; i < atlas->nodes->_size; ++i)
	{
		node = (OPfontAtlasNode*)OPvectorGet(atlas->nodes, i);
		prev = (OPfontAtlasNode*)OPvectorGet(atlas->nodes, i - 1);

		if (node->x < (prev->x + prev->z))
		{
			int shrink = prev->x + prev->z - node->x;
			node->x += shrink;
			node->z -= shrink;
			if (node->z <= 0)
			{
				OPvectorErase(atlas->nodes, i);
				--i;
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}
	}
	OPfontAtlasMerge(atlas);
	atlas->used += width * height;
	return region;
}

OPtexture OPfontAtlasTexture(OPfontAtlas* atlas) {
	OPtextureDescription desc = { 
		(ui16)atlas->width,
		(ui16)atlas->height,
		GL_RED,
		GL_RED,
		GL_UNSIGNED_BYTE,
		GL_LINEAR, 
		GL_LINEAR, 
		GL_CLAMP_TO_EDGE, 
		GL_CLAMP_TO_EDGE		
	};
	if (atlas->depth == 4) {
		desc.InternalFormat = GL_RGBA;
		desc.Format = GL_RGBA;
		desc.DataType = GL_UNSIGNED_BYTE;
	}
	if (atlas->depth == 3) {
		desc.InternalFormat = GL_RGB;
		desc.Format = GL_RGB;
		desc.DataType = GL_UNSIGNED_BYTE;
	}
	OPtexture texture = OPtextureCreate(desc);
	OPtextureBind(&texture);
	OPtextureSetData(atlas->data);
	return texture;
}

void OPfontAtlasSavePNG(OPfontAtlas* atlas, i8* filename) {
	if (atlas->depth == 1) {
		ui8* data = (ui8*)OPalloc(atlas->width * atlas->height * 3);
		for (OPint i = atlas->width * atlas->height; i--;) {
			data[i * 3] = atlas->data[i];
			data[i * 3 + 1] = atlas->data[i];
			data[i * 3 + 2] = atlas->data[i];
		}
		OPimagePNGCreate24(data, atlas->width, atlas->height, filename);
	} else if (atlas->depth == 3) {
		OPimagePNGCreate24(atlas->data, atlas->width, atlas->height, filename);
	}
	else if (atlas->depth == 4){
		OPimagePNGCreate32(atlas->data, atlas->width, atlas->height, filename);
	}
}
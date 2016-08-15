#include "./Human/include/Rendering/Font/OPfontAtlas.h"
#include "./Human/include/Rendering/OPimage.h"
#include "./Human/include/Utilities/OPimagePNG.h"

void OPfontAtlas::Init(i32 width, i32 height, OPint depth) {
	OPfontAtlasNode node = { 1, 1 };
	node.z = width - 2;
	nodes = OPvector::Create(sizeof(OPfontAtlasNode));
	used = 0;
	this->width = width;
	this->height = height;
	this->depth = depth;
	nodes->Push((ui8*)&node);
	data = (ui8*)OPalloc(width*height*depth*sizeof(ui8));
	OPbzero(data, width*height*depth*sizeof(ui8));
}

void OPfontAtlas::Destroy(){
	nodes->Destroy();
	OPfree(nodes);
	if (data){
		OPfree(data);
		data = NULL;
	}
}

void OPfontAtlas::SetRegion(OPint x, OPint y, OPint width, OPint height, ui8* data, OPint stride) {
	OPint i, depth, charsize;

	depth = this->depth;
	charsize = sizeof(ui8);
	for (i = 0; i < height; ++i){
		OPmemcpy(this->data + ((y + i)*this->width + x) * charsize * depth,
			data + (i * stride) * charsize, width * charsize * depth);
	}
}

OPint OPfontAtlas::Fit(OPint index, OPint width, OPint height){
	OPfontAtlasNode* node;
	OPint x, y, width_left, i;

	node = (OPfontAtlasNode*)(this->nodes->Get(index));
	x = node->x;
	y = node->y;
	width_left = width;
	i = index;

	if ((x + width) > (this->width - 1)) {
		return -1;
	}

	y = node->y;

	while (width_left > 0) {
		node = (OPfontAtlasNode*)(this->nodes->Get(i));
		if (node->y > y){
			y = node->y;
		}
		if ((y + height) > this->height - 1){
			return -1;
		}
		width_left -= node->z;
		++i;
	}
	return y;
}

void OPfontAtlas::Merge()
{
	OPfontAtlasNode *node, *next;
	OPint i;
	
	for (i = 0; i < (int)(this->nodes->_size) - 1; ++i)
	{
		node = (OPfontAtlasNode*)(this->nodes->Get(i));
		next = (OPfontAtlasNode*)(this->nodes->Get(i + 1));
		if (node->y == next->y)
		{
			node->z += next->z;
			this->nodes->Erase(i + 1);
			--i;
		}
	}
}

OPfontAtlasRegion OPfontAtlas::GetRegion(i32 width, i32 height)
{
	OPint y;
	OPint best_height, best_width, best_index;
	OPfontAtlasNode *node, *prev;
	OPfontAtlasRegion region = { 0, 0, width, height };
	OPuint i;
	OPint maxInt = 2147483647;

	best_height = maxInt;
	best_index = -1;
	best_width = maxInt;

	for (i = 0; i < this->nodes->_size; ++i)
	{
		y = Fit(i, width, height);
		if (y >= 0)
		{
			node = (OPfontAtlasNode*)this->nodes->Get(i);
			if (((y + height) < best_height) ||
				(((y + height) == best_height) && (node->z < best_width)))
			{
				best_height = y + height;
				best_index = i;
				best_width = node->z;
				region.x = node->x;
				region.y = (i32)y;
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
	this->nodes->Insert(best_index, (ui8*)node);
	OPfree(node);

	for (i = best_index + 1; i < this->nodes->_size; ++i)
	{
		node = (OPfontAtlasNode*)this->nodes->Get(i);
		prev = (OPfontAtlasNode*)this->nodes->Get(i - 1);

		if (node->x < (prev->x + prev->z))
		{
			int shrink = prev->x + prev->z - node->x;
			node->x += shrink;
			node->z -= shrink;
			if (node->z <= 0)
			{
				this->nodes->Erase(i);
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
	Merge();
	this->used += width * height;
	return region;
}

OPtexture OPfontAtlas::Texture() {
	OPtextureDesc desc;
	desc.width = this->width;
	desc.height = this->height;
	desc.filter = OPtextureFilter::LINEAR;
	desc.wrap = OPtextureWrap::CLAMP_TO_EDGE;
	desc.format = OPtextureFormat::RGB;
	if (this->depth == 4) {
		desc.format = OPtextureFormat::RGBA;
	}
	OPtexture result;
	OPRENDERER_ACTIVE->Texture.Init(&result, desc, this->data);
	return result;
}

void OPfontAtlas::SavePNG(OPchar* filename) {
	if (this->depth == 1) {
		ui8* data = (ui8*)OPalloc(this->width * this->height * 3);
		for (OPint i = this->width * this->height; i--;) {
			data[i * 3] = this->data[i];
			data[i * 3 + 1] = this->data[i];
			data[i * 3 + 2] = this->data[i];
		}
		OPimagePNGCreate24(data, this->width, this->height, filename);
	} else if (this->depth == 3) {
		OPimagePNGCreate24(this->data, this->width, this->height, filename);
	}
	else if (this->depth == 4){
		OPimagePNGCreate32(this->data, this->width, this->height, filename);
	}
}
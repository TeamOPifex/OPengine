#include "./Human/include/Rendering/UI/OPimgui.h"

#include "./Human/include/Rendering/OPrender.h"
#include "./Human/include/Rendering/Font/OPfontRender.h"
#include "./Human/include/Rendering/Primitives/OPquad.h"
#include "./Human/include/Input/OPmouse.h"
#include "./Data/include/OPstring.h"

#include "./Human/include/Platform/opengl/OPcommonGL.h"

OPimgui* OPIMGUI_ACTIVE = NULL;

OPimgui* OPimguiCreate(OPeffect* effect, OPfontManager* fontManager) {
	OPimgui* imgui = (OPimgui*)OPallocZero(sizeof(OPimgui));
	imgui->effect = effect;
	imgui->fontManager = fontManager;


	imgui->buffer.Init();// = OPrenderGenBuffer(OPvertexBufferOLD);
	// OPlog("Render %d, %d", OPRENDER_WIDTH, OPRENDER_HEIGHT);
	// OPlog("Screen %d, %d", OPRENDER_SCREEN_WIDTH, OPRENDER_SCREEN_HEIGHT);
	// OPlog("Screen %f, %f", OPRENDER_SCREEN_WIDTH_SCALE, OPRENDER_SCREEN_HEIGHT_SCALE);

	imgui->proj = OPmat4Ortho(0, (OPfloat)OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width, (OPfloat)OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height, 0, -1, 1);

	return imgui;
}


void OPimguiBegin() {
	OPfontRenderBegin(OPIMGUI_ACTIVE->fontManager);
    //glBindVertexArray(imgui->vaos[0]);
	//glUseProgram(imgui->effect->ProgramHandle);
}

void OPimguiEnd() {
	OPfontRenderEnd();
    //glBindVertexArray(imgui->vaos[0]);
	//glUseProgram(imgui->effect->ProgramHandle);
}

// Render solid block
void _block(
	OPvec2 pos,
	OPvec2 size,
	OPvec4 color
	) {

	ASSERT(OPIMGUI_ACTIVE != NULL, "An IMGUI must be bound");

	OPfloat x1, x2, y1, y2;
	x1 = pos.x;
	y1 = pos.y;
	x2 = pos.x + size.x;
	y2 = pos.y + size.y;

	//x1 = x2 = y1 = y2 = 0;

	OPimguiVertex data[6];
	for(OPint i = 0; i < 6; i++) {
		data[i].Color = color;
	}
	data[0].Position = OPvec3Create(x1, y1, 0);
	data[1].Position = OPvec3Create(x2, y1, 0);
	data[2].Position = OPvec3Create(x2, y2, 0);
	data[3].Position = OPvec3Create(x1, y1, 0);
	data[4].Position = OPvec3Create(x2, y2, 0);
	data[5].Position = OPvec3Create(x1, y2, 0);
    // float v[42] = {
    //                 x1, y1, 0, color.x, color.y, color.z, color.w,
    //                 x2, y1, 0, color.x, color.y, color.z, color.w,
    //                 x2, y2, 0, color.x, color.y, color.z, color.w,
    //                 x1, y1, 0, color.x, color.y, color.z, color.w,
    //                 x2, y2, 0, color.x, color.y, color.z, color.w,
    //                 x1, y2, 0, color.x, color.y, color.z, color.w
    //               };

	//OPrenderSetBufferData(&OPIMGUI_ACTIVE->buffer, sizeof(OPimguiVertex), 6, data);
	OPIMGUI_ACTIVE->buffer.SetData(sizeof(OPimguiVertex), 6, data);
	OPIMGUI_ACTIVE->effect->Bind();
	OPIMGUI_ACTIVE->effect->Set("uProj", &OPIMGUI_ACTIVE->proj);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}


// Render solid block
void _radio(
	OPvec2 pos,
	OPvec2 size,
	OPvec4 color
	) {

	ASSERT(OPIMGUI_ACTIVE != NULL, "An IMGUI must be bound");

	OPvec2 center = pos;
	OPfloat halfWidth = size.x / 2.5f;
	OPfloat halfHeight = size.y / 2.5f;

	OPvec2 points[8];
	points[0] = OPvec2(pos.x - size.x, pos.y - halfHeight);
	points[1] = OPvec2(pos.x - size.x, pos.y + halfHeight);
	points[2] = OPvec2(pos.x - halfWidth, pos.y + size.y);
	points[3] = OPvec2(pos.x + halfWidth, pos.y + size.y);
	points[4] = OPvec2(pos.x + size.x, pos.y + halfHeight);
	points[5] = OPvec2(pos.x + size.x, pos.y - halfHeight);
	points[6] = OPvec2(pos.x + halfWidth, pos.y - size.y);
	points[7] = OPvec2(pos.x - halfWidth, pos.y - size.y);

	OPimguiVertex data[24];
	for(OPint i = 0; i < 8; i++) {
		for(OPint j = 0; j < 3; j++) {
			data[i * 3 + j].Color = color;
		}
		OPint p1 = i;
		OPint p2 = (i + 1) % 8;
		data[i * 3 + 0].Position = OPvec3Create(points[p1].x, points[p1].y, 0);
		data[i * 3 + 1].Position = OPvec3Create(points[p2].x, points[p2].y, 0);
		data[i * 3 + 2].Position = OPvec3Create(center, 0);
	}

	//OPrenderSetBufferData(&OPIMGUI_ACTIVE->buffer, sizeof(OPimguiVertex), 24, data);
	OPIMGUI_ACTIVE->buffer.SetData(sizeof(OPimguiVertex), 24, data);
	OPIMGUI_ACTIVE->effect->Bind();
	OPIMGUI_ACTIVE->effect->Set("uProj", &OPIMGUI_ACTIVE->proj);
	glDrawArrays(GL_TRIANGLES, 0, 24);
}


OPint OPimguiTextbox(OPvec2 pos, const OPchar* text, const OPchar* placeholder, OPint active, OPint showCursor) {

	OPchar buffer[1024];
	OPvec4 colorForFont = OPIMGUI_ACTIVE->primaryColor;
	OPint usePlaceholder = 0;

	if(strlen(text) == 0 && !active) {
		usePlaceholder = 1;
		colorForFont = OPIMGUI_ACTIVE->hoverColor;
		for(ui32 i = 0 ; i < strlen(placeholder) + 1; i++) {
			buffer[i] = placeholder[i];
		}
	} else {
		for(ui32 i = 0 ; i < strlen(text) + 1; i++) {
			buffer[i] = text[i];
		}
	}

	OPvec2 textSize = OPIMGUI_ACTIVE->fontManager->_font->GetSize(buffer, OPIMGUI_ACTIVE->fontManager->scale);

	OPvec2 size = textSize;
	size.x = 400;//OPIMGUI_ACTIVE->padding[0] + OPIMGUI_ACTIVE->padding[1];
	size.y += OPIMGUI_ACTIVE->padding[2] + OPIMGUI_ACTIVE->padding[3];

	OPvec2 topLeft = pos;
	OPvec2 bottomRight = topLeft + size;


	OPint mx = OPMOUSE.X(), my = OPMOUSE.Y();
	OPint mouseWithin = mx >= topLeft.x && my >= topLeft.y &&
		mx <= bottomRight.x && my <= bottomRight.y;

	OPint clicked = mouseWithin && OPMOUSE.WasPressed(OPmouseButton::LBUTTON);


	_block(pos, OPvec2(400, size.y), OPIMGUI_ACTIVE->secondaryColor);

	OPvec2 p = pos;
	p.x += OPIMGUI_ACTIVE->padding[0];
	p.y += OPIMGUI_ACTIVE->padding[2];

	OPint diff = (OPint)(400 - OPIMGUI_ACTIVE->padding[0] - OPIMGUI_ACTIVE->padding[0] - 25);
	if(textSize.x > diff) {
		p.x -= (textSize.x - diff);
	}

	//p.y = (OPRENDER_SCREEN_HEIGHT * OPRENDER_SCREEN_HEIGHT_SCALE) - p.y - size.y;

	OPint screenHeight = (OPint)(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->HeightScale);
	OPint x, y, z, w;
	x = (OPint)(pos.x / OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->HeightScale);
	y = (OPint)(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height - (pos.y + size.y) / OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->HeightScale);
	z = (OPint)(400 / OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->HeightScale);
	w = (OPint)(size.y / OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->HeightScale);
	//OPlog("%d, Scissor %d,%d %dx%d", screenHeight, x,y,z,w);

    glScissor((GLint)x, (GLint)y, (GLsizei)z, (GLsizei)w);
    glEnable(GL_SCISSOR_TEST);
    //OPrenderClear(1,0,0);
        // draw other stuff using window's scissor

	if(active && showCursor && !usePlaceholder) {
		OPint charPos = strlen(buffer);
		buffer[charPos] = '|';
		buffer[charPos + 1] = '\0';
	}

	OPfontColor(colorForFont);
	OPfontRender(buffer, p);

    glDisable(GL_SCISSOR_TEST);

	if(clicked) return 1;
	else if (OPMOUSE.WasPressed(OPmouseButton::LBUTTON)) return 2;
	else return 0;
}

// Render Radio
OPint OPimguiRadio(
	OPvec2 pos,
	OPvec2 size,
	OPvec4 color,
	OPvec4 selected
	) {

	ASSERT(OPIMGUI_ACTIVE != NULL, "An IMGUI must be bound");

	OPvec2 topLeft = pos;
	OPvec2 bottomRight = topLeft + size;

	size /= 2;

	// Reposition to center
	// Ensures that TopLeft corner is at pos given
	pos.x += size.x;
	pos.y += size.y;

	OPint mx = OPMOUSE.X(), my = OPMOUSE.Y();

	_radio(pos, size, color);

	if(mx >= topLeft.x && my >= topLeft.y &&
		mx <= bottomRight.x && my <= bottomRight.y)
	{
		OPvec2 inner = size;
		inner *= 0.6f;
		_radio(pos, inner, selected);
	}

	return 0;
}

	// Render checkbox
OPint OPimguiCheckbox(
	OPint state,
	OPvec2 pos,
	OPvec2 size,
	OPvec4 color,
	OPvec4 selected,
	OPvec4 hover
	) {
	ASSERT(OPIMGUI_ACTIVE != NULL, "An IMGUI must be bound");

	OPvec2 topLeft = pos;
	OPvec2 bottomRight = topLeft + size;

	_block(pos, size, color);

	OPint mx = OPMOUSE.X(), my = OPMOUSE.Y();

	OPvec2 inner = size;
	inner *= 0.6f;
	OPvec2 innerPos = pos;
	OPvec2 diff = size - inner;
	diff /= 2;
	innerPos += diff;


	OPint mouseWithin = mx >= topLeft.x && my >= topLeft.y &&
		mx <= bottomRight.x && my <= bottomRight.y;

	if(state) {
		_block(innerPos, inner, selected);
	} else if(mouseWithin) {
		_block(innerPos, inner, hover);
	}

	if(mouseWithin && OPMOUSE.WasPressed(OPmouseButton::LBUTTON)) {
		return 1;
	}

	return 0;
}

	// Render Label
void OPimguiLabel(OPvec2 pos, const OPchar* text) {
	ASSERT(OPIMGUI_ACTIVE != NULL, "An IMGUI must be bound");
	// Determine label size
	//	OPvec2 textSize = OPfontGetSize(OPIMGUI_ACTIVE->fontManager->_font, text);

	// OPvec2 size = textSize;
	// size += 6;
	// _block(pos, size, OPIMGUI_ACTIVE->secondaryColor);

	OPvec2 paddedPosition = pos;
	paddedPosition += 3;

	OPvec4Log("Font Color", OPIMGUI_ACTIVE->primaryColor);
	OPfontColor(OPIMGUI_ACTIVE->primaryColor);
	OPfontRender(text, paddedPosition);
}

	// Render Label
void OPimguiLabel(
	OPvec2 pos,
	OPvec2 size,
	const OPchar* text,
	OPvec4 bg,
	OPvec4 color,
	OPint fill) {

	ASSERT(OPIMGUI_ACTIVE != NULL, "An IMGUI must be bound");

	if(bg.w > 0) {
		// Determine label size
		OPvec2 s = OPIMGUI_ACTIVE->fontManager->_font->GetSize(text, OPIMGUI_ACTIVE->fontManager->scale);

		OPvec2 _size = s;
		_size.x += 6;
		_size.y += 6;

		// Block
		_block(pos, _size, bg);
	}

	OPvec2 p = pos;
	p.x += 3;
	p.y += 3;
	// Label
	OPvec4Log("Font Color", color);
	OPfontColor(color);
	OPfontRender(text, p);

}

OPint OPimguiButton(OPvec2 pos, const OPchar* text, OPvec4 color, OPvec4 selected, OPvec4 hover) {

	OPvec2 size = OPIMGUI_ACTIVE->fontManager->_font->GetSize(text, OPIMGUI_ACTIVE->fontManager->scale);

	size.x += OPIMGUI_ACTIVE->padding[0] + OPIMGUI_ACTIVE->padding[1];
	size.y += OPIMGUI_ACTIVE->padding[2] + OPIMGUI_ACTIVE->padding[3];

	OPvec2 topLeft = pos;
	OPvec2 bottomRight = topLeft + size;


	OPint mx = OPMOUSE.X(), my = OPMOUSE.Y();
	OPint mouseWithin = mx >= topLeft.x && my >= topLeft.y &&
		mx <= bottomRight.x && my <= bottomRight.y;

	OPint clicked = mouseWithin && OPMOUSE.WasReleased(OPmouseButton::LBUTTON);
	OPint down = mouseWithin && OPMOUSE.IsDown(OPmouseButton::LBUTTON);

	if(clicked || down) {
		_block(pos, size, selected);
	} else if(mouseWithin) {
		_block(pos, size, hover);
	} else {
		_block(pos, size, color);
	}

	OPvec2 p = pos;
	p.x += OPIMGUI_ACTIVE->padding[0];
	p.y += OPIMGUI_ACTIVE->padding[2];

	OPfontColor(selected);
	OPfontRender(text, p);

	return clicked;
}

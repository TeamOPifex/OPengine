#include "./Human/include/Rendering/Primitives/OPline.h"

void OPline::Init(OPvec3* points, ui32 count) {
    OPvertexLayoutBuilder builder;
    builder.Init();
    builder.Add(OPattributes::POSITION);
    builder.Add(OPattributes::COLOR);
    model.Init(1, builder.Build());
    model.Build(count, points);
}

void OPline::Update(OPvec3* points, ui32 count) {
    model.Update(count, (f32*)points);
}

void OPline::Draw() {
    OPRENDERER_ACTIVE->SetDrawMode(OPdrawMode::Lines);
    model.Bind();
    OPrenderDrawBuffer(0);
    OPRENDERER_ACTIVE->SetDrawMode(OPdrawMode::Polygons);
}
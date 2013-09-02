#pragma once

#include <stdio.h>
#include "./Human/include/Resources/Model/Mesh.h"
#include "./Human/include/Resources/Model/MeshPacker.h"
#include "./Human/include/Resources/Model/PackedMesh.h"

Mesh* LoadOPM(const OPchar* filename);
PackedMesh* LoadOPM(const OPchar* filename, MeshPacker* meshPacker);
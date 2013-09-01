#pragma once

#include <stdio.h>
#include "./Human/include/Resources/Model/Mesh.h"
#include "./Human/include/Resources/Model/MeshPacker.h"
#include "./Human/include/Resources/Model/PackedMesh.h"

Mesh* LoadOPM(FILE* file);
PackedMesh* LoadOPM(FILE* file, MeshPacker* meshPacker);
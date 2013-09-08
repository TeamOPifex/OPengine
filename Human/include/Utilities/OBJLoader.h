#pragma once

#include <stdio.h>

#include "./Math/include/Vector2.h"
#include "./Math/include/Vector3.h"

#include "./Core/include/DynamicMemory.h"
#include "./Human/include/Resources/Model/Mesh.h"

Mesh* LoadOBJ(FILE* file, int start, int length);
int lineType(char* word);
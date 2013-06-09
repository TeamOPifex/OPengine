#pragma once

#include <stdio.h>

#include "./Human/Math/Vector2.h"
#include "./Human/Math/Vector3.h"

#include "./Core/include/DynamicMemory.h"
#include "./Human/Resources/Model/Mesh.h"

Mesh* LoadOBJ(FILE* file, int start, int length);
int lineType(char* word);
#pragma once

#include <stdio.h>
#include "./Human/Resources/Model/Mesh.h"

#include "./Human/Math/Vector2.h"
#include "./Human/Math/Vector3.h"
#include "OBJLoader.h"

Mesh* LoadOPM(FILE* file, int start, int length);
#pragma once

#include <cstdint>

#include "Renderer/Camera.h"
#include "Renderer/Mesh.h"
#include "Renderer/Texture.h"
#include "Structs/Block.h"
#include "Structs/Components.h"

uint32_t VBO;
uint32_t IBO;
uint32_t VAO;

bool FreeCursor = true;
Camera cam(45.0f, 0.01f, 100.0f, 800, 600);
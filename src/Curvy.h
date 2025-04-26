#pragma once

#include <cstdint>

#include "Renderer/Camera.h"
#include "Renderer/Texture.h"
#include "Structs/Components.h"

uint32_t VBO;
uint32_t IBO;
uint32_t VAO;

bool FreeCursor = true;
Camera cam(45.0f, 0.01f, 100.0f, 800, 600);

Transform CubeTransform{
    {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}};
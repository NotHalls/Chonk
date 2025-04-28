#pragma once

#include <cstdint>

#include "Block.h"
#include "Renderer/Camera.h"
#include "Renderer/Texture.h"
#include "Structs/Components.h"

uint32_t VBO;
uint32_t IBO;
uint32_t VAO;

bool FreeCursor = true;
Camera cam(45.0f, 0.01f, 100.0f, 800, 600);
// Block grassBlock = {26, 25, 27};
Block grassBlock = {0, 1, 2};

Transform CubeTransform{
    {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}};
#pragma once

#include <FastNoiseLite.h>

// clang-format off
enum class NoiseType
{
  OpenSimplex2  = FastNoiseLite::NoiseType_OpenSimplex2,
  OpenSimplex2S = FastNoiseLite::NoiseType_OpenSimplex2S,
  Cellular      = FastNoiseLite::NoiseType_Cellular,
  Perlin        = FastNoiseLite::NoiseType_Perlin,
  ValueCubic    = FastNoiseLite::NoiseType_ValueCubic,
  Value         = FastNoiseLite::NoiseType_Value
};
// clang-format on

class Noise
{
public:
  static void Init();
  static void SetSeed(int seed);
  static void SetNoiseType(NoiseType type);
  static FastNoiseLite Get();

private:
  static FastNoiseLite m_Noise;
};
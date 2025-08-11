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

struct NoiseSettings
{
  int Seed = 0;
  int Octaves = 3;
  float Frequency = 0.3f;
  float FrequencyMultiplier = 2.0f;
  float Amplitude = 1.0f;
  float AmplitudeDecay = 0.5f;

  NoiseType Type = NoiseType::Perlin;
};

class Noise
{
public:
  static FastNoiseLite Get();

  static void SetSeed(int seed);
  static void SetNoiseType(NoiseType type);
  static void SetNoiseSettings(const NoiseSettings &settings);
  static int GetNoise(int x, int z);
  static int GetNoise(int x, int y, int z);

private:
  static FastNoiseLite m_Noise;
  static NoiseSettings m_NoiseSettings;
};
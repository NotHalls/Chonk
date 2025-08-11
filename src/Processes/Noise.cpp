#include "Noise.h"

FastNoiseLite Noise::m_Noise;

void Noise::Init()
{
  SetNoiseType(NoiseType::Perlin);
  m_Noise.SetFrequency(0.01f);
}

void Noise::SetNoiseType(NoiseType type)
{
  m_Noise.SetNoiseType(static_cast<FastNoiseLite::NoiseType>(type));
}

void Noise::SetSeed(int seed) { m_Noise.SetSeed(seed); }

FastNoiseLite Noise::Get() { return m_Noise; }
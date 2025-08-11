#include "FixedGlobals.h"
#include "Noise.h"

NoiseSettings Noise::m_NoiseSettings;
FastNoiseLite Noise::m_Noise = FastNoiseLite(m_NoiseSettings.Seed);

void Noise::SetNoiseType(NoiseType type)
{
  m_Noise.SetNoiseType(static_cast<FastNoiseLite::NoiseType>(type));
}

void Noise::SetNoiseSettings(const NoiseSettings &settings)
{
  m_NoiseSettings = settings;
}

int Noise::GetNoise(int x, int z)
{
  float totalAmp = 0.0f;
  float finalNoise = 0.0f;

  float frequency = m_NoiseSettings.Frequency;
  float amplitude = m_NoiseSettings.Amplitude;

  for(int i = 0; i < m_NoiseSettings.Octaves; i++)
  {
    float sampleX = float(x) * frequency;
    float sampleZ = float(z) * frequency;

    finalNoise += m_Noise.GetNoise<float>(sampleX, sampleZ) * amplitude;
    totalAmp += amplitude;

    amplitude *= m_NoiseSettings.AmplitudeDecay;
    frequency *= m_NoiseSettings.FrequencyMultiplier;
  }

  finalNoise /= totalAmp;
  finalNoise = (finalNoise + 1.0f) / 2.0f; // Normalize to [0,1]

  return static_cast<int>(finalNoise * Global::CHUNK_SIZE_Y);
}

int Noise::GetNoise(int x, int y, int z) {}

void Noise::SetSeed(int seed) { m_Noise.SetSeed(seed); }

FastNoiseLite Noise::Get() { return m_Noise; }
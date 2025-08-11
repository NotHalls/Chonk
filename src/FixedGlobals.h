#pragma once

namespace Global
{
constexpr static const int CHUNK_SIZE_X = 16;
constexpr static const int CHUNK_SIZE_Y = 128;
constexpr static const int CHUNK_SIZE_Z = 16;

constexpr static const int CHUNK_VOLUME =
    CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z;

constexpr static const int SEA_LEVEL = 30;
} // namespace Global
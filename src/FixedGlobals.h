#pragma once

namespace Global
{
constexpr static const int CHUNK_SIZE_X = 16;
constexpr static const int CHUNK_SIZE_Y = 256;
constexpr static const int CHUNK_SIZE_Z = 16;

constexpr static const int CHUNK_VOLUME =
    CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z;
} // namespace Global
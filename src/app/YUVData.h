#pragma once

#include <QVector>

#include <vector>
#include <array>

struct Vector2D
{
	float x;
	float y;

	Vector2D(float xValue, float yValue)
		: x(xValue), y(yValue) {};
};

struct Vector3D
{
	float x;
	float y;
	float z;

	Vector3D(float xValue, float yValue, float zValue)
		: x(xValue), y(yValue), z(zValue) {};
};

struct VertexData
{
	Vector3D	vPosition;
	Vector2D	vTexCoord;
};

#define YUV_DEFAULT -1
enum
{
	YUV_Y,
	YUV_U,
	YUV_V,
	YUV_MAX
};
typedef std::array<uint32_t, YUV_MAX> YUV_TEXTURE;
typedef std::array<std::vector<uint8_t>, YUV_MAX> YUV_BUFFER;

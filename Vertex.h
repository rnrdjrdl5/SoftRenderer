#pragma once

#include "stdafx.h"
#include "Vector.h"

struct Vertex
{
public:
	Vertex() : color(RGB32(0,0,0)) {}
	Vertex(const Vector3 & InPosition, ULONG InColor) : position(InPosition), color(InColor) {}
	Vertex(const Vector3 & InPosition, ULONG InColor, Vector2 & InUV) 
		: position(InPosition), color(InColor), uv(InUV) {}

	Vector3 position;
	ULONG color;
	Vector2 uv;
};
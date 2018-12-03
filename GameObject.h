#pragma once

#include "stdafx.h"
#include "QuadMesh.h"
#include "Transform.h"

struct GameObject2D
{
public:
	GameObject2D() {};
	GameObject2D(const QuadMesh & InMesh)
		: Mesh(InMesh)
	{
	}

	void SetTransform(const Transform2D & newTransform)
	{
		Transform = newTransform;
		for (int i = 0; i < 2; i++)
		{
			Mesh.Triangles[i] = Mesh.Triangles[i].GetTransformedTriangle(Transform);
		}
	}

public:
	QuadMesh Mesh;
	Transform2D Transform;
};
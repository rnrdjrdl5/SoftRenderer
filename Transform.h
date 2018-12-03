#pragma once

#include "stdafx.h"
#include "Vector.h"

struct Transform2D
{
public:
	Transform2D() : Angle(0) 
	{
		SetTRSMatrix();
	}

	Transform2D(const Vector2 & InPosition, float InAngle, const Vector2 & InScale)
		: Position(InPosition), Angle(InAngle), Scale(InScale)
	{
		SetTRSMatrix();
	}

private:
	void SetTRSMatrix()
	{
		Matrix3 tMat, rMat, sMat;
		sMat.SetScale(Scale.X, Scale.Y, 1);
		rMat.SetRotation(Angle);
		tMat.SetTranslation(Position.X, Position.Y);
		TRSMatrix = tMat * rMat * sMat;
	}

public:
	Vector2 Position;
	float Angle;
	Vector2 Scale;
	Matrix3 TRSMatrix;
};
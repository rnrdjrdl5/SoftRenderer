#pragma once

#include "stdafx.h"
#include "Vertex.h"
#include "Transform.h"

struct Triangle
{
public:
	Triangle() 
	{
		CalculateValues();
	}

	Triangle(const Vertex & v1, const Vertex & v2, const Vertex & v3)
	{
		vt[0] = v1;
		vt[1] = v2;
		vt[2] = v3;
		CalculateValues();
	}

	//  Matrix 적용 후 return 
	Triangle GetTransformedTriangle(const Transform2D & newTransform) const
	{
		Triangle newTriangle = *this;


		for (int i = 0; i < 3; i++)
		{
			newTriangle.vt[i].position = newTriangle.vt[i].position * newTransform.TRSMatrix;
		}
		newTriangle.CalculateValues();
		return newTriangle;
	}

	Vector2 GetBaryCentricCoordinate(const Vector3 & ptToCheck) const 
	{
		Vector3 w = ptToCheck - vt[0].position;
		float dotUW = Vector3::Dot(u, w);
		float dotVW = Vector3::Dot(v, w);
		float s = (dotVV * dotUW - dotUV * dotVW) * invDenom;
		float t = (dotUU * dotVW - dotUV * dotUW) * invDenom;
		return Vector2(s, t);
	}

	ULONG GetInterpolatedColor(const Vector2 & baryValues) const
	{
		BYTE RV1 = GetRValue(vt[0].color);
		BYTE RV2 = GetRValue(vt[1].color);
		BYTE RV3 = GetRValue(vt[2].color);

		BYTE GV1 = GetGValue(vt[0].color);
		BYTE GV2 = GetGValue(vt[1].color);
		BYTE GV3 = GetGValue(vt[2].color);

		BYTE BV1 = GetBValue(vt[0].color);
		BYTE BV2 = GetBValue(vt[1].color);
		BYTE BV3 = GetBValue(vt[2].color);

		BYTE FinalR = (BYTE)(RV1 * (1 - baryValues.X - baryValues.Y) + RV2 * baryValues.X + RV3 * baryValues.Y);
		BYTE FinalG = (BYTE)(GV1 * (1 - baryValues.X - baryValues.Y) + GV2 * baryValues.X + GV3 * baryValues.Y);
		BYTE FinalB = (BYTE)(BV1 * (1 - baryValues.X - baryValues.Y) + BV2 * baryValues.X + BV3 * baryValues.Y);

		return RGB32(FinalR, FinalG, FinalB);
	}

	ULONG GetTextureColor(Texture* texture, const Vector2 & baryValues) const
	{
		Vector2 baryUV = vt[0].uv * (1 - baryValues.X - baryValues.Y) + vt[1].uv * baryValues.X + vt[2].uv * baryValues.Y;
		return texture->GetTexturePixel(baryUV);
	}

	// u v 벡터 계산
	void CalculateValues()
	{
		for (int i = 0; i < 3; i++)
		{
			if (vt[i].position.X < minPos.X) minPos.X = vt[i].position.X;
			if (vt[i].position.Y < minPos.Y) minPos.Y = vt[i].position.Y;
			if (vt[i].position.X > maxPos.X) maxPos.X = vt[i].position.X;
			if (vt[i].position.Y > maxPos.Y) maxPos.Y = vt[i].position.Y;
		}

		// 점 - 점 = 벡터를 이용, uv 계산 
		u = vt[1].position - vt[0].position;
		v = vt[2].position - vt[0].position;

		// 아래 왜 Dot 수행하는지 알아내야함
		dotUU = Vector3::Dot(u, u);
		dotUV = Vector3::Dot(u, v);
		dotVV = Vector3::Dot(v, v);
		invDenom = 1.0f / (dotUU * dotVV - dotUV * dotUV);
	}

public:
	Vertex vt[3];

	IntPoint minPt;
	IntPoint maxPt;
	Vector2 minPos = Vector2(INFINITY, INFINITY);
	Vector2 maxPos = Vector2(-INFINITY, -INFINITY);

	Vector3 u;
	Vector3 v;
	float dotUU, dotUV, dotVV, invDenom;
};
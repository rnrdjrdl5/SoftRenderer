
#include "stdafx.h"
#include "SoftRenderer.h"
#include "GDIHelper.h"
#include "Renderer.h"

#include "Vector.h"
#include "IntPoint.h"
#include "Vertex.h"
#include "GameObject.h"

bool IsInRange(int x, int y);
void PutPixel(int x, int y);

bool IsInRange(int x, int y)
{
	return (abs(x) < (g_nClientWidth / 2)) && (abs(y) < (g_nClientHeight / 2));
}

void PutPixel(const IntPoint& InPt)
{
	PutPixel(InPt.X, InPt.Y);
}

void PutPixel(int x, int y)
{
	if (!IsInRange(x, y)) return;

	ULONG* dest = (ULONG*)g_pBits;
	DWORD offset = g_nClientWidth * g_nClientHeight / 2 + g_nClientWidth / 2 + x + g_nClientWidth * -y;
	*(dest + offset) = g_CurrentColor;
}

void DrawLine(const Vector3 &start, const Vector3 &end)
{
	// L = (1-t)P + tQ   0 <= t <= 1
	float length = Vector3::Dist(end, start);
	float inc = 1.0f / length;
	int nLength = (int)(length + 1);

	for (int i = 0; i <= nLength; i++)
	{
		float t = inc * i;
		if (t >= length) t = 1.0f;
		Vector3 pt = start * (1.0f - t) + end * t;
		PutPixel(IntPoint(pt));
	}
}

void DrawTriangle(const Triangle &triangle)
{
	IntPoint minPt(triangle.minPos);
	IntPoint maxPt(triangle.maxPos);

	// loop 돌면서 각 지점이 포함되는지 체크한다.
	for (int x = minPt.X; x < maxPt.X; x++)
	{
		for (int y = minPt.Y; y < maxPt.Y; y++)
		{

			
			IntPoint pt(x, y);
			Vector2 baryValues = triangle.GetBaryCentricCoordinate(pt.ToVector3());

			if (baryValues.X >= 0 && baryValues.Y >= 0 && ((baryValues.X + baryValues.Y) <= 1))
			{
				ULONG finalColor = RGB32(255, 0, 0);
				if (g_Texture->IsLoaded())
				{
					finalColor = triangle.GetTextureColor(g_Texture, baryValues);
				}
				else
				{
					finalColor = triangle.GetInterpolatedColor(baryValues);
				}
				SetColor(finalColor);
				PutPixel(pt);
			}
		}
	}
}

void UpdateFrame(void)
{
	// Buffer Clear
	SetColor(32, 128, 255);
	Clear();

	static float theta = 0;
	static float pos = 0;
	static float scale = 1;

	// Input 
	if (GetAsyncKeyState(VK_LEFT)) theta -= 1;
	if (GetAsyncKeyState(VK_RIGHT)) theta += 1;
	if (GetAsyncKeyState(VK_UP)) pos += 1;
	if (GetAsyncKeyState(VK_DOWN)) pos -= 1;
	if (GetAsyncKeyState(VK_PRIOR)) scale += 0.01f;
	if (GetAsyncKeyState(VK_NEXT)) scale -= 0.01f;


	// 점 찍기
	/*static Vector3 p1 = Vector3::Make2DPoint(-80, -80);
	static Vector3 p2 = Vector3::Make2DPoint(-80, 80);
	static Vector3 p3 = Vector3::Make2DPoint(80, 80);
	static Vector3 p4 = Vector3::Make2DPoint(80, -80);*/

	// 머리
	static Vector3 p1 = Vector3::Make2DPoint(-15,30);
	static Vector3 p2 = Vector3::Make2DPoint(-15, 60);
	static Vector3 p3 = Vector3::Make2DPoint(15, 60);
	static Vector3 p4 = Vector3::Make2DPoint(15, 30);

	// 몸통
	static Vector3 p5 = Vector3::Make2DPoint(-15, -15);
	static Vector3 p6 = Vector3::Make2DPoint(-15, 30);
	static Vector3 p7 = Vector3::Make2DPoint(15, 30);
	static Vector3 p8 = Vector3::Make2DPoint(15, -15);


	// 팔 2개
	static Vector3 p9 = Vector3::Make2DPoint(-30, -15);
	static Vector3 p10 = Vector3::Make2DPoint(-30, 30);
	static Vector3 p11 = Vector3::Make2DPoint(-15, 30);
	static Vector3 p12 = Vector3::Make2DPoint(-15, -15);

	static Vector3 p13 = Vector3::Make2DPoint(15, -15);
	static Vector3 p14 = Vector3::Make2DPoint(15, 30);
	static Vector3 p15 = Vector3::Make2DPoint(30, 30);
	static Vector3 p16 = Vector3::Make2DPoint(30, -15);

	// 다리2개
	static Vector3 p17 = Vector3::Make2DPoint(-15, -60);
	static Vector3 p18 = Vector3::Make2DPoint(-15, -15);
	static Vector3 p19 = Vector3::Make2DPoint(0, -15);
	static Vector3 p20 = Vector3::Make2DPoint(0, -60);

	static Vector3 p21 = Vector3::Make2DPoint(0, -60);
	static Vector3 p22 = Vector3::Make2DPoint(0, -15);
	static Vector3 p23 = Vector3::Make2DPoint(15, -15);
	static Vector3 p24 = Vector3::Make2DPoint(15, -60);




	// 버텍스 설정 , Vector3, Color, UV 

	// 머리
	static Vertex v1(p1, RGB32(255, 0, 0), Vector2(0.125f, 0.25f));
	static Vertex v2(p2, RGB32(0, 255, 0), Vector2(0.125f, 0.125f));
	static Vertex v3(p3, RGB32(0, 0, 255), Vector2(0.25f, 0.125f));
	static Vertex v4(p4, RGB32(255, 255, 255), Vector2(0.25f, 0.25f));


	// 몸통
	static Vertex v5(p5, RGB32(255, 0, 0), Vector2(0.328125f, 0.5f));
	static Vertex v6(p6, RGB32(0, 255, 0), Vector2(0.328125f, 0.328125f));
	static Vertex v7(p7, RGB32(0, 0, 255), Vector2(0.4375f, 0.328125f));
	static Vertex v8(p8, RGB32(255, 255, 255), Vector2(0.4375f, 0.5f));


	// 팔2개
	static Vertex v9(p9, RGB32(255, 0, 0), Vector2(0.703125f, 0.5f));
	static Vertex v10(p10, RGB32(0, 255, 0), Vector2(0.703125f, 0.328125f));
	static Vertex v11(p11, RGB32(0, 0, 255), Vector2(0.75f, 0.328125f));
	static Vertex v12(p12, RGB32(255, 255, 255), Vector2(0.75f, 0.5f));

	static Vertex v13(p13, RGB32(255, 0, 0), Vector2(0.578125f, 1));
	static Vertex v14(p14, RGB32(0, 255, 0), Vector2(0.578125f, 0.828125f));
	static Vertex v15(p15, RGB32(0, 0, 255), Vector2(0.625f, 0.828125f));
	static Vertex v16(p16, RGB32(255, 255, 255), Vector2(0.625f, 1));

	// 다리2개
	static Vertex v17(p17, RGB32(255, 0, 0), Vector2(0.078125f, 0.5f));
	static Vertex v18(p18, RGB32(0, 255, 0), Vector2(0.078125f, 0.328f));
	static Vertex v19(p19, RGB32(0, 0, 255), Vector2(0.125f, 0.328f));
	static Vertex v20(p20, RGB32(255, 255, 255), Vector2(0.125f, 0.5f));

	static Vertex v21(p21, RGB32(255, 0, 0), Vector2(0.328125f, 1));
	static Vertex v22(p22, RGB32(0, 255, 0), Vector2(0.328125f, 0.828125f));
	static Vertex v23(p23, RGB32(0, 0, 255), Vector2(0.375f, 0.828125f));
	static Vertex v24(p24, RGB32(255, 255, 255), Vector2(0.375f, 1));



	// 버텍스를 통한 triangle 생성, u, v 계산

	// 머리
	static Triangle tri1(v1, v2, v3);
	static Triangle tri2(v1, v3, v4);

	//몸통
	static Triangle tri3(v5, v6, v7);
	static Triangle tri4(v5, v7, v8);

	// 팔2개
	static Triangle tri5(v9, v10, v11);
	static Triangle tri6(v9, v11, v12);

	static Triangle tri7(v13, v14, v15);
	static Triangle tri8(v13, v15, v16);

	// 다리2개
	static Triangle tri9(v17, v18, v19);
	static Triangle tri10(v17, v19, v20);

	static Triangle tri11(v21, v22, v23);
	static Triangle tri12(v21, v23, v24);



	// mesh 생성

	// 머리
	static QuadMesh mesh1(tri1, tri2);


	// 몸통
	static QuadMesh mesh2(tri3, tri4);

	
	// 팔2개
	static QuadMesh mesh3(tri5, tri6);
	static QuadMesh mesh4(tri7, tri8);

	// 다리2개
	static QuadMesh mesh5(tri9, tri10);
	static QuadMesh mesh6(tri11, tri12);


	// 각 TRS 값들 초기화요소로 사용, Matrix 생성
	Transform2D transform1(Vector2(pos, pos), theta, Vector2(scale, scale));

	// GameObject에 Mesh 설정
	GameObject2D obj1(mesh1);

	GameObject2D obj2(mesh2);

	GameObject2D obj3(mesh3);
	GameObject2D obj4(mesh4);

	GameObject2D obj5(mesh5);
	GameObject2D obj6(mesh6);

	// GameObject에 Transform 설정 + Matrix 설정을 통한 Position 변경
	obj1.SetTransform(transform1);

	obj2.SetTransform(transform1);

	obj3.SetTransform(transform1);
	obj4.SetTransform(transform1);

	obj5.SetTransform(transform1);
	obj6.SetTransform(transform1);

	
	// 그리기
	for (int i = 0; i < 2; i++)
	{
		DrawTriangle(obj1.Mesh.Triangles[i]);
		DrawTriangle(obj2.Mesh.Triangles[i]);
		DrawTriangle(obj3.Mesh.Triangles[i]);
		DrawTriangle(obj4.Mesh.Triangles[i]);
		DrawTriangle(obj5.Mesh.Triangles[i]);
		DrawTriangle(obj6.Mesh.Triangles[i]);
	}

	// Buffer Swap 
	BufferSwap();
}

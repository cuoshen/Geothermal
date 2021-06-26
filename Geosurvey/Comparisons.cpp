#include "pch.h"
#include "Comparisons.h"

using namespace DirectX;

bool AreEqual(XMVECTOR v1, XMVECTOR v2)
{
	XMFLOAT3 v1unpacked;
	XMStoreFloat3(&v1unpacked, v1);
	return AreEqual(v1unpacked, v2);
}

bool AreEqual(XMVECTOR v1, XMFLOAT3 v2)
{
	return AreEqual(v2, v1);
}

bool AreEqual(XMFLOAT3 v1, XMVECTOR v2)
{
	XMFLOAT3 v2unpacked;
	XMStoreFloat3(&v2unpacked, v2);
	return AreEqual(v1, v2unpacked);
}

bool AreEqual(XMFLOAT3 v1, XMFLOAT3 v2)
{
	return (v1.x == v2.x) && (v1.y == v2.y) && (v1.z == v2.z);
}
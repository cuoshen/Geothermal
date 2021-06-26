#pragma once
#include "pch.h"

bool AreEqual(DirectX::XMVECTOR v1, DirectX::XMVECTOR v2);
bool AreEqual(DirectX::XMVECTOR v1, DirectX::XMFLOAT3 v2);
bool AreEqual(DirectX::XMFLOAT3 v1, DirectX::XMVECTOR v2); 
bool AreEqual(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2);
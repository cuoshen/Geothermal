#include "pch.h"
#include "Transform.h"

using namespace DirectX;
using namespace Geothermal;

Transform::Transform():
	object2world(XMMatrixIdentity())
{
}

Transform::Transform(XMMATRIX initialTransform):
	object2world(initialTransform)
{
}

void Transform::ApplyTransform(XMMATRIX transform)
{
	object2world *= transform;
}

void Transform::UpdateTransform(XMMATRIX newTransform)
{
	object2world = newTransform;
}

XMVECTOR Transform::WorldPosition()
{
	return XMVector4Transform(ORIGIN, object2world);
}

XMVECTOR Transform::Front()
{
	return XMVector3Normalize(XMVector4Transform(FRONT, object2world));
}

XMVECTOR Transform::Up()
{
	return XMVector3Normalize(XMVector4Transform(UP, object2world));
}

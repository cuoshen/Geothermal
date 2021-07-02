#include "pch.h"
#include "ObjectRenderer.h"

using namespace Geothermal::Graphics;
using namespace Meshes;
using namespace DirectX;
using namespace std;

ObjectRenderer::ObjectRenderer(Mesh mesh, shared_ptr<DeviceResources> const& deviceResources):
	mesh(), object2WorldTransformBuffer(deviceResources, 0u), deviceResources(deviceResources)
{
	this->mesh.vertices = mesh.vertices;
}

void ObjectRenderer::Render(XMMATRIX object2World)
{
	object2WorldTransformBuffer.Update(XMMatrixTranspose(object2World));
	BindAllResources();
	// Issue draw call
	if (mesh.vertices->IsIndexed())
	{
		deviceResources->D3DDeviceContext()->DrawIndexed(mesh.vertices->GetIndexCount(), 0u, 0u);
	}
	else
	{
		deviceResources->D3DDeviceContext()->Draw(mesh.vertices->GetVertexCount(), 0u);
	}
}

void ObjectRenderer::BindAllResources()
{
	mesh.vertices->Bind();
	object2WorldTransformBuffer.Bind();
}

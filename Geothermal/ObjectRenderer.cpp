#include "pch.h"
#include "ObjectRenderer.h"

using namespace Geothermal::Graphics;
using namespace Meshes;

ObjectRenderer::ObjectRenderer(Mesh mesh, shared_ptr<DeviceResources> const& deviceResources):
	mesh(), transformBuffer(deviceResources, 0u), deviceResources(deviceResources)
{
	this->mesh.vertices = mesh.vertices;
}

void ObjectRenderer::Render(DirectX::XMMATRIX object2World)
{
	transformBuffer.Update(object2World);
	BindAllResources();
	// Issue draw call
	if (mesh.vertices->IsIndexed())
	{
		deviceResources->GetD3DDeviceContext()->DrawIndexed(mesh.vertices->GetIndexCount(), 0u, 0u);
	}
	else
	{
		deviceResources->GetD3DDeviceContext()->Draw(mesh.vertices->GetVertexCount(), 0u);
	}
}

void ObjectRenderer::BindAllResources()
{
	mesh.vertices->Bind();
	transformBuffer.Bind();
}

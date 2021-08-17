#include "pch.h"
#include "ObjectRenderer.h"

using namespace Geothermal::Graphics;
using namespace Meshes;
using namespace DirectX;
using namespace std;

ObjectRenderer::ObjectRenderer
(
	std::shared_ptr<DeviceResources> const& deviceResources,
	Meshes::Mesh mesh, std::shared_ptr<Materials::Material> material
) :
	mesh(mesh), object2WorldTransformBuffer(deviceResources, 0u),
	deviceResources(deviceResources), material(material)
{
}

void ObjectRenderer::Render(XMMATRIX object2World)
{
	object2WorldTransformBuffer.Update(XMMatrixTranspose(object2World));
	BindAllResources();
	// Issue draw call
	if (mesh.vertices->IsIndexed())
	{
		deviceResources->Context()->DrawIndexed(mesh.vertices->GetIndexCount(), 0u, 0u);
	}
	else
	{
		deviceResources->Context()->Draw(mesh.vertices->GetVertexCount(), 0u);
	}
}

void ObjectRenderer::BindAllResources()
{
	mesh.vertices->Bind();
	object2WorldTransformBuffer.Bind();

	if (material != nullptr)
	{
		material->Bind();
	}
}

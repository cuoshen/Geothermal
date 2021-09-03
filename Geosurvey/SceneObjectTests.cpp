#include "pch.h"
#include "CppUnitTest.h"
#include "GameObject.h"
#include "Scene.h"

#include "GameObject.cpp"
#include "Scene.cpp"

#define MOCK_OBJECT_RENDERER
#ifdef MOCK_OBJECT_RENDERER
namespace Geothermal::Graphics
{
	void ObjectRenderer::Render(XMMATRIX)
	{
	}

	ObjectRenderer::ObjectRenderer
	(
		std::shared_ptr<DeviceResources> const& deviceResources,
		Meshes::Mesh mesh, std::shared_ptr<Materials::Material> material
	) :
		mesh(mesh), object2WorldTransformBuffer(deviceResources, 0u),
		deviceResources(deviceResources), material(material)
	{
	}
}
#endif

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace DirectX;
using namespace Geothermal;

namespace Geosurvey
{
	TEST_CLASS(SceneObjectTests)
	{
	public:
		TEST_METHOD(GameObjectFactoryTest)
		{

		}
	};
}
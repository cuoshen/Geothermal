#include "pch.h"
#include "GameObject.h"
#include "Scene.h"

using namespace Geothermal;
using namespace Graphics;
using namespace Meshes;
using namespace Materials;
using namespace std;
using namespace DirectX;

GameObject::GameObject() :
	id(0), isActive(true), transform(nullptr), renderer(nullptr)
{
	transform = make_unique<Transform>();
}

void GameObject::Render() const
{
	if (!isActive)
	{
		// only render active game objects
		return;
	}
	if (renderer != nullptr)
	{
		renderer->Render(transform->GetObject2WorldMatrix());
	}
}

GameObjectFactory::GameObjectFactory() :
	product(nullptr), registered(false)
{
}

void GameObjectFactory::MakeNewProduct()
{
	product = make_shared<GameObject>();
	registered = false;
}

void GameObjectFactory::BuildTransform(DirectX::XMMATRIX initialTransform)
{
	product->transform = make_unique<Transform>(initialTransform);
}

void GameObjectFactory::BuildRenderer
(
	shared_ptr<DeviceResources> deviceResources,
	Mesh mesh, shared_ptr<Material> material
)
{
	product->renderer =
		make_unique<ObjectRenderer>(deviceResources, mesh, material);
}

void GameObjectFactory::SetObjectID(UINT64 id)
{
	product->id = id;
}

/// <summary>
/// Register the current object to main scene by default
/// </summary>
void GameObjectFactory::registerToScene()
{
	if (!registered)
	{
		Scene::Instance()->ObjectsInScene.push_back(product.get());
	}
}

shared_ptr<GameObject> GameObjectFactory::GetProduct()
{
	if (!registered)
	{
		registerToScene();
	}
	shared_ptr<GameObject> currentProduct = product;
	product.reset();	// Release ownership from builder class
	return currentProduct;
}

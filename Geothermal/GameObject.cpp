#include "pch.h"
#include "GameObject.h"
#include "Scene.h"
#include "SceneManager.h"

using namespace Geothermal;
using namespace SceneManagement;
using namespace Graphics;
using namespace Meshes;
using namespace Materials;
using namespace std;
using namespace DirectX;

GameObject::GameObject() :
	id(0), isActive(true), transform(nullptr), renderer(nullptr)
{
	RegisterToSceneManager();
	transform = make_unique<Transform>();
}

GameObject::~GameObject()
{
	DeregisterFromSceneManager();
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
		renderer->Render(transform->Object2WorldMatrix());
	}
}

inline void GameObject::RegisterToSceneManager()
{
	SceneManager::Instance().GameObjectRegistry.push_back(this);
}

inline void GameObject::DeregisterFromSceneManager()
{
	SceneManager::Instance().GameObjectRegistry.remove(this);
}

GameObjectFactory::GameObjectFactory() :
	product(nullptr)
{
}

void GameObjectFactory::MakeNewProduct()
{
	product = make_shared<GameObject>();
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

shared_ptr<GameObject> GameObjectFactory::GetProduct()
{
	shared_ptr<GameObject> currentProduct = product;
	product.reset();	// Release ownership from builder class
	return currentProduct;
}

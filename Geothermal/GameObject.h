#pragma once
#include "ObjectRenderer.h"
#include "Transform.h"
#include "DeviceResources.h"
#include "Mesh.h"
#include "PoolAllocator.h"

namespace Geothermal
{
	/// <summary>
	/// Game object base class.
	/// </summary>
	class GameObject
	{
	public:
		GameObject();
		~GameObject();

		friend class GameObjectFactory;

		/// <summary>
		/// Update is called once per frame
		/// </summary>
		virtual void Update() {}
		/// <summary>
		/// Have the object renderer draw the object
		/// </summary>
		virtual void Render() const;

		bool IsActive() const { return isActive; }
		Transform& GetTransform() const { return *transform.get(); }
		Graphics::ObjectRenderer& Renderer() const { return *renderer.get(); }

		static void* operator new(size_t size);
		static void operator delete(void* pointerToObject, size_t size);

	protected:
		static PoolAllocator allocator;

		inline void RegisterToSceneManager();
		inline void DeregisterFromSceneManager();

		UINT64 id;	// 64-bit unsigned integer ID which uniquely identify the game object
		bool isActive;
		std::unique_ptr<Graphics::ObjectRenderer> renderer;	// Each game object carry its own renderer for its graphics
		std::unique_ptr<Transform> transform;
	};

	class GameObjectFactory
	{
	public:
		GameObjectFactory();

		void MakeNewProduct();
		void BuildTransform(DirectX::XMMATRIX initialTransform);
		void BuildRenderer
		(
			std::shared_ptr<Graphics::DeviceResources> deviceResources,
			Graphics::Meshes::Mesh mesh, std::shared_ptr<Graphics::Materials::Material> material
		);
		void SetObjectID(UINT64 id);

		/// <summary>
		/// Return finalized game object product and release ownership
		/// </summary>
		std::shared_ptr<GameObject> GetProduct();

	protected:
		std::shared_ptr<GameObject> product;
	};
}

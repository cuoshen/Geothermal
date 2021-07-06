#pragma once
#include "tiny_obj_loader.h"
#include "Mesh.h"
#include "Vertices.h"
#include "DeviceResources.h"

namespace Geothermal
{
	class ModelLoader
	{
	public:
		ModelLoader();

		/// <summary>
		/// Attempt to load an obj file into a mesh
		/// </summary>
		bool LoadObj2Mesh
		(
			winrt::hstring const& objFileName,
			winrt::hstring const& mtlFileName,
			Geothermal::Graphics::Meshes::Mesh* mesh,
			std::shared_ptr<Geothermal::Graphics::DeviceResources> const& deviceResources
		);

		/// <summary>
		/// Instead of using an obj file, we use two strings for the obj and mtl files.
		/// </summary>
		bool LoadObjString2Mesh
		(
			std::string objString,
			std::string mtlString,
			Geothermal::Graphics::Meshes::Mesh* mesh,
			std::shared_ptr<Geothermal::Graphics::DeviceResources> const& deviceResources
		);
	private:
		std::vector<Geothermal::Graphics::Structures::VertexPNT> ParseVertices();
		void ConstructVertex
		(
			Geothermal::Graphics::Structures::VertexPNT* vertex,
			tinyobj::index_t index, 
			const tinyobj::attrib_t& attrib
		);

		tinyobj::ObjReader reader;
		tinyobj::ObjReaderConfig reader_config;
	};
}

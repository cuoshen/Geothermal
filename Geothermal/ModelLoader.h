#pragma once
#include "tiny_obj_loader.h"
#include "Mesh.h"
#include "GraphicResources.h"

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
		std::vector<Geothermal::Graphics::Structures::VertexPNTT> ParseVertices();
		/// <summary>
		/// Assemble a single vertex from parsed attribute data
		/// </summary>
		void ConstructVertex
		(
			Geothermal::Graphics::Structures::VertexPNTT* vertex,
			tinyobj::index_t index,
			const tinyobj::attrib_t& attrib
		);
		/// <summary>
		/// Compute per-vertex tangent given triangle with position+uv
		/// </summary>
		void ComputeTangent
		(
			Geothermal::Graphics::Structures::VertexPNTT triangle[3]
		);

		tinyobj::ObjReader reader;
		tinyobj::ObjReaderConfig reader_config;
	};
}

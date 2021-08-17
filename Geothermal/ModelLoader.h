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
		/// <returns>Whether the operation is successful</returns>
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
		/// <returns>Whether the operation is successful</returns>
		bool LoadObjString2Mesh
		(
			std::string objString,
			std::string mtlString,
			Geothermal::Graphics::Meshes::Mesh* mesh,
			std::shared_ptr<Geothermal::Graphics::DeviceResources> const& deviceResources
		);

	private:
		/// <summary>
		/// At this point we already have tiny obj loader parse the obj into some arrays of data.
		/// Assemble a Geothermal::Graphics::Meshes::Mesh from the information parsed.
		/// </summary>
		/// <returns>Whether the operation is successful</returns>
		bool AssembleMesh
		(
			Geothermal::Graphics::Meshes::Mesh* mesh,
			std::shared_ptr<Geothermal::Graphics::DeviceResources> const& deviceResources
		);
		/// <summary>
		/// Construct a single vertex from parsed attribute data
		/// </summary>
		inline Geothermal::Graphics::Structures::VertexPNTT ConstructVertex
		(
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

		std::unique_ptr<tinyobj::ObjReader> reader;
		tinyobj::ObjReaderConfig reader_config;
	};
}

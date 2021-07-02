#include "pch.h"
#include "ModelLoader.h"
#include "Mesh.h"
#include "VertexBuffer.h"
#include "FileIO.h"

using namespace Geothermal;
using namespace Graphics;
using namespace Meshes;
using namespace Structures;
using namespace Bindables;
using namespace std;

ModelLoader::ModelLoader():
	reader(), reader_config()
{
	reader_config.vertex_color = false;
}

bool ModelLoader::LoadObj2Mesh
(
	winrt::hstring const& objFileName,
	winrt::hstring const& mtlFileName, 
	Mesh* mesh, 
	shared_ptr<DeviceResources> const& deviceResources
)
{
	if (mesh == nullptr)
	{
		return false;	// Cannot load into nullptr
	}
	string objString = FileIO::ReadData(objFileName);
	string mtlString = FileIO::ReadData(mtlFileName);
	return LoadObjString2Mesh(objString, mtlString, mesh, deviceResources);
}

bool ModelLoader::LoadObjString2Mesh
(string objString, string mtlString, Mesh* mesh, std::shared_ptr<DeviceResources> const& deviceResources)
{
	if (mesh == nullptr)
	{
		return false;	// Cannot load into nullptr
	}
	if (!reader.ParseFromString(objString, mtlString, reader_config))
	{
		if (!reader.Error().empty())
		{
			OutputDebugStringA(reader.Error().c_str());
		}
		return false;	// Failed to parse obj file
	}
	if (!reader.Warning().empty())
	{
		OutputDebugStringA(reader.Warning().c_str());
	}

	vector<VertexPNT> verticesParsed = ParseVertices();
	if (verticesParsed.size() > 0)
	{
		// Create vertex buffer and load into mesh
		mesh->vertices = nullptr;
		mesh->vertices = make_shared<IndexedVertexBuffer<VertexPNT>>(deviceResources, verticesParsed);
	}
	else
	{
		OutputDebugString(L"No shape parsed \n");
	}

	return true;
}

vector<VertexPNT> ModelLoader::ParseVertices()
{
	auto& attrib = reader.GetAttrib();
	auto& shapes = reader.GetShapes();
	auto& materials = reader.GetMaterials();

	vector<VertexPNT> vertices;
	// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++)
	{
		tinyobj::shape_t currentShape = shapes[s];
		// Loop over faces

		size_t index_offset = 0;
		for (size_t f = 0; f < currentShape.mesh.num_face_vertices.size(); f++)
		{
			// We enforce the rule that each face must be a triangle,
			// if the shape is not fully triangulated, the loading operation is unsuccessful
			if (currentShape.mesh.num_face_vertices[f] != 3)
			{
				OutputDebugString(L"Error parsing obj: Shape is not fully triangulated. \n");
				return vector<VertexPNT>();
			}
			// Read vertices, always assume triangular face
			for (size_t v = 0; v < 3; v++)
			{
				tinyobj::index_t index = currentShape.mesh.indices[index_offset + v];
				VertexPNT vertex;
				size_t startingIndex = 3 * size_t(index.vertex_index);
				vertex.position.x = attrib.vertices[startingIndex + 0];
				vertex.position.y = attrib.vertices[startingIndex + 1];
				vertex.position.z = attrib.vertices[startingIndex + 2];
				if (index.normal_index >= 0)
				{
					startingIndex = 3 * size_t(index.normal_index);
					vertex.normal.x = attrib.normals[startingIndex + 0];
					vertex.normal.y = attrib.normals[startingIndex + 1];
					vertex.normal.z = attrib.normals[startingIndex + 2];
				}
				if (index.texcoord_index >= 0)
				{
					startingIndex = 2 * size_t(index.texcoord_index);
					vertex.textureCoordinate.x = attrib.texcoords[startingIndex + 0];
					vertex.textureCoordinate.y = attrib.texcoords[startingIndex + 1];
				}
				vertices.push_back(vertex);
			}
			index_offset += 3;
		}
	}

	return vertices;
}

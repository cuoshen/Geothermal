#include "pch.h"
#include "ModelLoader.h"
#include "Mesh.h"
#include "GraphicResources.h"
#include "FileIO.h"
#include <limits>

using namespace DirectX;
using namespace Geothermal;
using namespace Graphics;
using namespace Meshes;
using namespace Structures;
using namespace Bindables;
using namespace std;

ModelLoader::ModelLoader() :
	reader(nullptr), reader_config()
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
(
	string objString, string mtlString, Mesh* mesh,
	shared_ptr<DeviceResources> const& deviceResources
)
{
	// Clear existing data from previous read action
	reader = nullptr;
	reader = make_unique<tinyobj::ObjReader>();

	if (mesh == nullptr)
	{
		return false;	// Cannot load into nullptr
	}
	if (!reader->ParseFromString(objString, mtlString, reader_config))
	{
		if (!reader->Error().empty())
		{
			OutputDebugStringA(reader->Error().c_str());
		}
		return false;	// Failed to parse obj file
	}
	if (!reader->Warning().empty())
	{
		OutputDebugStringA(reader->Warning().c_str());
	}

	return AssembleMesh(mesh, deviceResources);
}

bool ModelLoader::AssembleMesh(Mesh* mesh, shared_ptr<DeviceResources> const& deviceResources)
{
	const tinyobj::attrib_t& attrib = reader->GetAttrib();
	const vector<tinyobj::shape_t>& shapes = reader->GetShapes();
	const vector<tinyobj::material_t>& materials = reader->GetMaterials();

	vector<VertexPNTT> vertices;
	shared_ptr<AABB> bounds = make_shared<AABB>();
	float floatMin = numeric_limits<float>::min();
	float floatMax = numeric_limits<float>::max();
	bounds->MinXYZ = XMFLOAT3{ floatMin, floatMin, floatMin };
	bounds->MaxXYZ = XMFLOAT3{ floatMax, floatMax, floatMax };
	VertexPNTT* triangle = new VertexPNTT[3];

	// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++)
	{
		tinyobj::shape_t currentShape = shapes[s];
		// Loop over faces
		size_t index_offset = 0;
		for (size_t f = 0; f < currentShape.mesh.num_face_vertices.size(); f++)	// For each triangle
		{
			// We enforce the rule that each face must be a triangle,
			// if the shape is not fully triangulated, the loading operation is unsuccessful
			assert(currentShape.mesh.num_face_vertices[f] == 3);

			for (size_t v = 0; v < 3; v++)	// For each vertex
			{
				tinyobj::index_t index = currentShape.mesh.indices[index_offset + v];
				triangle[v] = ConstructVertex(index, attrib);
				UpdateBounds(*bounds.get(), triangle[v].position);
			}

			// Compute tangent
			ComputeTangent(triangle);

			for (size_t i = 0; i < 3; i++)
			{
				vertices.push_back(triangle[i]);
			}

			index_offset += 3;
		}
	}
	delete[] triangle;

	if (vertices.empty())
	{
		mesh->vertices = nullptr;
		mesh->bounds = nullptr;
		OutputDebugString(L"No shape parsed \n");
		return false;
	}

	// Create vertex buffer and load into mesh
	mesh->vertices =
		make_shared<IndexedVertexBuffer<VertexPNTT>>(deviceResources, vertices);
	mesh->bounds = bounds;

	return true;
}

inline VertexPNTT ModelLoader::ConstructVertex(tinyobj::index_t index, const tinyobj::attrib_t& attrib)
{
	VertexPNTT vertex;
	size_t startingIndex = 3 * size_t(index.vertex_index);
	vertex.position =
	{
		attrib.vertices[startingIndex + 0],
		attrib.vertices[startingIndex + 1],
		attrib.vertices[startingIndex + 2]
	};
	if (index.normal_index >= 0)
	{
		startingIndex = 3 * size_t(index.normal_index);
		vertex.normal =
		{
			attrib.normals[startingIndex + 0],
			attrib.normals[startingIndex + 1],
			attrib.normals[startingIndex + 2]
		};
	}
	if (index.texcoord_index >= 0)
	{
		startingIndex = 2 * size_t(index.texcoord_index);
		vertex.textureCoordinate =
		{
			attrib.texcoords[startingIndex + 0],
			attrib.texcoords[startingIndex + 1]
		};
	}

	return vertex;
}

void ModelLoader::ComputeTangent(VertexPNTT triangle[3])
{
	// Solve the equation EdgeMatrix == DeltaUVMatrix . TangentBitangentBasisMatrix

	XMFLOAT3& p0 = triangle[0].position;
	XMFLOAT3& p1 = triangle[1].position;
	XMFLOAT3& p2 = triangle[2].position;

	XMFLOAT3 edge0 =
	{
		p1.x - p0.x,
		p1.y - p0.y,
		p1.z - p0.z
	};
	XMFLOAT3 edge1 =
	{
		p2.x - p0.x,
		p2.y - p0.y,
		p2.z - p0.z
	};

	float du0 = { triangle[1].textureCoordinate.x - triangle[0].textureCoordinate.x };
	float du1 = { triangle[2].textureCoordinate.x - triangle[0].textureCoordinate.x };
	float dv0 = { triangle[1].textureCoordinate.y - triangle[0].textureCoordinate.y };
	float dv1 = { triangle[2].textureCoordinate.y - triangle[0].textureCoordinate.y };

	float determinantReciprocal = 1.0f / (du0 * dv1 - du1 * dv0);
	XMFLOAT3 tangent =
	{
		determinantReciprocal * (dv1 * edge0.x - dv0 * edge1.x),
		determinantReciprocal * (dv1 * edge0.y - dv0 * edge1.y),
		determinantReciprocal * (dv1 * edge0.z - dv0 * edge1.z)
	};

	triangle[0].tangent = triangle[1].tangent = triangle[2].tangent = tangent;
}

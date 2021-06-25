#include "pch.h"
#include "QuadMesh.h"
#include "Vertices.h"
#include "VertexBuffer.h"

using namespace Geothermal::Graphics;
using namespace Meshes;
using namespace Structures;
using namespace Bindables;
using namespace std;

QuadMesh::QuadMesh(shared_ptr<DeviceResources> const& deviceResources)
{
	VertexPNT verts[] =
	{
		{{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
		{{0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
		{{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
		{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
	};
	vector<VertexPNT> vertices(begin(verts), end(verts));
	UINT ind[] =
	{
		0, 2, 1,
		1, 2, 3,
		0, 1, 2,
		1, 3, 2,
	};
	vector<UINT> indices(begin(ind), end(ind));
	this->vertices = make_shared<IndexedVertexBuffer<VertexPNT>>(deviceResources, vertices, indices);
}

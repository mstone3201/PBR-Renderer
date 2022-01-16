#pragma once

#include <fstream>
#include <d3d11.h>
#include <vector>

struct MESH_HEADER {
	unsigned vertexSize;
	unsigned indexSize;
	unsigned materialsSize;
};

HRESULT readMeshFile(std::string, MESH_HEADER&, unsigned char*&);
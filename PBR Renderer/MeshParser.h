#pragma once

#include <fstream>
#include <vector>
#include <winerror.h>

struct MESH_HEADER {
	unsigned vertexSize;
	unsigned indexSize;
	unsigned materialsSize;
};

HRESULT readMeshFile(std::string, MESH_HEADER&, unsigned char*&);
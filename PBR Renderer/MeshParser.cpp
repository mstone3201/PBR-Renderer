#include "MeshParser.h"

HRESULT readMeshFile(std::string fileName, MESH_HEADER& header, unsigned char*& data) {
	HRESULT result = ERROR_FILE_NOT_FOUND;

	FILE* file;
	fopen_s(&file, fileName.c_str(), "rb");
	if(file) {
		fread(&header, sizeof(MESH_HEADER), 1, file);
		
		unsigned size = header.vertexSize + header.indexSize + header.materialsSize;
		data = new unsigned char[size];
		fread(data, 1, size, file);

		result = S_OK;
		fclose(file);
	}

	return result;
}
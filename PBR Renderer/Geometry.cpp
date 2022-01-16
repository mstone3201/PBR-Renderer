#include "Geometry.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "MeshParser.h"

HRESULT Geometry::initMesh(Graphics& graphics, ID3D11Buffer** vertexBuffer, ID3D11Buffer** indexBuffer, unsigned& numIndices, std::string*& materials, unsigned& numMaterials, std::string meshFile) {
	MESH_HEADER header;
	unsigned char* data;

	HRESULT result = readMeshFile(meshFile, header, data);
	if(FAILED(result)) return result;

	numIndices = header.indexSize / sizeof(unsigned);

	result = VertexBuffer::init(graphics, vertexBuffer, data, header.vertexSize);
	
	if(SUCCEEDED(result)) result = IndexBuffer::init(graphics, indexBuffer, data + header.vertexSize, header.indexSize);

	if(SUCCEEDED(result)) {
		numMaterials = 0;
		for(unsigned i = 0; i < header.materialsSize; i++)
			if(!data[header.vertexSize + header.indexSize + i])
				numMaterials++;
		
		materials = new std::string[numMaterials];
		for(unsigned i = 0, s = header.vertexSize + header.indexSize; i < numMaterials; i++) {
			std::string str((char*)(data + s));
			materials[i] = str;
			s += (unsigned)str.length() + 1;
		}
	}

	delete[] data;
	return result;
}
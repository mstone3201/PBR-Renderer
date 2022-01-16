#include "Material.h"
#include "ImageTexture.h"

HRESULT Material::init(Graphics& graphics, ID3D11Texture2D** textures, ID3D11ShaderResourceView** resources, std::string material) {
	// assume that the textures are adjacent
	HRESULT result = ImageTexture::initDDS(graphics, textures, resources, material + "/bbbx.dds");
	if(FAILED(result)) return result;
	result = ImageTexture::initDDS(graphics, textures + 1, resources + 1, material + "/nnnh.dds");
	if(FAILED(result)) return result;
	result = ImageTexture::initDDS(graphics, textures + 2, resources + 2, material + "/rmax.dds");

	return result;
}

HRESULT Material::init(Graphics& graphics, ID3D11Texture2D** textures, ID3D11ShaderResourceView** resources, std::string* materials, unsigned numMaterials) {
	std::string* materialFiles = new std::string[numMaterials * 3];
	for(unsigned i = 0; i < numMaterials; i++) {
		std::string basePath = "../../res/materials/" + materials[i] + "/";
		materialFiles[i] = basePath + "bbbx.dds";
		materialFiles[numMaterials + i] = basePath + "nnnh.dds";
		materialFiles[numMaterials * 2 + i] = basePath + "rmax.dds";
	}

	// assume that the textures are adjacent
	HRESULT result = ImageTexture::initDDS(graphics, textures, resources, materialFiles, numMaterials);
	if(SUCCEEDED(result)) result = ImageTexture::initDDS(graphics, textures + 1, resources + 1, materialFiles + numMaterials, numMaterials);
	if(SUCCEEDED(result)) result = ImageTexture::initDDS(graphics, textures + 2, resources + 2, materialFiles + numMaterials + numMaterials, numMaterials);

	delete[] materialFiles;
	return result;
}
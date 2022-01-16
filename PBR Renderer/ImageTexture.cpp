#include "ImageTexture.h"
#include "DDSParser.h"
#pragma warning(push, 0)
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#pragma warning(pop)

HRESULT ImageTexture::init(Graphics& graphics, ID3D11Texture2D** texture, ID3D11ShaderResourceView** resource, DXGI_FORMAT format, std::string imgFile) {
	int width, height, channels;
	unsigned char* pixels = stbi_load(imgFile.c_str(), &width, &height, &channels, 4);
	if(!pixels) return ERROR_FILE_NOT_FOUND;

	D3D11_TEXTURE2D_DESC textureDesc{};
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = format;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_IMMUTABLE;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA resourceData{};
	resourceData.pSysMem = pixels;
	resourceData.SysMemPitch = width*sizeof(unsigned char)*4;
	resourceData.SysMemSlicePitch = resourceData.SysMemPitch*height;

	HRESULT result = graphics.getDevice()->CreateTexture2D(&textureDesc, &resourceData, texture);
	if(SUCCEEDED(result)) {
		D3D11_SHADER_RESOURCE_VIEW_DESC resourceDesc{};
		resourceDesc.Format = textureDesc.Format;
		resourceDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		resourceDesc.Texture2D.MostDetailedMip = 0;
		resourceDesc.Texture2D.MipLevels = 1;
		
		result = graphics.getDevice()->CreateShaderResourceView(*texture, &resourceDesc, resource);
	}

	stbi_image_free(pixels);
	return result;
}

HRESULT ImageTexture::initDDS(Graphics& graphics, ID3D11Texture2D** texture, ID3D11ShaderResourceView** resource, std::string ddsFile) {
	DDS_HEADER header;
	DDS_HEADER_DXT10 header10;
	unsigned char* data;

	HRESULT result = readDDSFile(ddsFile, header, header10, data);
	if(FAILED(result)) return result;

	D3D11_TEXTURE2D_DESC textureDesc{};
	textureDesc.Width = header.dwWidth;
	textureDesc.Height = header.dwHeight;
	textureDesc.MipLevels = header.dwMipMapCount;
	textureDesc.ArraySize = header10.miscFlag ? 6 : 1; // don't support texture arrays
	textureDesc.Format = header10.dxgiFormat;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_IMMUTABLE;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = header10.miscFlag; // whether it is a texturecube

	unsigned bpp = 0, blockSize = 0;
	switch(header10.dxgiFormat) {
	case DXGI_FORMAT_B8G8R8X8_UNORM:
	case DXGI_FORMAT_B8G8R8A8_UNORM:
	case DXGI_FORMAT_R16G16_FLOAT:
		bpp = 32;
		break;
	case DXGI_FORMAT_R16G16B16A16_FLOAT:
		bpp = 64;
		break;
	case DXGI_FORMAT_R32G32B32A32_FLOAT:
		bpp = 128;
		break;
		// add more formats as needed
	}

	unsigned len = textureDesc.MipLevels * textureDesc.ArraySize; // avoid possible overflow by storing in int
	D3D11_SUBRESOURCE_DATA* resourceData = new D3D11_SUBRESOURCE_DATA[len]{};
	unsigned char* subData = data;
	for(unsigned i = 0; i < textureDesc.ArraySize; i++) {
		unsigned width = header.dwWidth, height = header.dwHeight;
		for(unsigned mip = 0; mip < textureDesc.MipLevels; mip++) {
			unsigned index = i * textureDesc.MipLevels + mip;
			if(index >= len) { // avoid possible overflow by keeping index in bounds
				i = -1;
				break;
			}
			resourceData[index].pSysMem = subData;
			resourceData[index].SysMemPitch = blockSize ? max(1, (width + 3) / 4) * blockSize : (width * bpp + 7) / 8;
			resourceData[index].SysMemSlicePitch = resourceData[index].SysMemPitch * height;
			width = max(1, width / 2);
			height = max(1, height / 2);
			subData += resourceData[index].SysMemSlicePitch;
		}
	}

	result = graphics.getDevice()->CreateTexture2D(&textureDesc, resourceData, texture);

	if(SUCCEEDED(result)) {
		D3D11_SHADER_RESOURCE_VIEW_DESC resourceDesc{};
		resourceDesc.Format = textureDesc.Format;
		if(header10.miscFlag) { // it is a texturecube
			resourceDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
			resourceDesc.TextureCube.MostDetailedMip = 0;
			resourceDesc.TextureCube.MipLevels = header.dwMipMapCount;
		} else { // normal texture2d
			resourceDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			resourceDesc.Texture2D.MostDetailedMip = 0;
			resourceDesc.Texture2D.MipLevels = header.dwMipMapCount;
		}

		result = graphics.getDevice()->CreateShaderResourceView(*texture, &resourceDesc, resource);
	}

	delete[] resourceData;
	delete[] data;
	return result;
}

HRESULT ImageTexture::initDDS(Graphics& graphics, ID3D11Texture2D** texture, ID3D11ShaderResourceView** resource, std::string* ddsFiles, unsigned numTextures) {
	DDS_HEADER header;
	DDS_HEADER_DXT10 header10;
	unsigned char* data;

	HRESULT result = readDDSFiles(ddsFiles, numTextures, header, header10, data);
	if(FAILED(result)) return result;

	D3D11_TEXTURE2D_DESC textureDesc{};
	textureDesc.Width = header.dwWidth;
	textureDesc.Height = header.dwHeight;
	textureDesc.MipLevels = header.dwMipMapCount;
	textureDesc.ArraySize = header10.miscFlag ? 6 * numTextures : numTextures; // don't support texture arrays
	textureDesc.Format = header10.dxgiFormat;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_IMMUTABLE;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = header10.miscFlag; // whether it is a texturecube

	unsigned bpp = 0, blockSize = 0;
	switch(header10.dxgiFormat) {
	case DXGI_FORMAT_B8G8R8X8_UNORM:
	case DXGI_FORMAT_B8G8R8A8_UNORM:
	case DXGI_FORMAT_R16G16_FLOAT:
		bpp = 32;
		break;
	case DXGI_FORMAT_R16G16B16A16_FLOAT:
		bpp = 64;
		break;
	case DXGI_FORMAT_R32G32B32A32_FLOAT:
		bpp = 128;
		break;
		// add more formats as needed
	}

	unsigned len = textureDesc.MipLevels * textureDesc.ArraySize; // avoid possible overflow by storing in int
	D3D11_SUBRESOURCE_DATA* resourceData = new D3D11_SUBRESOURCE_DATA[len]{};
	unsigned char* subData = data;
	for(unsigned i = 0; i < textureDesc.ArraySize; i++) {
		unsigned width = header.dwWidth, height = header.dwHeight;
		for(unsigned mip = 0; mip < textureDesc.MipLevels; mip++) {
			unsigned index = i * textureDesc.MipLevels + mip;
			if(index >= len) { // avoid possible overflow by keeping index in bounds
				i = -1;
				break;
			}
			resourceData[index].pSysMem = subData;
			resourceData[index].SysMemPitch = blockSize ? max(1, (width + 3) / 4) * blockSize : (width * bpp + 7) / 8;
			resourceData[index].SysMemSlicePitch = resourceData[index].SysMemPitch * height;
			width = max(1, width / 2);
			height = max(1, height / 2);
			subData += resourceData[index].SysMemSlicePitch;
		}
	}

	result = graphics.getDevice()->CreateTexture2D(&textureDesc, resourceData, texture);

	if(SUCCEEDED(result)) {
		D3D11_SHADER_RESOURCE_VIEW_DESC resourceDesc{};
		resourceDesc.Format = textureDesc.Format;
		if(header10.miscFlag) { // it is a texturecube
			resourceDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBEARRAY;
			resourceDesc.TextureCubeArray.MostDetailedMip = 0;
			resourceDesc.TextureCubeArray.MipLevels = header.dwMipMapCount;
			resourceDesc.TextureCubeArray.First2DArrayFace = 0;
			resourceDesc.TextureCubeArray.NumCubes = numTextures;
		} else { // normal texture2d
			resourceDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
			resourceDesc.Texture2DArray.MostDetailedMip = 0;
			resourceDesc.Texture2DArray.MipLevels = header.dwMipMapCount;
			resourceDesc.Texture2DArray.FirstArraySlice = 0;
			resourceDesc.Texture2DArray.ArraySize = numTextures;
		}

		result = graphics.getDevice()->CreateShaderResourceView(*texture, &resourceDesc, resource);
	}

	delete[] resourceData;
	delete[] data;
	return result;
}
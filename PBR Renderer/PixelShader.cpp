#include "PixelShader.h"
#include "ShaderParser.h"

HRESULT PixelShader::init(Graphics& graphics, ID3D11PixelShader** shader, std::string shaderFile) {
	unsigned char* shaderBytes;
	size_t shaderSize = readShaderFile(shaderFile, shaderBytes);
	if(!shaderSize) return D3D11_ERROR_FILE_NOT_FOUND;

	HRESULT result = graphics.getDevice()->CreatePixelShader((void*)shaderBytes, shaderSize, 0, shader);

	delete[] shaderBytes;
	return result;
}

HRESULT PixelShader::initForwardPBR(Graphics& graphics, ID3D11PixelShader** shader) {
	return init(graphics, shader, "ForwardPBRPS.cso");
}

HRESULT PixelShader::initSkyBox(Graphics& graphics, ID3D11PixelShader** shader) {
	return init(graphics, shader, "SkyBoxPS.cso");
}
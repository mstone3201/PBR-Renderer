#include "VertexShader.h"
#include "ShaderParser.h"

HRESULT VertexShader::init(Graphics& graphics, ID3D11VertexShader** shader, ID3D11InputLayout** inputLayout, D3D11_INPUT_ELEMENT_DESC* inputs, unsigned numInputs, std::string shaderFile) {
	unsigned char* shaderBytes;
	size_t shaderSize = readShaderFile(shaderFile, shaderBytes);
	if(!shaderSize) return D3D11_ERROR_FILE_NOT_FOUND;

	HRESULT result = graphics.getDevice()->CreateVertexShader((void*)shaderBytes, shaderSize, 0, shader);
	if(SUCCEEDED(result)) {
		result = graphics.getDevice()->CreateInputLayout(inputs, numInputs, (void*)shaderBytes, shaderSize, inputLayout);
	}

	delete[] shaderBytes;
	return result;
}

HRESULT VertexShader::init(Graphics& graphics, ID3D11VertexShader** shader, std::string shaderFile) {
	unsigned char* shaderBytes;
	size_t shaderSize = readShaderFile(shaderFile, shaderBytes);
	if(!shaderSize) return D3D11_ERROR_FILE_NOT_FOUND;

	HRESULT result = graphics.getDevice()->CreateVertexShader((void*)shaderBytes, shaderSize, 0, shader);

	delete[] shaderBytes;
	return result;
}

HRESULT VertexShader::initForwardPBR(Graphics& graphics, ID3D11VertexShader** shader, ID3D11InputLayout** inputLayout) {
	D3D11_INPUT_ELEMENT_DESC inputs[5] = { // vertex data in slot 0
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	return init(graphics, shader, inputLayout, inputs, 5, "ForwardPBRVS.cso");
}

HRESULT VertexShader::initForwardPBR(Graphics& graphics, ID3D11VertexShader** shader) {
	return init(graphics, shader, "ForwardPBRVS.cso");
}

HRESULT VertexShader::initSkyBox(Graphics& graphics, ID3D11VertexShader** shader, ID3D11InputLayout** inputLayout) {
	D3D11_INPUT_ELEMENT_DESC inputs[1] = { // vertex data in slot 1
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	return init(graphics, shader, inputLayout, inputs, 1, "SkyBoxVS.cso");
}

HRESULT VertexShader::initSkyBox(Graphics& graphics, ID3D11VertexShader** shader) {
	return init(graphics, shader, "SkyBoxVS.cso");
}
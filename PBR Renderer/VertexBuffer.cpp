#include "VertexBuffer.h"

HRESULT VertexBuffer::init(Graphics& graphics, ID3D11Buffer** buffer, void* vertices, unsigned size) {
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = size;
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data{};
	data.pSysMem = vertices;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	return graphics.getDevice()->CreateBuffer(&bufferDesc, &data, buffer);
}
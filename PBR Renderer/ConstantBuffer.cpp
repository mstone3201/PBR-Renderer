#include "ConstantBuffer.h"

HRESULT ConstantBuffer::init(Graphics& graphics, ID3D11Buffer** buffer, unsigned size) {
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = size;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	return graphics.getDevice()->CreateBuffer(&bufferDesc, 0, buffer);
}

HRESULT ConstantBuffer::setData(Graphics& graphics, ID3D11Buffer* buffer, void* data, unsigned size) {
	D3D11_MAPPED_SUBRESOURCE res;
	HRESULT result = graphics.getDeviceContext()->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &res);
	if(SUCCEEDED(result)) {
		memcpy(res.pData, data, size);
		graphics.getDeviceContext()->Unmap(buffer, 0);
	}
	return result;
}
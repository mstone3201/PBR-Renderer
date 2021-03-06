#include "DDSParser.h"

HRESULT readDDSFile(std::string fileName, DDS_HEADER& header, DDS_HEADER_DXT10& header10, unsigned char*& data) {
	HRESULT result = ERROR_FILE_NOT_FOUND;

	DWORD dwMagic;

	FILE* file;
	fopen_s(&file, fileName.c_str(), "rb");
	if(file) {
		fread(&dwMagic, sizeof(DWORD), 1, file);
		if(dwMagic == 0x20534444) { // see if file starts with "DDS "
			fread(&header, sizeof(DDS_HEADER), 1, file);
			if(header.ddspf.dwFlags & DDPF_FOURCC && header.ddspf.dwFourCC == 0x30315844) { // see if file has DXT10 header: dwFourCC == "DX10"
				fread(&header10, sizeof(DDS_HEADER_DXT10), 1, file);

				fseek(file, 0, SEEK_END);
				size_t size = ftell(file) - sizeof(DWORD) - sizeof(DDS_HEADER) - sizeof(DDS_HEADER_DXT10); // pixel data bytes
				data = new unsigned char[size];
				fseek(file, sizeof(DWORD) + sizeof(DDS_HEADER) + sizeof(DDS_HEADER_DXT10), SEEK_SET);
				fread(data, 1, size, file);

				result = S_OK;
			} else result = E_INVALIDARG;
		} else result = E_INVALIDARG;

		fclose(file);
	}

	return result;
}

HRESULT readDDSFiles(std::string* fileNames, unsigned numFiles, DDS_HEADER& header, DDS_HEADER_DXT10& header10, unsigned char*& data) {
	HRESULT result = ERROR_FILE_NOT_FOUND;

	DWORD dwMagic;

	FILE* file;
	size_t size = 0;
	fopen_s(&file, fileNames[0].c_str(), "rb");
	if(file) {
		fread(&dwMagic, sizeof(DWORD), 1, file);
		if(dwMagic == 0x20534444) { // see if file starts with "DDS "
			fread(&header, sizeof(DDS_HEADER), 1, file);
			if(header.ddspf.dwFlags & DDPF_FOURCC && header.ddspf.dwFourCC == 0x30315844) { // see if file has DXT10 header: dwFourCC == "DX10"
				fread(&header10, sizeof(DDS_HEADER_DXT10), 1, file);

				fseek(file, 0, SEEK_END);
				size = ftell(file) - sizeof(DWORD) - sizeof(DDS_HEADER) - sizeof(DDS_HEADER_DXT10); // pixel data bytes
				data = new unsigned char[size * numFiles];
				fseek(file, sizeof(DWORD) + sizeof(DDS_HEADER) + sizeof(DDS_HEADER_DXT10), SEEK_SET);
				fread(data, 1, size, file);

				result = S_OK;
			} else result = E_INVALIDARG;
		} else result = E_INVALIDARG;

		fclose(file);
	}

	if(FAILED(result)) return result;

	for(unsigned i = 1; i < numFiles && SUCCEEDED(result); i++) {
		fopen_s(&file, fileNames[i].c_str(), "rb");
		if(file) {
			fread(&dwMagic, sizeof(DWORD), 1, file);
			if(dwMagic == 0x20534444) { // see if file starts with "DDS "
				fread(&header, sizeof(DDS_HEADER), 1, file);
				if(header.ddspf.dwFlags & DDPF_FOURCC && header.ddspf.dwFourCC == 0x30315844) { // see if file has DXT10 header: dwFourCC == "DX10"
					fseek(file, sizeof(DDS_HEADER_DXT10), SEEK_CUR);
					fread(data + size * i, 1, size, file);

					result = S_OK;
				} else result = E_INVALIDARG;
			} else result = E_INVALIDARG;

			fclose(file);
		} else result = ERROR_FILE_NOT_FOUND;

		if(FAILED(result)) return result;
	}

	return result;
}
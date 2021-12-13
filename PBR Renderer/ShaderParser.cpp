#include "ShaderParser.h"

size_t readShaderFile(std::string fileName, unsigned char*& buffer) {
	FILE* file;
	fopen_s(&file, fileName.c_str(), "rb");
	if(file) {
		fseek(file, 0, SEEK_END);
		size_t size = ftell(file);
		buffer = new unsigned char[size];
		fseek(file, 0, SEEK_SET);
		fread(buffer, 1, size, file);
		fclose(file);
		return size;
	}
	return 0;
}
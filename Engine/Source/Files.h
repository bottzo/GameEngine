#ifndef FILES
#define FILES

bool FileToBuffer(const char* filePath, char** buffer);
unsigned int CreateProgram(const char* vShaderPath, const char* fShaderPath);

#endif // !FILES
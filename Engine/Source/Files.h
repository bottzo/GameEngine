#ifndef FILES
#define FILES

long FileToBuffer(const char* filePath, char** buffer);
unsigned int CreateProgram(const char* vShaderPath, const char* fShaderPath);
bool LoadGLTFModel(const char* assetPath);

#endif // !FILES
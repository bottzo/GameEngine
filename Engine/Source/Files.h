#ifndef FILES
#define FILES

long FileToBuffer(const char* filePath, char** buffer);
unsigned int CreateProgram(const char* vShaderPath, const char* fShaderPath);

#include <vector>
bool LoadGLTFModel(const char* assetPath, std::vector<Mesh*>& out);
namespace tinygltf{
	class Model;
	struct Mesh;
	struct Primitive;
	struct Accessor;
}
class Mesh {
	unsigned int VBOEBO[2];
	unsigned int materialIdx;

	void LoadBufferData(const tinygltf::Model& model, const tinygltf::Accessor** accessors, const unsigned int numAccessors, char* ptr);

public:
	void Load(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive);
};
bool LoadGLTFModel(const char* assetPath, std::vector<Mesh>& out);

#endif // !FILES
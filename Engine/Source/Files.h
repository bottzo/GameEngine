#ifndef FILES
#define FILES

long FileToBuffer(const char* filePath, char** buffer);
unsigned int CreateProgram(const char* vShaderPath, const char* fShaderPath);

#include <vector>
namespace tinygltf {
	class Model;
	struct Mesh;
	struct Primitive;
	struct Accessor;
}
typedef unsigned int GLenum;
class Mesh {
	unsigned int VBOEBO[2];
	unsigned int vertexSize = 0;
	unsigned int VAO;
	unsigned int texIdx = 0;
	unsigned int numIndices = 0;
	unsigned int programId;
	GLenum indexType;

	void LoadBufferData(const tinygltf::Model& model, const tinygltf::Accessor** accessors, const unsigned int numAccessors, char* ptr);

public:
	void Load(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive);
	void Draw();
	void GenerateTangents();
};
bool LoadGLTFModel(const char* assetPath, std::vector<Mesh*>& out);

void GenerateTangents(unsigned int indexType, unsigned int VBOEBO[1], unsigned int numIndices, unsigned int vertexSize);

#endif // !FILES
#include "Model.h"
#include "Renderer.h"

#include <assimp/Importer.hpp>
#include <assimp/PostProcess.h>
#include <assimp/Scene.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp> 
#include <GL/glew.h>

Model::Model(const std::string & fileName) : Component("Model")
{
	data.SetType(GL_TRIANGLES);

	// Create model importer
	Assimp::Importer loadModel;
	// Read in the model data 
	const aiScene *model = loadModel.ReadFile(fileName, aiProcess_Triangulate
		| aiProcess_GenSmoothNormals
		| aiProcess_ValidateDataStructure
		| aiProcess_FindInvalidData
		| aiProcess_FixInfacingNormals
		| aiProcess_ImproveCacheLocality
		| aiProcess_GenUVCoords);
	// Check that data has been read in correctly
	if (!model)
	{
		// Display error
		std::fprintf(stderr, "Data incorrectly read in at $s", fileName);
		std::fprintf(stderr, loadModel.GetErrorString());
		// Throw exception
		throw std::runtime_error("Error reading in model file");
	}
	// Vectors to store model data.

	unsigned int vertex_begin = 0;
	// Loop throw each sub-mesh.
	for (unsigned int i = 0; i < model->mNumMeshes; i++)
	{
		// Get the sub-mesh.
		aiMesh *modelMesh = model->mMeshes[i];
		// get the vertex positions.

		for (unsigned int j = 0; j < modelMesh->mNumVertices; j++)
		{
			aiVector3D pos = modelMesh->mVertices[j];
			data.InsertVertex(glm::vec3(pos.x, pos.y, pos.z));
			aiVector3D norm = modelMesh->mNormals[j];
			data.InsertNormal(glm::vec3(norm.x, norm.y, norm.z));
			if (modelMesh->HasVertexColors(0))
			{
				aiColor4D colour = modelMesh->mColors[0][j];
				data.InsertColour(glm::vec4(colour.r, colour.g, colour.b, colour.a));
			}
			else
				data.InsertColour(glm::vec4(0.7, 0.7, 0.7, 1.0));
			auto texCoord = modelMesh->mTextureCoords[0][j];
			data.InsertTextureCoordinate(glm::vec2(texCoord.x, texCoord.y));
		}

		// If we have face information, then add to index buffer
		if (modelMesh->HasFaces() == true)
		{
			for (unsigned int j = 0; j < modelMesh->mNumFaces; j++)
			{
				aiFace modelFace = modelMesh->mFaces[j];
				for (GLuint l = 0; l < 3; l++)
				{
					data.InsertIndex(vertex_begin + modelFace.mIndices[l]);
				}
			}
		}
		vertex_begin += modelMesh->mNumVertices;
	}

	data.InitialiseMesh();
}

Model::~Model()
{
}

void Model::SetShader(GLShader& shader)
{
	data.SetShader(&shader);
}

void Model::Update(double deltaTime)
{
	data.SetTransform(GetTransform());
	data.UpdateTransforms();
}

void Model::Render()
{
	Renderer::Get().AddMesh(data);
}

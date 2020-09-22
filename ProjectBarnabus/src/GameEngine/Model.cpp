#include "Model.h"
#include "Renderer.h"
#include "BarnabusGameEngine.h"
#include <assimp/Importer.hpp>
#include <assimp/PostProcess.h>
#include <assimp/Scene.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp> 
#include <GL/glew.h>


namespace
{
	inline glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4 &aiMat)
	{
		return {
		aiMat.a1, aiMat.b1, aiMat.c1, aiMat.d1,
		aiMat.a2, aiMat.b2, aiMat.c2, aiMat.d2,
		aiMat.a3, aiMat.b3, aiMat.c3, aiMat.d3,
		aiMat.a4, aiMat.b4, aiMat.c4, aiMat.d4
		};
	}

	void LoadMesh(MeshData& mesh, aiMesh* modelMesh, unsigned int& vertexBegin)
	{
		mesh.SetType(GL_TRIANGLES);
		mesh.ResizeBoneData(modelMesh->mNumVertices);
		// get the vertex positions.
		for (unsigned int j = 0; j < modelMesh->mNumVertices; j++)
		{
			Vertex vert;

			aiVector3D pos = modelMesh->mVertices[j];
			vert.position = glm::vec3(pos.x, pos.y, pos.z);

			aiVector3D norm = modelMesh->mNormals[j];
			vert.normal = glm::vec3(norm.x, norm.y, norm.z);

			if (modelMesh->HasVertexColors(0))
			{
				aiColor4D colour = modelMesh->mColors[0][j];
				vert.color = glm::vec4(colour.r, colour.g, colour.b, colour.a);
			}
			else
			{
				vert.color = glm::vec4(0.7, 0.7, 0.7, 1.0);
			}

			auto texCoord = modelMesh->mTextureCoords[0][j];
			vert.texCoords = glm::vec2(texCoord.x, texCoord.y);

			mesh.InsertVertex(vert);
		}

		// If we have face information, then add to index buffer
		if (modelMesh->HasFaces() == true)
		{
			for (unsigned int j = 0; j < modelMesh->mNumFaces; j++)
			{
				aiFace modelFace = modelMesh->mFaces[j];
				for (GLuint l = 0; l < 3; l++)
				{
					mesh.InsertIndex(vertexBegin + modelFace.mIndices[l]);
				}
			}
		}

		// todo If the mesh has a texture associated with the mesh, use it here.

		if (!BarnabusGameEngine::Get().HasTexture("test"))
		{
			auto texture = std::make_unique<Texture>();
			texture->LoadTexture("res\\Textures\\test.png");
			BarnabusGameEngine::Get().AddTexture("test", std::move(texture));
		}

		mesh.SetTexture(BarnabusGameEngine::Get().GetTexture("test"));
		vertexBegin += modelMesh->mNumVertices;

	}

	void ParseNodes(Node*& meshRootNode, aiNode* rootNode, Node* parent, const aiScene* scene)
	{
		meshRootNode = new Node;
		meshRootNode->transformation = aiMatrix4x4ToGlm(rootNode->mTransformation);
		meshRootNode->name = rootNode->mName.C_Str();
		meshRootNode->parent = parent;
		meshRootNode->children.resize(rootNode->mNumChildren);
		meshRootNode->data.resize(rootNode->mNumMeshes);

		unsigned int vertexBegin = 0;
		for (unsigned int i = 0; i < rootNode->mNumMeshes; i++)
		{
			auto index = rootNode->mMeshes[i];
			aiMesh* mesh = scene->mMeshes[index];
			LoadMesh(meshRootNode->data[i], mesh, vertexBegin);
		}

		for (unsigned int i = 0; i < rootNode->mNumChildren; i++)
		{
			ParseNodes(meshRootNode->children[i], rootNode->mChildren[i], meshRootNode, scene);
		}
	}

	void SetNodeShader(Node*& meshRootNode, IShader* shader)
	{
		for (auto& mesh : meshRootNode->data)
		{
			mesh.SetShader(shader);
		}

		for (auto& child : meshRootNode->children)
		{
			SetNodeShader(child, shader);
		}
	}

	void SetNodeMaterial(Node*& meshRootNode, Material material)
	{
		for (auto& mesh : meshRootNode->data)
		{
			mesh.SetMaterial(material);
		}

		for (auto& child : meshRootNode->children)
		{
			SetNodeMaterial(child, material);
		}
	}

	void InitModelNodes(Node*& meshRootNode)
	{
		for (auto& mesh : meshRootNode->data)
		{
			mesh.InitialiseMesh();
		}

		for (auto& child : meshRootNode->children)
		{
			InitModelNodes(child);
		}
	}

	void UpdateNodes(Node*& meshRootNode, float deltaTime, glm::mat4 parentTransform)
	{
		for (auto& mesh : meshRootNode->data)
		{
			mesh.SetTransform(parentTransform * meshRootNode->transformation);
		}

		for (auto& child : meshRootNode->children)
		{
			UpdateNodes(child, deltaTime, parentTransform * meshRootNode->transformation);
		}
	}

	void RenderNodes(Node*& meshRootNode, std::string environmentName)
	{
		for (auto& mesh : meshRootNode->data)
		{
			Renderer::Get().AddMesh(environmentName, mesh);
		}

		for (auto& child : meshRootNode->children)
		{
			RenderNodes(child, environmentName);
		}
	}

} // namespace

Model::Model() : Component("Model")
{
}

Model::Model(const std::string& fileName) : Model()
{
	// Create model importer
	Assimp::Importer loadModel;
	// Read in the model data 
	const aiScene* model = loadModel.ReadFile(fileName, aiProcess_Triangulate
		| aiProcess_FlipUVs
		| aiProcess_JoinIdenticalVertices
	);
	// Check that data has been read in correctly
	if (!model)
	{
		BarnabusGameEngine::Get().AddMessageLog(StringLog("Data incorrectly read in at " + fileName, StringLog::Priority::Critical));
		BarnabusGameEngine::Get().AddMessageLog(StringLog(loadModel.GetErrorString(), StringLog::Priority::Critical));
		throw std::runtime_error("Error reading in model file");
	}

	ParseNodes(rootNode, model->mRootNode, NULL, model);
}

void Model::SetShader(IShader* shader)
{
	assert(rootNode);
	SetNodeShader(rootNode, shader);
}

void Model::SetMaterial(Material mat)
{
	assert(rootNode);
	SetNodeMaterial(rootNode, mat);
}

void Model::InitModel()
{
	assert(rootNode);
	InitModelNodes(rootNode);
}

const Node * Model::GetRootNode()
{
	return rootNode;
}

void Model::Update(float deltaTime)
{
	assert(rootNode);
	UpdateNodes(rootNode, deltaTime, GetTransform());
}

void Model::Render()
{
	assert(rootNode);
	RenderNodes(rootNode, GetParent()->GetEnvironmentName());
}

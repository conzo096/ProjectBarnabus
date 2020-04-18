#pragma once
#include "Entity.h"
#include "MeshData.h"

#include "GLShader.h"

class MeshNode
{
public:
	std::string name;
	std::vector<MeshNode*> children;
	std::vector<MeshData> data;
	glm::mat4 transformation;
	MeshNode* parent;
};

class Model : public Component
{
public:
	Model();
	Model(const std::string& fileName);

	void SetShader(GLShader& shader);
	void SetMaterial(Material mat);
	void InitModel();

	void Update(float deltaTime) override;
	void Render() override;

	std::vector<MeshData> data;

protected:
	MeshNode* rootMeshNode;
};
#pragma once
#include "Entity.h"
#include "Node.h"
#include "GLShader.h"

class Model : public Component
{
public:
	Model();
	Model(const std::string& fileName);

	void SetShader(GLShader& shader);
	void SetMaterial(Material mat);
	void InitModel();
	const MeshData& GetMesh();

public:
	void Update(float deltaTime) override;
	void Render() override;

protected:
	Node* rootNode;
};
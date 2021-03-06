#pragma once
#include "Entity.h"
#include "Node.h"
#include "GLShader.h"

class Model : public Component
{
public:
	Model();
	Model(const std::string& fileName);

	void SetShader(IShader* shader);
	void SetMaterial(Material mat);
	void InitModel();
	const Node* GetRootNode();

public:
	void Update(float deltaTime) override;
	void Render() override;

protected:
	Node* rootNode;
};
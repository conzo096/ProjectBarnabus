#pragma once
#include "Entity.h"
#include "MeshData.h"

#include "GLShader.h"

class Model : public Component
{
public:
	Model();
	Model(const std::string& fileName);
	~Model();

	void SetShader(GLShader& shader);
	void InitModel();

	void Update(double deltaTime) override;
	void Render() override;

	std::vector<MeshData> data;
};
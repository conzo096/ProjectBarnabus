#include "BoundingVolumes.h"
#include "Model.h"

namespace BoundingVolumes
{

namespace
{

void AddBoundingBoxes(const Node* node, std::vector<BoundingBox>& boundingBoxes)
{
	for (unsigned int i = 0; i < node->data.size(); i++)
	{
		boundingBoxes.push_back(BoundingBox(node->data[i].vertices, node->transformation));
	}

	for (unsigned int i = 0; i < node->children.size(); i++)
	{
		AddBoundingBoxes(node->children[i], boundingBoxes);
	}
}
}


BoundingVolumes::BoundingVolumes::BoundingVolumes() : Component("BoundingVolumes")
{
}
	
void BoundingVolumes::SetShader(GLShader & shader)
{
	for (auto& bb : boundingBoxes)
	{
		bb.SetShader(shader);
	}
}
	
void BoundingVolumes::InitMeshes()
{
	for (auto& bb : boundingBoxes)
	{
		bb.InitMesh();
	}
}

void BoundingVolumes::AddBoundingVolumes(const Node * rootNode)
{
	AddBoundingBoxes(rootNode, boundingBoxes);
}

void BoundingVolumes::AddBoundingVolumes(glm::vec3 minCoords, glm::vec3 maxCoords)
{
	boundingBoxes.push_back(BoundingBox(minCoords, maxCoords));
}

void BoundingVolumes::AddBoundingVolumes(const std::vector<Vertex>& vertices, glm::mat4 trans)
{
	boundingBoxes.push_back(BoundingBox(vertices, trans));
}

void BoundingVolumes::Update(float deltaTime)
{
	for (auto& bb : boundingBoxes)
	{
		bb.SetTransform(GetTransform());
		bb.Update(deltaTime);
	}
}

void BoundingVolumes::Render()
{
	for (auto& bb : boundingBoxes)
	{
		bb.Render(GetParent()->GetEnvironmentName());
	}
}
}

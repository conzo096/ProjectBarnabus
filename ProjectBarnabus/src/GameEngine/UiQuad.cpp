#include "UiQuad.h"
#include <GL\glew.h>
#include <GL/GL.h>

#include "Renderer.h"

UiQuad::UiQuad(): width(1),height(1), transparency(1.0f)
{
	mesh.SetType(GL_QUADS);
	std::vector<glm::vec3> positions
	{
		glm::vec3(1.0f, 1.0f, 0.0f),
		glm::vec3(-1.0f, 1.0f, 0.0f),
		glm::vec3(-1.0f, -1.0f, 0.0f),
		glm::vec3(1.0f, -1.0f, 0.0f)
	};

	std::vector<glm::vec2> tex_coords
	{
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f),
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f)
	};

	for (int i = 0; i < 4; i++)
	{
		Vertex v;
		v.position = positions.at(i);
		v.texCoords = tex_coords.at(i);

		mesh.InsertVertex(v);
	}
}

UiQuad::UiQuad(glm::vec2 bottomLeft, glm::vec2 topRight) : transparency(1.0f)
{
	mesh.SetType(GL_QUADS);

	width = topRight.x - bottomLeft.x;
	height = topRight.y - bottomLeft.y;
	
	std::vector<glm::vec3> positions
	{
		glm::vec3(topRight.x, topRight.y, 0.0f),
		glm::vec3(bottomLeft.x, topRight.y, 0.0f),
		glm::vec3(bottomLeft.x, bottomLeft.y, 0.0f),
		glm::vec3(topRight.x, bottomLeft.y, 0.0f)
	};

	std::vector<glm::vec2> tex_coords
	{
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f),
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f)
	};

	for (int i = 0; i < 4; i++)
	{
		Vertex v;
		v.position = positions.at(i);
		v.texCoords = tex_coords.at(i);

		mesh.InsertVertex(v);
	}
}

UiQuad::~UiQuad()
{
}

MeshData& UiQuad::GetMeshData()
{
	return mesh;
}

void UiQuad::InitQuad()
{
	shader.CreateProgram();
	shader.AddShaderFromFile("..\\ProjectBarnabus\\res\\shaders\\UI.vert", GLShader::VERTEX);
	shader.AddShaderFromFile("..\\ProjectBarnabus\\res\\shaders\\UI.frag", GLShader::FRAGMENT);
	shader.Link();

	mesh.SetShader(&shader);
	mesh.InitialiseMesh();
}

void UiQuad::Draw()
{
	Renderer::Get().AddUiElement(mesh);
}
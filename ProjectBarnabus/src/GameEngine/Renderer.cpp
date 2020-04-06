#include "Renderer.h"
#include "BarnabusGameEngine.h"
#include "GLShader.h"
#include "UiQuad.h"

Renderer::Renderer()
{
	backgroundColour = glm::vec4(0.1f, 0.0f, 0.4f, 1.0f);
	auto main = new FrameBuffer;
	main->LoadFrameBuffer(1920,1080);
	auto pair = std::pair<std::string, FrameBuffer*>(std::string("main"), main);
	AddFramebuffer(pair);
	auto ui = new FrameBuffer;
	ui->LoadFrameBuffer(1920, 1080);
	pair = std::pair<std::string, FrameBuffer*>(std::string("ui"), ui);
	AddFramebuffer(pair);

	screenQuad = new UiQuad(glm::vec2(-1,-1), glm::vec2(1, 1));
	screenQuad->InitQuad();

	shader.CreateProgram();
	shader.AddShaderFromFile("res\\shaders\\FinalPass.vert", GLShader::VERTEX);
	shader.AddShaderFromFile("res\\shaders\\FinalPass.frag", GLShader::FRAGMENT);
	shader.Link();

	shader.gameTexture = GetFrameBuffer("main").GetFrameTexture();
	shader.uiTexture = GetFrameBuffer("ui").GetFrameTexture();
}

Renderer::~Renderer()
{

}

void Renderer::Render()
{
	auto renderMeshes = [](auto& renderList)
	{
		for (int i = 0; i < renderList.size(); i++)
		{
			auto& mesh = renderList[i];
			mesh.GetShader()->UpdateUniforms(mesh);
			// Bind and draw model.
			mesh.GetShader()->DrawMesh(mesh);
			glBindVertexArray(0);
		}
	};

	auto renderMeshesWithLights = [](auto& renderList, auto& lights)
	{
		for (int i = 0; i < renderList.size(); i++)
		{
			auto& mesh = renderList[i];
			mesh.GetShader()->UpdateUniforms(mesh, lights);
			// Bind and draw model.
			mesh.GetShader()->DrawMesh(mesh);
			glBindVertexArray(0);
		}
	};

	// Render main game.
	GetFrameBuffer("main").BindFrameBuffer();
	glClearColor(backgroundColour.x,backgroundColour.y,backgroundColour.z,backgroundColour.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	for (auto& meshes : meshesToRender)
	{
		auto lights = environmentLights.find(meshes.first);
		if (lights == environmentLights.end())
		{
			renderMeshes(meshes.second);
		}
		else
		{
			renderMeshesWithLights(meshes.second, lights->second);
		}
	}
	meshesToRender.clear();


	// render to screen
	GetFrameBuffer("ui").BindFrameBuffer();
	glClearColor(backgroundColour.x, backgroundColour.y, backgroundColour.z,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	renderMeshes(uiElementsToRender);
	uiElementsToRender.clear();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	shader.UpdateUniforms(screenQuad->GetMeshData());
	shader.DrawMesh(screenQuad->GetMeshData());

	glfwSwapBuffers(BarnabusGameEngine::Get().GetWindow());

	environmentLights.clear();

}

void Renderer::SetCameraViewProjection(glm::mat4 camera)
{
	cameraVP = camera;
}

void Renderer::AddMesh(std::string environmentName, MeshData md)
{
	auto environmentMeshes = meshesToRender.find(environmentName);

	// enviroment does not exist. Add a new vector to list
	if (environmentMeshes == meshesToRender.end())
	{
		std::vector<MeshData> newList;
		newList.push_back(md);
		meshesToRender.insert(std::pair<std::string, std::vector<MeshData>>(environmentName, newList));
	}
	else
	{
		environmentMeshes->second.push_back(md);
	}
}

void Renderer::AddLight(std::string environmentName, Light* light)
{
	auto lights = environmentLights.find(environmentName);

	// enviroment does not exist. Add a new vector to list
	if (lights == environmentLights.end())
	{
		std::vector<Light*> newList;
		newList.push_back(light);
		environmentLights.insert(std::pair<std::string, std::vector<Light*>>(environmentName, newList));
	}
	else
	{
		lights->second.push_back(light);
	}
}

glm::mat4 Renderer::GetCameraVP()
{
	return cameraVP;
}

void Renderer::AddUiElement(MeshData md)
{
	uiElementsToRender.push_back(md);
}

void Renderer::AddFramebuffer(std::pair<std::string, FrameBuffer*> pair)
{
	framebuffers.insert(pair);
}

FrameBuffer& Renderer::GetFrameBuffer(const std::string& buffer)
{
	return *framebuffers.at(buffer);
}

glm::vec4 Renderer::GetBackgroundColour()
{
	return backgroundColour;
}

void Renderer::SetBackgroundColour(glm::vec4 colour)
{
	backgroundColour = colour;
}

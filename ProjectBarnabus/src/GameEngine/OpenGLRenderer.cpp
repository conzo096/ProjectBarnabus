#include "OpenGLRenderer.h"
#include "BarnabusGameEngine.h"
#include "OpenGLFramebuffer.h"
#include "OpenGLTexture.h"
#include "FinalPassShader.h"
#include "UiQuad.h"

#include <GL\glew.h>

namespace
{
	enum BUFFERS { POSITION, COLOR, NORMAL, TEX_COORD, BONES, BONE_ID, BONE_WEIGHT };
}

OpenGLRenderer::OpenGLRenderer()
{
}

OpenGLRenderer::~OpenGLRenderer()
{
}

bool OpenGLRenderer::InitialiseGameEngine()
{
	if (!glfwInit())
	{
		BarnabusGameEngine::Get().AddMessageLog(StringLog("ERROR: glfw failed init! exiting.", StringLog::Priority::Critical));
		return false;
	}

	window = glfwCreateWindow(1920, 1080, "Testing", NULL, NULL);

	// Window is now initalised, now make it the current context.
	glfwMakeContextCurrent(window);

	if (!window)
	{
		assert(window != NULL);
		BarnabusGameEngine::Get().AddMessageLog(StringLog("Error: Window could not be initialised!", StringLog::Priority::Critical));
		return false;
	}

	// Set up glew.
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		BarnabusGameEngine::Get().AddMessageLog(StringLog("ERROR: glew failed init!exiting.", StringLog::Priority::Critical));
		return false;
	}
	// glExperimental throws junk errors, Ignore.
	glGetError();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	// V-Sync, does not run without it
	glfwSwapInterval(1);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	backgroundColour = glm::vec4(0.1f, 0.0f, 0.4f, 1.0f);
	auto main = new OpenGLFrameBuffer("main");
	main->LoadFrameBuffer(1920, 1080);
	auto pair = std::pair<std::string, OpenGLFrameBuffer*>(std::string("main"), main);
	AddFramebuffer(pair);
	auto ui = new OpenGLFrameBuffer("ui");
	ui->LoadFrameBuffer(1920, 1080);
	pair = std::pair<std::string, OpenGLFrameBuffer*>(std::string("ui"), ui);
	AddFramebuffer(pair);

	finalShader = new FinalPassShader;
	finalShader->CreateProgram("FinalRender");
	finalShader->AddShaderFromFile("res\\shaders\\FinalPass.vert", GLShader::VERTEX);
	finalShader->AddShaderFromFile("res\\shaders\\FinalPass.frag", GLShader::FRAGMENT);
	finalShader->Link();

	screenQuad = new UiQuad(glm::vec2(-1, -1), glm::vec2(1, 1));
	screenQuad->InitQuad();

	static_cast<FinalPassShader*>(finalShader)->gameTexture = GetFrameBuffer("main")->GetFrameTexture();
	static_cast<FinalPassShader*>(finalShader)->uiTexture = GetFrameBuffer("ui")->GetFrameTexture();

	return true;
}

bool OpenGLRenderer::ShouldWindowClose()
{
	return glfwWindowShouldClose(window);
}

GLFWwindow * OpenGLRenderer::GetWindow()
{
	return window;
}

IRenderer::GraphicsRenderer OpenGLRenderer::GetRenderType()
{
	return IRenderer::OpenGL;
}

void OpenGLRenderer::InitialiseMesh(MeshData& data)
{
	glGenVertexArrays(1, &data.GetVao());
	glGenBuffers(1, &data.GetVbo());
	glGenBuffers(1, &data.GetEbo());

	glBindVertexArray(data.GetVao());

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.GetEbo());
	if (data.GetIndices().size() > 0)
	{
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.GetIndices().size() * sizeof(unsigned int),
			&data.GetIndices()[0], GL_STATIC_DRAW);
	}

	UpdateBaseVertexBuffers(data);

	if (data.GetBoneData().size() > 0)
	{
		// vertex bone information
		glGenBuffers(1, &data.GetBones());
		glBindBuffer(GL_ARRAY_BUFFER, data.GetBones());
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexBoneData) * data.GetBoneData().size(), &data.GetBoneData()[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(BUFFERS::BONE_ID);
		glVertexAttribIPointer(BUFFERS::BONE_ID, 4, GL_INT, sizeof(VertexBoneData), (void*)offsetof(VertexBoneData, boneIds));

		glEnableVertexAttribArray(BUFFERS::BONE_WEIGHT);
		glVertexAttribPointer(BUFFERS::BONE_WEIGHT, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (void*)offsetof(VertexBoneData, weights));
	}

	assert(glGetError() == GL_NO_ERROR);
}

void OpenGLRenderer::UpdateBaseVertexBuffers(MeshData& data)
{
	glBindVertexArray(data.GetVao());

	glBindBuffer(GL_ARRAY_BUFFER, data.GetVbo());
	glBufferData(GL_ARRAY_BUFFER, data.vertices.size() * sizeof(Vertex), &data.vertices[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(BUFFERS::POSITION);
	glVertexAttribPointer(BUFFERS::POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

	glEnableVertexAttribArray(BUFFERS::COLOR);
	glVertexAttribPointer(BUFFERS::COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

	// vertex normals
	glEnableVertexAttribArray(BUFFERS::NORMAL);
	glVertexAttribPointer(BUFFERS::NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	// vertex texture coords
	glEnableVertexAttribArray(BUFFERS::TEX_COORD);
	glVertexAttribPointer(BUFFERS::TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLRenderer::Render()
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
	GetFrameBuffer("main")->BindFrameBuffer();
	glClearColor(backgroundColour.x, backgroundColour.y, backgroundColour.z, backgroundColour.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	for (auto& meshes : meshesToRender)
	{
		renderMeshes(meshes.second);
	}

	meshesToRender.clear();

	// render to screen
	GetFrameBuffer("ui")->BindFrameBuffer();
	glClearColor(backgroundColour.x, backgroundColour.y, backgroundColour.z, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	renderMeshes(uiElementsToRender);
	uiElementsToRender.clear();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	finalShader->UpdateUniforms(screenQuad->GetMeshData());
	finalShader->DrawMesh(screenQuad->GetMeshData());

	glfwSwapBuffers(BarnabusGameEngine::Get().GetWindow());

	environmentLights.clear();
}

void OpenGLRenderer::SetCameraViewProjection(glm::mat4 camera)
{
	cameraVP = camera;
}

void OpenGLRenderer::AddMesh(MeshData& md)
{
	auto environmentMeshes = meshesToRender.find(md.GetShader());

	// enviroment does not exist. Add a new vector to list
	if (environmentMeshes == meshesToRender.end())
	{
		std::vector<MeshData> newList;
		newList.push_back(md);
		meshesToRender.insert(std::pair<IShader*, std::vector<MeshData>>(md.GetShader(), newList));
	}
	else
	{
		environmentMeshes->second.push_back(md);
	}
}

void OpenGLRenderer::AddLight(Light* light)
{
}

glm::mat4 OpenGLRenderer::GetCameraVP()
{
	return cameraVP;
}

void OpenGLRenderer::AddUiElement(MeshData & md)
{
	uiElementsToRender.push_back(md);
}

void OpenGLRenderer::AddFramebuffer(std::pair<std::string, IFrameBuffer*> pair)
{
	framebuffers.insert(pair);
}

IFrameBuffer* OpenGLRenderer::GetFrameBuffer(const std::string& buffer)
{
	return framebuffers.at(buffer);
}

bool OpenGLRenderer::AddShader(std::string name, std::unique_ptr<IShader> shader)
{
	auto result = shaders.insert(std::pair < std::string, std::unique_ptr<IShader>>(name, std::move(shader)));
	return result.second;
}

IShader * OpenGLRenderer::GetShader(const std::string & shaderName)
{
	auto it = shaders.find(shaderName);
	assert(it != shaders.end());

	return it->second.get();
}

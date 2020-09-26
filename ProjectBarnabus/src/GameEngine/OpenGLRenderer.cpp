#include "OpenGLRenderer.h"
#include "BarnabusGameEngine.h"

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

#include "OpenGLFrameBuffer.h"
#include "OpenGLTexture.h"
#include "BarnabusGameEngine.h"

#include <glm\gtc\type_ptr.hpp>

OpenGLFrameBuffer::OpenGLFrameBuffer(const std::string & frameBufferName) : name(frameBufferName)
{
}

OpenGLFrameBuffer::~OpenGLFrameBuffer()
{
	glDeleteFramebuffers(1, &bufferId);
}

GLuint OpenGLFrameBuffer::GetFrameBuffer()
{
	return bufferId;
}

ITexture* OpenGLFrameBuffer::GetFrameTexture()
{
	return frameTexture;
}

ITexture* OpenGLFrameBuffer::GetDepthTexture()
{
	return depthTexture;
}

void OpenGLFrameBuffer::LoadFrameBuffer(int w, int h)
{
	// The draw buffer
	static GLenum drawBuffer = GL_COLOR_ATTACHMENT0;
	width = w;
	height = h;
	// Create textures with OpenGL
	frameTexture = new OpenGLTexture(width, height);
	depthTexture = new OpenGLTexture(width, height);

	// Bind image with OpenGL
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, frameTexture->GetTextureId());

	// Create the image data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	// Set texture properties
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthTexture->GetTextureId());
	// Create the depth image data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

	// Set texture properties
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));
	glGenFramebuffers(1, &bufferId);
	glBindFramebuffer(GL_FRAMEBUFFER, bufferId);


	// Attach the frame and depth textures
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameTexture->GetTextureId(), 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture->GetTextureId(), 0);

	// Set draw buffer
	glDrawBuffers(1, &drawBuffer);

	// Unbind frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	BarnabusGameEngine::Get().AddMessageLog(StringLog("Created Framebuffer: " + name, StringLog::Priority::Low));

}

void OpenGLFrameBuffer::BindFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, bufferId);
}

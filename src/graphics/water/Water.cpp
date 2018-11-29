#include "Water.h"
#include "../mesh/common/Quad.h"

namespace graphics {

  unsigned int createFrameBuffer();
  void bindFrameBuffer(unsigned int frameBuffer, int w, int h);
  void unbindFrameBuffer();
  unsigned int createTextureAttachment(int w, int h);
  unsigned int createDepthTextureAttachment(int w, int h);
  unsigned int createDepthBufferAttachment(int w, int h);

  Water::Water(glm::vec3 position, glm::vec3 scale, glm::vec3 rotationAxis, float radianRotation, FPSCamera *cam) : 
	m_Position(position), m_Scale(scale), 
	m_Orientation(glm::angleAxis(radianRotation, rotationAxis)), 
	m_WaterShader("res/shaders/water.vert", "res/shaders/water.frag"), m_Camera(cam){

	m_GLCache = GLCache::getInstance();

	m_Mesh = new Quad();

	// Reflection FrameBuffer
	m_ReflectionFrameBuffer = createFrameBuffer();
	m_ReflectionTexture = createTextureAttachment(REFLECTION_WIDTH, REFLECTION_HEIGHT);
	m_ReflectionDepthBuffer = createDepthBufferAttachment(REFLECTION_WIDTH, REFLECTION_HEIGHT);
	unbindFrameBuffer();

	// Refraction FrameBuffer
	m_RefractionFrameBuffer = createFrameBuffer();
	m_RefractionTexture = createTextureAttachment(REFRACTION_WIDTH, REFRACTION_HEIGHT);
	m_RefractionDepthTexture = createDepthTextureAttachment(REFRACTION_WIDTH, REFRACTION_HEIGHT);
	unbindFrameBuffer();
  }

  Water::~Water() {
	cleanUp();
	delete m_Mesh;
  }

  void Water::Draw() {
	m_GLCache->switchShader(m_WaterShader.getShaderID());
	m_GLCache->setDepthTest(true);
	m_GLCache->setBlend(true);
	m_GLCache->setStencilTest(false);
	m_GLCache->setFaceCull(false);
	m_GLCache->setClipDistance0(true);

	glm::mat4 m_ModelMatrix(1);
	glm::mat4 translate = glm::translate(m_ModelMatrix, m_Position);
	glm::mat4 rotate = glm::toMat4(m_Orientation);
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), m_Scale);
	m_ModelMatrix = translate * rotate * scale;

	m_WaterShader.setUniformMat4("model", m_ModelMatrix);
	m_WaterShader.setUniformMat4("view", m_Camera->getViewMatrix());
	m_WaterShader.setUniformMat4("projection", m_Camera->getProjectionMatrix());

	m_Mesh->Draw();
  }

  void Water::cleanUp() {
	glDeleteFramebuffers(1, &m_ReflectionFrameBuffer);
	glDeleteTextures(1, &m_ReflectionTexture);
	glDeleteRenderbuffers(1, &m_ReflectionDepthBuffer);
	glDeleteFramebuffers(1, &m_RefractionFrameBuffer);
	glDeleteTextures(1, &m_RefractionTexture);
	glDeleteTextures(1, &m_RefractionDepthTexture);
  }

  // Utility Functions to manage Framebuffers.
  unsigned int createFrameBuffer() {
	unsigned int framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	return framebuffer;
  }

  void bindFrameBuffer(unsigned int frameBuffer, int w, int h) {
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glViewport(0, 0, w, h);
  }

  void unbindFrameBuffer(){
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, WINDOW_X_RESOLUTION, WINDOW_Y_RESOLUTION);
  }

  unsigned int createTextureAttachment(int w, int h) {
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);
	return texture;
  }
  unsigned int createDepthTextureAttachment(int w, int h) {
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture, 0);
	return texture;
  }

  unsigned int createDepthBufferAttachment(int w, int h) {
	unsigned int depthBuffer;
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
	return depthBuffer;
  }

}

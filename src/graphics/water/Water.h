#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <vector>
#include <glm/gtx/norm.hpp>

#include "../renderer/RenderPass.h"
#include "../mesh/Mesh.h"
#include "../../utils/loaders/TextureLoader.h"
#include "../renderer/GLCache.h"
#include "../camera/FPSCamera.h"
#include "../Shader.h"
#include "../Window.h"
#include "../../Defs.h"

namespace graphics{
  class Water {
	public:
	  Water(glm::vec3 position, glm::vec3 scale, glm::vec3 rotationAxis, float radianRotation, FPSCamera *cam);
	  ~Water();

	  void Draw();

	  glm::vec3 getPosition() const { return m_Position; }
	  glm::vec3 getScale() const { return m_Scale; }
	  Mesh *getMesh() const { return m_Mesh; }

	private:
	  unsigned int m_ReflectionFrameBuffer, m_RefractionFrameBuffer;
	  unsigned int m_ReflectionTexture, m_RefractionTexture;
	  unsigned int m_ReflectionDepthBuffer, m_RefractionDepthTexture;

	  FPSCamera *m_Camera;
	  Shader m_WaterShader;

	  glm::vec3 m_Position;
	  glm::vec3 m_Scale;
	  glm::quat m_Orientation;

	  glm::mat4 m_ModelMatrix;
	  GLCache *m_GLCache;

	  Mesh *m_Mesh;

	  void cleanUp();

  };
}

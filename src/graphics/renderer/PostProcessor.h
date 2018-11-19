#pragma once

#include "../../Defs.h"
#include "../mesh/common/Quad.h"
#include "MeshRenderer.h"
#include "../Shader.h"
#include "../../platform/OpenGL/Framebuffers/RenderTarget.h"
#include "../../utils/Timer.h"
#include "../../ui/DebugPane.h"
#include "../../ui/RuntimePane.h"

namespace graphics {

  class PostProcessor {
	public:
	  PostProcessor(MeshRenderer *meshRenderer);
	  ~PostProcessor();

	  void preLightingPostProcess();
	  // Assumes the input RenderTarget is bound, after function runs the default screen RenderTarget is bound
	  void postLightingPostProcess(opengl::RenderTarget *input);

	  // Might be useful to have if we want to have more custom post processing. Unity does it this way
	  //void blit(Texture *texture, opengl::RenderTarget *source);

	  inline void EnableBlur(bool choice) { m_Blur = choice; }
	private:
	  float m_GammaCorrection = 2.2f;

	  MeshRenderer *m_MeshRenderer;
	  Shader m_PostProcessShader;
	  Quad m_NDC_Plane;
	  opengl::RenderTarget m_ScreenRenderTarget;
	  utils::Timer m_Timer;

	  bool m_Blur = false;
  };

}

#include "Scene3D.h"
#include <iterator>
#include <iostream>
#include <glm/glm.hpp>
#include <math.h>

#include "graphics/mesh/Mesh.h"
#include "graphics/mesh/common/Cube.h"
#include "graphics/mesh/common/Sphere.h"
#include "graphics/mesh/common/Quad.h"

// TODO: Replace new with smart pointers.

Scene3D::Scene3D(graphics::FPSCamera *camera, graphics::Window *window)
  : m_TerrainShader("res/shaders/terrain.vert", "res/shaders/terrain.frag"), m_ModelShader("res/shaders/pbr_model.vert", "res/shaders/pbr_model.frag"), m_Camera(camera),
  m_ShadowmapShader("res/shaders/shadowmap.vert", "res/shaders/shadowmap.frag"), m_DynamicLightManager(){
  m_MeshRenderer = new graphics::MeshRenderer(camera);
  m_GLCache = graphics::GLCache::getInstance();
  m_Terrain = new terrain::Terrain(glm::vec3(0.0f, -20.0f, 0.0f));
  m_Water = new graphics::Water(glm::vec3(555.0f, 50.0f, 350.0f), glm::vec3(60.0f, 60.0f, 60.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::radians(-90.0f), m_Camera);

  init();
}

Scene3D::~Scene3D() {
  delete m_MeshRenderer;
  delete m_Terrain;
  delete m_Water;
}

void Scene3D::init() {
  m_GLCache->setMultisample(true);

  graphics::Model *sponza = new graphics::Model("res/3D_Models/Sponza/sponza.obj");
  scene::SceneNode *sponzaNode = new scene::SceneNode(glm::vec3(200.0f, 50.0f, 100.0f), glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.0f, 0.0f, 1.0f), glm::radians(0.0f), sponza, nullptr, false);
  add(sponzaNode);

  //add(new scene::SceneNode(glm::vec3(40, 60, 40), glm::vec3(15, 15, 15), glm::vec3(0.0, 1.0, 0.0), glm::radians(180.0f), glass, nullptr, true));
  //add(new scene::SceneNode(glm::vec3(80, 60, 80), glm::vec3(15, 15, 15), glm::vec3(0.0, 1.0, 0.0), glm::radians(180.0f), glass, nullptr, true));
  //add(new scene::SceneNode(glm::vec3(120, 60, 120), glm::vec3(15, 15, 15), glm::vec3(0.0, 1.0, 0.0), glm::radians(180.0f), glass, nullptr, true));

  // NOTE: Tmp Code.
  graphics::Model *pbrGun = new graphics::Model("res/3D_Models/Cerberus_Gun/Cerberus_LP.FBX");
  add(new scene::SceneNode(glm::vec3(120.0f, 85.0f, 220.0f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::radians(-90.0f), pbrGun, nullptr, false));
  pbrGun->getMeshes()[0].getMaterial().setAlbedoMap(utils::TextureLoader::load2DTexture(std::string("res/3D_Models/Cerberus_Gun/Textures/Cerberus_A.tga"), true));
  pbrGun->getMeshes()[0].getMaterial().setNormalMap(utils::TextureLoader::load2DTexture(std::string("res/3D_Models/Cerberus_Gun/Textures/Cerberus_N.tga"), false));
  pbrGun->getMeshes()[0].getMaterial().setMetallicMap(utils::TextureLoader::load2DTexture(std::string("res/3D_Models/Cerberus_Gun/Textures/Cerberus_M.tga"), false));
  pbrGun->getMeshes()[0].getMaterial().setRoughnessMap(utils::TextureLoader::load2DTexture(std::string("res/3D_Models/Cerberus_Gun/Textures/Cerberus_R.tga"), false));
  pbrGun->getMeshes()[0].getMaterial().setAmbientOcclusionMap(utils::TextureLoader::load2DTexture(std::string("res/3D_Models/Cerberus_Gun/Textures/Cerberus_AO.tga"), false));

  // NOTE: Tmp Code.
  float spacing = 20.0f;
  for (int row = 0; row < 4; row++) {
	graphics::Model *sphere = new graphics::Model("res/3D_Models/Sphere/globe-sphere.obj");
	graphics::Material &mat = sphere->getMeshes()[0].getMaterial();
	mat.setAlbedoMap(utils::TextureLoader::load2DTexture(std::string("res/3D_Models/Sphere/rustediron2_basecolor.png"), true));
	mat.setNormalMap(utils::TextureLoader::load2DTexture(std::string("res/3D_Models/Sphere/rustediron2_normal.png"), false));
	mat.setAmbientOcclusionMap(utils::TextureLoader::load2DTexture(std::string("res/textures/default/white.png"), false));
	mat.setMetallicMap(utils::TextureLoader::load2DTexture(std::string("res/3D_Models/Sphere/rustediron2_metallic.png"), false));
	mat.setRoughnessMap(utils::TextureLoader::load2DTexture(std::string("res/3D_Models/Sphere/rustediron2_roughness.png"), false));
	add(new scene::SceneNode(glm::vec3((float)(140.0f + spacing * row), 67.0f, 270.0f + spacing * row / 7.0f), glm::vec3(8.0f, 8.0f, 8.0f), glm::vec3(1.0f, 0.0f, 0.0f), 0.0f, sphere, nullptr, false));
  }

  // Skybox
  std::vector<std::string> skyboxFilePaths;
  skyboxFilePaths.push_back("res/skybox/right.png");
  skyboxFilePaths.push_back("res/skybox/left.png");
  skyboxFilePaths.push_back("res/skybox/top.png");
  skyboxFilePaths.push_back("res/skybox/bottom.png");
  skyboxFilePaths.push_back("res/skybox/back.png");
  skyboxFilePaths.push_back("res/skybox/front.png");
  m_Skybox = new graphics::Skybox(skyboxFilePaths, m_Camera);
}

void Scene3D::shadowmapPass() {
  m_GLCache->setClipDistance0(false);
  glm::vec3 dirLightShadowmapLookAtPos = m_Camera->getPosition() + (glm::normalize(glm::vec3(m_Camera->getFront().x, 0.0f, m_Camera->getFront().z)) * 50.0f);
  glm::vec3 dirLightShadowmapEyePos = dirLightShadowmapLookAtPos + (-m_DynamicLightManager.getDirectionalLightDirection() * 100.0f);

  m_GLCache->switchShader(m_ShadowmapShader.getShaderID());
  glm::mat4 directionalLightProjection = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, SHADOWMAP_NEAR_PLANE, SHADOWMAP_FAR_PLANE);
  glm::mat4 directionalLightView = glm::lookAt(dirLightShadowmapEyePos, dirLightShadowmapLookAtPos, glm::vec3(0.0f, 1.0f, 0.0f));
  glm::mat4 directionalLightViewProjMatrix = directionalLightProjection * directionalLightView;
  m_ShadowmapShader.setUniformMat4("lightSpaceViewProjectionMatrix", directionalLightViewProjMatrix);

  addObjectsToRenderQueue();

  m_MeshRenderer->flushOpaque(m_ShadowmapShader, graphics::RenderPass::ShadowmapPass);
  m_MeshRenderer->flushTransparent(m_ShadowmapShader, graphics::RenderPass::ShadowmapPass);
  m_Terrain->Draw(m_ShadowmapShader, graphics::RenderPass::ShadowmapPass);

  // Setup shadow uniforms for other shaders so they can have normal mapping
  m_GLCache->switchShader(m_TerrainShader.getShaderID());
  m_TerrainShader.setUniformMat4("lightSpaceViewProjectionMatrix", directionalLightViewProjMatrix);

  m_GLCache->switchShader(m_ModelShader.getShaderID());
  m_ModelShader.setUniformMat4("lightSpaceViewProjectionMatrix", directionalLightViewProjMatrix);
}

void Scene3D::onRender(unsigned int shadowmap) {
  m_GLCache->setClipDistance0(false);
  // Setup
  glm::mat4 projectionMatrix = m_Camera->getProjectionMatrix();

  m_DynamicLightManager.setSpotLightDirection(m_Camera->getFront());
  m_DynamicLightManager.setSpotLightPosition(m_Camera->getPosition());

  // Models
  m_GLCache->switchShader(m_ModelShader.getShaderID());
  m_DynamicLightManager.setupLightingUniforms(m_ModelShader);
  m_ModelShader.setUniform3f("viewPos", m_Camera->getPosition());
  m_ModelShader.setUniformMat4("view", m_Camera->getViewMatrix());
  m_ModelShader.setUniformMat4("projection", projectionMatrix);

  // Shadowmap
  m_ModelShader.setUniform1i("shadowmap", 0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, shadowmap);

  // Add objects to the renderer
  addObjectsToRenderQueue();

  // Opaque objects
  m_MeshRenderer->flushOpaque(m_ModelShader, graphics::RenderPass::LightingPass);

  // Terrain
  m_GLCache->switchShader(m_TerrainShader.getShaderID());
  m_TerrainShader.setUniform1i("shadowmap", 0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, shadowmap);

  m_DynamicLightManager.setupLightingUniforms(m_TerrainShader);
  m_TerrainShader.setUniform3f("viewPos", m_Camera->getPosition());
  glm::mat4 modelMatrix(1);
  modelMatrix = glm::translate(modelMatrix, m_Terrain->getPosition());
  glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(modelMatrix)));
  m_TerrainShader.setUniformMat3("normalMatrix", normalMatrix);
  m_TerrainShader.setUniformMat4("model", modelMatrix);
  m_TerrainShader.setUniformMat4("view", m_Camera->getViewMatrix());
  m_TerrainShader.setUniformMat4("projection", projectionMatrix);
  m_Terrain->Draw(m_TerrainShader, graphics::RenderPass::LightingPass);

  // Skybox
  m_Skybox->Draw();

  // Water
  m_Water->Draw();
  
  // Transparent objects
  m_GLCache->switchShader(m_ModelShader.getShaderID());
  m_MeshRenderer->flushTransparent(m_ModelShader, graphics::RenderPass::LightingPass);
}

void Scene3D::add(scene::SceneNode *renderable) {
  m_Renderables.push_back(renderable);
}

void Scene3D::addObjectsToRenderQueue() {
  auto iter = m_Renderables.begin();
  while (iter != m_Renderables.end()) {
	scene::SceneNode *curr = *iter;
	if (curr->getTransparent()) {
	  m_MeshRenderer->submitTransparent(curr);
	} else {
	  m_MeshRenderer->submitOpaque(curr);
	}

	iter++;
  }
}

const float rotSpeed = 100.0f;
const float moveSpeed = 20.0f;
const float radius = 1.0f;

void Scene3D::onUpdate(float deltaTime) {

  // Rotation testing for PBR gun.
  m_Renderables[1]->rotate(glm::radians(deltaTime * rotSpeed), glm::vec3(0.0f, 1.0f, 0.0f));

  /*
  for (int i = 2; i < 6; ++i){
	m_Renderables[i]->move(glm::vec3(1.0f, 0.0f, 0.0f) * deltaTime * moveSpeed);
  }
  */

}


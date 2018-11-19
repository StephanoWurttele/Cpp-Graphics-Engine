#include "DynamicLight.h"

namespace graphics {

  DynamicLight::DynamicLight(glm::vec3 &lightColour) 
	: lightColour(lightColour), isActive(false) {}

}

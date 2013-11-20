#include "DeferredLight.hpp"
#include "DeferredContainer.hpp"
#include "Camera.hpp"

DeferredLight::DeferredLight() : pos(0.0f), renderer((DeferredContainer*)getGame()->getObjectByName("deferred")) {
    quad.mesh = Meshes.get("quad");
    quad.program = Programs.get("lightQuad");
}

DeferredLight::~DeferredLight() {
}

void DeferredLight::update(float deltaTime) {
	(void) deltaTime;
}

void DeferredLight::draw() const {
	switch(renderer->getMode()) {
		case DeferredContainer::Light:
			drawDeferredLight();
			break;
		default:
			break;
	}
}

void DeferredLight::drawDeferredLight() const{
    Camera* cam = (Camera*)getGame()->getObjectByName("playerCam");
	vec4f poschachi = cam->view*fullTransform*vec4f(pos,1.0);
    quad.program->uniform("diffuse")->set(renderer->getDiffuse());
    quad.program->uniform("normal")->set(renderer->getNormal());
	quad.program->uniform("depth")->set(renderer->getDepth());
	quad.program->uniform("aspectRatio")->set(float(SCRWIDTH)/float(SCRHEIGHT));
	quad.program->uniform("lightPos")->set(vec3f(poschachi.x,poschachi.y,poschachi.z));
    quad.draw();
}

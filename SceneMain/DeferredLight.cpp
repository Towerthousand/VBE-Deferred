#include "DeferredLight.hpp"
#include "DeferredContainer.hpp"

DeferredLight::DeferredLight() : pos(0.0f), color(1.0f), vel(0.0f), radius(15.0f), renderer((DeferredContainer*)getGame()->getObjectByName("deferred")) {
    quad.mesh = Meshes.get("quad");
	quad.program = Programs.get("deferredLight");
}

DeferredLight::~DeferredLight() {
}

void DeferredLight::update(float deltaTime) {
    (void) deltaTime;
	pos += vel*deltaTime;
	transform = glm::translate(mat4f(1.0f), pos);
	Camera* cam = (Camera*)getGame()->getObjectByName("playerCam");
	if(glm::length(cam->getWorldPos()-vec3f(fullTransform*vec4f(0,0,0,1.0))) > 1000.0f)
		removeAndDelete();
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
	vec3f posWorldSpace = vec3f(fullTransform*vec4f(0,0,0,1));
	vec3f posViewSpace = vec3f(cam->getView()*vec4f(posWorldSpace,1.0));

	mat4f t(1.0);
	if(glm::length(posViewSpace) > radius) {
		vec3f front = cam->getWorldPos()-posWorldSpace;
		front = glm::normalize(front);
		vec3f dummyUp(0, 1, 0);
		vec3f right = glm::cross(dummyUp, front);
        right = glm::normalize(right);
		vec3f up = glm::cross(front, right);
        up = glm::normalize(up);
		mat4f rot(right.x, right.y, right.z, 0,
				  up.x   , up.y   , up.z   , 0,
				  front.x, front.y, front.z, 0,
				  0      , 0      , 0      , 1);
		t = glm::scale(rot, vec3f(radius));
		t = glm::translate(t, vec3f(0, 0, 1));
		quad.program->uniform("MVP")->set(cam->projection*cam->getView()*fullTransform*t);
    }
	else
		quad.program->uniform("MVP")->set(t);

	quad.program->uniform("invResolution")->set(vec2f(1.0f/Environment::getScreen()->getWidth(), 1.0f/Environment::getScreen()->getHeight()));
	quad.program->uniform("color0")->set(renderer->getColor0());
	quad.program->uniform("color1")->set(renderer->getColor1());
    quad.program->uniform("depth")->set(renderer->getDepth());
	quad.program->uniform("lightPos")->set(posViewSpace);
    quad.program->uniform("invProj")->set(glm::inverse(cam->projection));
    quad.program->uniform("lightColor")->set(color);
    quad.program->uniform("lightRadius")->set(radius);
	quad.draw();
}

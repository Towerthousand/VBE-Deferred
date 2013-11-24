#include "DeferredLight.hpp"
#include "DeferredContainer.hpp"
#include "Camera.hpp"

DeferredLight::DeferredLight() : pos(0.0f), color(1.0f), radius(20.0f), renderer((DeferredContainer*)getGame()->getObjectByName("deferred")) {
    quad.mesh = Meshes.get("quad");
    quad.program = Programs.get("lightQuad");
}

DeferredLight::~DeferredLight() {
}

void DeferredLight::update(float deltaTime) {
    (void) deltaTime;
	transform = glm::translate(mat4f(1.0f), pos);
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
	vec4f posViewSpace = cam->view*fullTransform*vec4f(0.0, 0.0, 0.0, 1.0);

    mat4f t(1.0);
    if(glm::length(cam->pos-pos) > radius) {
		vec3f front = cam->pos-pos;
		front = glm::normalize(front);
        vec3f dummyUp(0,1,0);
		vec3f right = glm::cross(dummyUp,front);
        right = glm::normalize(right);
		vec3f up = glm::cross(front,right);
        up = glm::normalize(up);
		mat4f rot(right.x, up.x, front.x, 0,
				  right.y, up.y, front.y   , 0,
				  right.z, up.z, front.z , 0,
                  0      , 0      , 0      , 1);
		rot = glm::transpose(rot);


		t = rot;
		t = glm::scale(t,vec3f(radius));
		t = glm::translate(t,vec3f(0,0,1));
		quad.program->uniform("MVP")->set(cam->projection*cam->view*fullTransform*t);
    }
	else
		quad.program->uniform("MVP")->set(t);

	quad.program->uniform("invResolution")->set(vec2f(1.0f/SCRWIDTH, 1.0f/SCRHEIGHT));
    quad.program->uniform("diffuse")->set(renderer->getDiffuse());
    quad.program->uniform("normal")->set(renderer->getNormal());
    quad.program->uniform("depth")->set(renderer->getDepth());
    quad.program->uniform("lightPos")->set(vec3f(posViewSpace.x,posViewSpace.y,posViewSpace.z));
    quad.program->uniform("invProj")->set(glm::inverse(cam->projection));
    quad.program->uniform("lightColor")->set(color);
    quad.program->uniform("lightRadius")->set(radius);
    quad.draw();
}

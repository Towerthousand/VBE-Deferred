#include "DeferredLight.hpp"
#include "DeferredContainer.hpp"
#include "Camera.hpp"

DeferredLight::DeferredLight() : pos(0.0f), color(1.0f), radius(3.0f), renderer((DeferredContainer*)getGame()->getObjectByName("deferred")) {
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
    vec4f posViewSpace = cam->view*fullTransform*vec4f(pos,1.0);

    mat4f t(1.0);
    if(glm::length(cam->pos-pos) > radius) {
        vec3f back = cam->pos-pos;
        back = glm::normalize(back);
        vec3f dummyUp(0,1,0);
        vec3f right = glm::cross(dummyUp,back);
        right = glm::normalize(right);
        vec3f up = glm::cross(back,right);
        up = glm::normalize(up);
        mat4f rot(right.x, right.y, right.z, 0,
                  up.x   , up.y   , up.z   , 0,
                  back.x , back.y , back.z , 0,
                  0      , 0      , 0      , 1);


        t = glm::translate(mat4f(1.0),pos+vec3f(0,0,radius))*rot;
        t = glm::scale(t,vec3f(radius));
    }

    quad.program->uniform("MVP")->set(cam->projection*cam->view*fullTransform*t);
    quad.program->uniform("diffuse")->set(renderer->getDiffuse());
    quad.program->uniform("normal")->set(renderer->getNormal());
    quad.program->uniform("depth")->set(renderer->getDepth());
    quad.program->uniform("lightPos")->set(vec3f(posViewSpace.x,posViewSpace.y,posViewSpace.z));
    quad.program->uniform("invProj")->set(glm::inverse(cam->projection));
    quad.program->uniform("lightColor")->set(color);
    quad.program->uniform("lightRadius")->set(radius);
    quad.draw();
}

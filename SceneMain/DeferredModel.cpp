#include "DeferredModel.hpp"
#include "DeferredContainer.hpp"
#include "Camera.hpp"

DeferredModel::DeferredModel(const std::string& meshID, const std::string& texID, float ambient, float specular) :
	pos(0.0f), rot(0.0f), scale(1.0f), ambient(ambient), specular(specular),
	renderer((DeferredContainer*)getGame()->getObjectByName("deferred")), tex(texID) {
	model.mesh = Meshes.get(meshID);
	model.program = Programs.get("standardDeferred");
}

DeferredModel::~DeferredModel() {
}

void DeferredModel::update(float deltaTime) {
	(void) deltaTime;
	transform = mat4f(1.0f);
	transform = glm::translate(transform,pos);
	transform = glm::rotate(transform,rot.z,vec3f(0,0,1));
	transform = glm::rotate(transform,rot.y,vec3f(0,1,0));
	transform = glm::rotate(transform,rot.x,vec3f(1,0,0));
	transform = glm::scale(transform,scale);
}

void DeferredModel::draw() const {
	switch(renderer->getMode()) {
		case DeferredContainer::Deferred:
			drawDeferredModel();
			break;
		default:
			break;
	}
}

void DeferredModel::drawDeferredModel() const {
	Camera* cam = (Camera*)getGame()->getObjectByName("playerCam");
	model.program->uniform("MVP")->set(cam->projection*cam->view*fullTransform);
	model.program->uniform("M")->set(fullTransform);
	model.program->uniform("V")->set(cam->view);
	model.program->uniform("ambient")->set(ambient);
	model.program->uniform("specular")->set(specular);
	model.program->uniform("diffuseTex")->set(Textures.get(tex));
	model.draw();
}

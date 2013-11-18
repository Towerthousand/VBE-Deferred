#include "DeferredModel.hpp"
#include "DeferredContainer.hpp"
#include "Camera.hpp"

DeferredModel::DeferredModel(const std::string& meshID, const std::string& texID) : renderer((DeferredContainer*)getGame()->getObjectByName("deferred")), tex(texID) {
	model.mesh = Meshes.get(meshID);
	model.program = Programs.get("standardDeferred");
}

DeferredModel::~DeferredModel() {
}

void DeferredModel::update(float deltaTime) {
	(void) deltaTime;
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
	model.draw();
}

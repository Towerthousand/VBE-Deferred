#include "DeferredLight.hpp"
#include "DeferredContainer.hpp"

DeferredLight::DeferredLight() : renderer((DeferredContainer*)getGame()->getObjectByName("deferred")) {
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
	//default light drawing, with white color
}

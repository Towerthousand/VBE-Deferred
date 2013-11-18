#ifndef DEFERREDCONTAINER_HPP
#define DEFERREDCONTAINER_HPP
#include "commons.hpp"

class DeferredContainer : public ContainerObject {
	public:

		enum Technique {
			Deferred = 0,
			Light
		};

		DeferredContainer();
		~DeferredContainer();

		void update(float deltaTime);
		void draw() const;
		Technique getMode() const;

	private:
		RenderTarget* gBuffer;
		mutable Technique drawMode;
		Model quad;
		int texOption;
};

#endif // DEFERREDCONTAINER_HPP

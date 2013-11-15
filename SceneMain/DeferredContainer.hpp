#ifndef DEFERREDCONTAINER_HPP
#define DEFERREDCONTAINER_HPP
#include "commons.hpp"

class DeferredContainer : public ContainerObject {
	public:

		enum Technique {
			Deferred = 0,
			Lights
		};

		DeferredContainer();
		~DeferredContainer();

		void draw() const;

	private:
		RenderTarget* gBuffer;
		mutable Technique drawMode;
};

#endif // DEFERREDCONTAINER_HPP

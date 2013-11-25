#ifndef DEFERREDCONTAINER_HPP
#define DEFERREDCONTAINER_HPP
#include "commons.hpp"

class DeferredContainer : public ContainerObject {
	public:

        enum DrawMode {
			Deferred = 0,
			Light
		};

		DeferredContainer();
		~DeferredContainer();

		void update(float deltaTime);
		void draw() const;
        DrawMode getMode() const;
        Texture* getColor0() const;
        Texture* getDepth() const;
        Texture* getColor1() const;

	private:
		RenderTarget* gBuffer;
		RenderTarget* noBlur;
		RenderTarget* horitzontalBlurred;
		RenderTarget* blurred;
        mutable DrawMode drawMode;
		mutable Model quad;
};

#endif // DEFERREDCONTAINER_HPP

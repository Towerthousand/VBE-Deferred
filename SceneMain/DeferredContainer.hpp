#ifndef DEFERREDCONTAINER_HPP
#define DEFERREDCONTAINER_HPP
#include "commons.hpp"

class DeferredContainer : public ContainerObject {
	public:
        enum DrawMode {
			Deferred = 0,
			Shadow,
			Light
		};

		DeferredContainer();
		~DeferredContainer();

		void update(float deltaTime);
		void draw() const;
        DrawMode getMode() const;
		Texture2D* getColor0() const;
		Texture2D* getColor1() const;
		Texture2D* getDepth() const;

	private:
		RenderTarget* gBuffer;
		RenderTarget* noBlur;
		RenderTarget* blurMask;
		RenderTarget* horitzontalBlurred;
		RenderTarget* blurred;
		RenderTarget* shadowMap;
        mutable DrawMode drawMode;
		mutable Model quad;
};

#endif // DEFERREDCONTAINER_HPP

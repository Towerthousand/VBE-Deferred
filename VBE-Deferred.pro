include(VBE/VBE.pro)
INCLUDEPATH += .
SOURCES += main.cpp \
    commons.cpp \
    SceneMain/SceneMain.cpp \
    SceneMain/PlayerCamera.cpp \
    SceneMain/DeferredModel.cpp \
    SceneMain/DeferredContainer.cpp \
    SceneMain/DeferredLight.cpp \
    particles/ParticleSystem.cpp \
    particles/ParticleEmitter.cpp \
    particles/Particle.cpp \
    particles/LightParticleEmitter.cpp

HEADERS += \
    commons.hpp \
    SceneMain/SceneMain.hpp \
    SceneMain/PlayerCamera.hpp \
    SceneMain/DeferredModel.hpp \
    SceneMain/DeferredContainer.hpp \
    SceneMain/DeferredLight.hpp \
    particles/ParticleSystem.hpp \
    particles/ParticleEmitter.hpp \
    particles/Particle.hpp \
    particles/LightParticleEmitter.hpp

OTHER_FILES += \
    data/shaders/quad.vert \
    data/shaders/standardDeferred.frag \
    data/shaders/standardDeferred.vert \
    data/shaders/light.frag \
    data/shaders/particles.frag \
    data/shaders/particles.vert \
    data/shaders/particles.geom \
    data/shaders/ambientPass.frag \
    data/shaders/blurPassVertical.frag \
    data/shaders/blurPassHoritzontal.frag \
    data/shaders/quad.frag \
    data/shaders/blurMaskPass.frag \
    data/shaders/depth.frag \
    data/shaders/depth.vert


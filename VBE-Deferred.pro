include(VBE/VBE.pro)
INCLUDEPATH += .
DEFINES += __DLOG
SOURCES += main.cpp \
    commons.cpp \
    SceneMain/SceneMain.cpp \
    SceneMain/Camera.cpp \
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
    SceneMain/Camera.hpp \
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
    data/shaders/quad.frag \
    data/shaders/standardDeferred.frag \
    data/shaders/standardDeferred.vert \
    data/shaders/light.frag \
    data/shaders/particles.frag \
    data/shaders/particles.vert \
    data/shaders/particles.geom


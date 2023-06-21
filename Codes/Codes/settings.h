#ifndef SETTINGS_H
#define SETTINGS_H

class Settings {
    friend class Controls;
    friend class Game;

public:
    bool isThirdPersonView() const;
    bool isWireframeMode() const;
    bool isCollisionEnabled() const;
    bool isFlying() const;
    int getLoadDistance() const;
    bool isShowingChunkInformations() const;

    bool isSSAOEnabled() const;

private:
    bool thirdPersonView = false;
    bool wireframeMode = false;
    bool collisionEnabled = false;
    bool flying = true;
    int loadDistance = 4;
    bool showingChunkInformations = false;

    bool ssaoEnabled = false;
};

#endif

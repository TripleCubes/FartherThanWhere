#ifndef SETTINGS_H
#define SETTINGS_H

class Settings {
    friend class Controls;

public:
    bool isThirdPersonView() const;
    bool isWireframeMode() const;
    bool isCollisionEnabled() const;
    bool isFlying() const;
    int getLoadDistance() const;

private:
    bool thirdPersonView = false;
    bool wireframeMode = false;
    bool collisionEnabled = false;
    bool flying = true;
    int loadDistance = 4;
};

#endif

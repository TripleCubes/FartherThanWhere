#ifndef SETTINGS_H
#define SETTINGS_H

class Settings {
    friend class Controls;

public:
    bool isThirdPersonView() const;
    bool isWireframeMode() const;
    bool isCollisionEnabled() const;
    bool isFlying() const;

private:
    bool thirdPersonView = false;
    bool wireframeMode = false;
    bool collisionEnabled = false;
    bool flying = true;
};

#endif

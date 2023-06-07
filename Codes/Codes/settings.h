#ifndef SETTINGS_H
#define SETTINGS_H

class Settings {
    friend class Controls;

public:
    bool isThirdPersonView() const;
    bool isWireframeMode() const;
    bool isCollisionDisabled() const;

private:
    bool thirdPersonView = false;
    bool wireframeMode = false;
    bool collisionDisabled = false;
};

#endif

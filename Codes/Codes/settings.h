#ifndef SETTINGS_H
#define SETTINGS_H

class Settings {
    friend class Controls;

public:
    bool isThirdPersonView() const;
    bool isWireframeMode() const;

private:
    bool thirdPersonView = false;
    bool wireframeMode = false;
};

#endif

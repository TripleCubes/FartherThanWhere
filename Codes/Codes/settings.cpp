#include <Codes/settings.h>

bool Settings::isThirdPersonView() const {
    return thirdPersonView;
}

bool Settings::isWireframeMode() const {
    return wireframeMode;
}

bool Settings::isCollisionDisabled() const {
    return collisionDisabled;
}

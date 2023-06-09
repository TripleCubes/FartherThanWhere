#include <Codes/settings.h>

bool Settings::isThirdPersonView() const {
    return thirdPersonView;
}

bool Settings::isWireframeMode() const {
    return wireframeMode;
}

bool Settings::isCollisionEnabled() const {
    return collisionEnabled;
}

bool Settings::isFlying() const {
    return flying;
}

int Settings::getLoadDistance() const {
    return loadDistance;
}

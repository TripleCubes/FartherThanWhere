#ifndef CONTROLS_H
#define CONTROLS_H

class View;
class Settings;

class Controls {
public:
    Controls(Settings &settings, View &view);

    void update();
    
private:
    Settings &settings;
    View &view;
    float cameraRotationX = 0;
    float cameraRotationY = 0;

    void updateSettings();
    void updateCameraDir();
    void updateMovements();
};

#endif

#ifndef CONTROLS_H
#define CONTROLS_H

class View;
class Settings;
class ChunkLoader;

class Controls {
public:
    Controls(Settings &settings, View &view, ChunkLoader &chunkLoader);

    void update();
    
private:
    Settings &settings;
    View &view;
    ChunkLoader &chunkLoader;

    float cameraRotationX = 0;
    float cameraRotationY = 0;

    void updateSettings();
    void updateCameraDir();
    void updateMovements();
    void placeBreakBlock();
};

#endif

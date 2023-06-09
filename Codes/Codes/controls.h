#ifndef CONTROLS_H
#define CONTROLS_H

class Camera;
class Settings;
class ChunkLoader;
class Player;

class Controls {
public:
    Controls(Settings &settings, Camera &camera, ChunkLoader &chunkLoader, Player &player);

    void update();
    
private:
    Settings &settings;
    Camera &camera;
    ChunkLoader &chunkLoader;
    Player &player;

    float cameraRotationX = 0;
    float cameraRotationY = 0;

    void updateSettings();
    void updateCameraDir();
    void updateMovements();
    void placeBreakBlock();
};

#endif

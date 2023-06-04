#include <Codes/Types/intPos.h>

#include <Codes/Types/vec3.h>
#include <Codes/Chunk/chunk.h>
#include <cmath>

int mod(int a, int b) {
    return a % b >= 0 ? a % b : (a % b) + b;
}

IntPos::IntPos(int x, int y, int z): x(x), y(y), z(z) {}
IntPos::IntPos(Vec3 vec): x(floor(vec.x)), y(floor(vec.y)), z(floor(vec.z)) {}
IntPos::IntPos() {}

bool IntPos::operator == (IntPos pos) const {
    return x==pos.x && y==pos.y && z==pos.z;
}

IntPos IntPos::operator + (IntPos pos) const {
    return IntPos(x+pos.x, y+pos.y, z+pos.z);
}

IntPos IntPos::operator - (IntPos pos) const {
    return IntPos(x-pos.x, y-pos.y, z-pos.z);
}

IntPos IntPos::operator * (int num) const {
    return IntPos(x*num, y*num, z*num);
}

IntPos IntPos::operator / (int num) const {
    return IntPos(x/num, y/num, z/num);
}

IntPos IntPos::operator % (int num) const {
    return IntPos(mod(x, num), mod(y, num), mod(z, num));
}

IntPos IntPos::getChunkPos() const {
    return IntPos((int)floor((float)x / CHUNK_WIDTH), (int)floor((float)y / CHUNK_WIDTH), (int)floor((float)z / CHUNK_WIDTH));
}

IntPos IntPos::getBlockPosInChunk() const {
    return *this % CHUNK_WIDTH;
}

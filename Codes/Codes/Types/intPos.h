#ifndef INTPOS_H
#define INTPOS_H

class Vec3;

struct IntPos {
    int x = 0;
    int y = 0;
    int z = 0;

    IntPos(int x, int y, int z);
    IntPos(Vec3 vec);
    IntPos();

    bool operator == (IntPos pos) const;
    IntPos operator + (IntPos pos) const;
    IntPos operator - (IntPos pos) const;
    IntPos operator * (int num) const;
    IntPos operator / (int num) const;
    IntPos operator % (int num) const;

    IntPos getChunkPos() const;
    IntPos getBlockPosInChunk() const;
};

#endif

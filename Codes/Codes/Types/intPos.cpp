#include <Codes/Types/intPos.h>

int mod(int a, int b) {
    return a % b >= 0 ? a % b : (a % b) + b;
}

IntPos::IntPos(int x, int y, int z): x(x), y(y), z(z) {}

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

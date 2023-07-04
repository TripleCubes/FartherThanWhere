#include <Codes/Types/color.h>

float max(float a, float b) {
    return a > b? a : b;
}

float min(float a, float b) {
    return a < b? a : b;
}

Color::Color(float r, float g, float b, float a): r(r), g(g), b(b), a(a) {}

Color Color::operator + (Color color) const {
    return Color(min(r + color.r, 1), 
                min(g + color.g, 1), 
                min(b + color.b, 1), 
                min(a + color.a, 1));
}

Color Color::operator - (Color color) const {
    return Color(max(r - color.r, 0), 
                max(g - color.g, 0), 
                max(b - color.b, 0), 
                max(a - color.a, 0));
}

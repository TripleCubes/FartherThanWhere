#ifdef DEBUG

#include <Codes/print.h>

#include <iostream>
#include <Codes/Types/vec2.h>
#include <Codes/Types/vec3.h>
#include <Codes/Types/intPos.h>

void Debug::print(const std::string &text) {
    std::cout << text << " ";
}

void Debug::println(const std::string &text) {
    std::cout << text << std::endl;
}

void Debug::print(const char *text) {
    std::cout << text << " ";
}

void Debug::println(const char *text) {
    std::cout << text << std::endl;
}

void Debug::print(int num) {
    std::cout << num << " ";
}

void Debug::println(int num) {
    std::cout << num << std::endl;
}

void Debug::print(float num) {
    std::cout << num << " ";
}

void Debug::println(float num) {
    std::cout << num << std::endl;
}

void Debug::print(Vec2 vec) {
    std::cout << vec.x << " " << vec.y << " ";
}

void Debug::println(Vec2 vec) {
    std::cout << vec.x << " " << vec.y << std::endl;
}

void Debug::print(Vec3 vec) {
    std::cout << vec.x << " " << vec.y << " "  << vec.z << " ";
}

void Debug::println(Vec3 vec) {
    std::cout << vec.x << " " << vec.y << " "  << vec.z << std::endl;
}

void Debug::print(IntPos pos) {
    std::cout << pos.x << " " << pos.y << " "  << pos.z << " ";    
}

void Debug::println(IntPos pos) {
    std::cout << pos.x << " " << pos.y << " "  << pos.z << std::endl;
}

void Debug::print(bool b) {
    std::cout << (b ? "true" : "false") << " ";
}

void Debug::println(bool b) {
    std::cout << (b ? "true" : "false") << std::endl;
}

#endif

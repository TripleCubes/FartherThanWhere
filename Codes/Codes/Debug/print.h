#ifdef DEBUG
    #ifndef PRINT_H
    #define PRINT_H

    #include <string>
    class Vec2;
    class Vec3;
    class IntPos;

    #define PRINTLN(a) Debug::println(a)
    #define PRINT(a) Debug::print(a)

    class Debug {
    public:
        static void print(const std::string &text);
        static void println(const std::string &text);

        static void print(const char *text);
        static void println(const char *text);

        static void print(int num);
        static void println(int num);

        static void print(float num);
        static void println(float num);

        static void print(Vec2 vec);
        static void println(Vec2 vec);

        static void print(Vec3 vec);
        static void println(Vec3 vec);

        static void print(IntPos pos);
        static void println(IntPos pos);

        static void print(bool b);
        static void println(bool b);
    
    private:
    };

    #endif
#else
    #define PRINTLN
    #define PRINT
#endif

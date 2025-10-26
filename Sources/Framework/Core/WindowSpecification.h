#ifndef GUARD_WINDOWSPECIFICATION_H
#define GUARD_WINDOWSPECIFICATION_H

struct WindowSpecification final
{
    int x;
    int y;
    int width;
    int height;
    const char* title;
    bool resizable;
    bool fullscreen;
    bool vsync;
    bool decorated;
};

#endif // !GUARD_WINDOWSPECIFICATION_H
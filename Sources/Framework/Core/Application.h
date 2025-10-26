#ifndef GUARD_APPLICATION_H
#define GUARD_APPLICATION_H

#include <memory>

#include "Window.h"
#include "Graphics.h"

class Application final
{
public:
    static void Initialize();
    static void Run();
private:
    static std::unique_ptr<Window> window;
    static std::unique_ptr<Graphics> graphics;
};

#endif // !GUARD_APPLICATION_H
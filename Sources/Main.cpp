#include "Framework/Application.h"
#include "Framework/Common.h"

int main()
{
    Application::Configuration configuration;
    configuration.name   = "Labyrinth";
    configuration.width  = 1280;
    configuration.height = 720;

    Application app(configuration);
    return app.run();
}
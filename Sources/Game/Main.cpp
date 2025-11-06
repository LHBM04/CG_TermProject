#include "../Framework/Core/Application.h"

int main()
{
    Application::Specification specification{};
    specification.name = "Game Application";

    Application::Initialize(specification);
    Application::Run();

    return 0;
}
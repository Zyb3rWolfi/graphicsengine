#include "Engine/Core/Application.h"

int main() {
    Application app(1920, 1080);

    if (app.Init()) {
        app.Run();
    }

    return 0;
}

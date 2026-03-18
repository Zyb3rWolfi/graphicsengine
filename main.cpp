#include "Engine/Core/Application.h"

int main() {
    Application app(800, 600);

    if (app.Init()) {
        app.Run();
    }

    return 0;
}

#include "StartMenu.h"
#include "Application.h"

int main() {
    StartMenu menu;
    if (menu.Run()) {
        Application app;
        while (app.IsRunning()) {
            app.Tick();
        }
    }
    return 0;
}
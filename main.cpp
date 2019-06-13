#include <iostream>

#include "ue.h"



constexpr auto WINDOW_WIDTH = 1280;
constexpr auto WINDOW_HEIGHT = 720;



int main(int argc, char const *argv[]) {
    ue::Window win("Test", WINDOW_WIDTH, WINDOW_HEIGHT);
    ue::Renderer render(win);
    ue::Target screen(render, WINDOW_WIDTH, WINDOW_HEIGHT);

    return 0;
}

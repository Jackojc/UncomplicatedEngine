// CANVAS
// Manages a window and renderer in unison.


#pragma once


namespace ue {
    struct Canvas {
        ue::Window window;
        ue::Renderer renderer;

        //void set_render_target();  // Allow user to specify a custom render target.

        //void draw();
    };
}
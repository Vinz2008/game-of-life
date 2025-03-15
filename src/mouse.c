#include "mouse.h"
#include "raymath.h"

void handle_mouse_move(Camera2D* camera){
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
        Vector2 delta = GetMouseDelta();
        delta = Vector2Scale(delta, -1.0f/camera->zoom);
        camera->target = Vector2Add(camera->target, delta);
    }
}

void handle_mouse_zoom(Camera2D* camera){
    float wheel = GetMouseWheelMove();
    if (wheel != 0){
        // Get the world point that is under the mouse
        Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), *camera);

            // Set the offset to where the mouse is
        camera->offset = GetMousePosition();

            // Set the target to match, so that the camera maps the world space point 
            // under the cursor to the screen space point under the cursor at any zoom
        camera->target = mouseWorldPos;

            // Zoom increment
        float scaleFactor = 1.0f + (0.25f*fabsf(wheel));
        if (wheel < 0){ 
            scaleFactor = 1.0f/scaleFactor; 
        }
        camera->zoom = Clamp(camera->zoom*scaleFactor, 0.125f, 64.0f);
    }
}
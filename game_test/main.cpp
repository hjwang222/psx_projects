extern "C"
{
    #include <psx.h>
    #include <meidogte.h>
    #include <string.h>
}
#include "math_utils.h"

GsDrawEnv game_draw_env;
GsDispEnv game_disp_env;
unsigned int game_draw_list[0x4000];

unsigned int tick = 0;
unsigned int last_tick = 0;

#define CORNFLOUR_BLUE 100, 149, 237
#define WIDTH 320
#define HEIGHT 240
#define DOUBLE_BUFFER_OFFSET 16

void swap_buffers()
{
    if (game_disp_env.y == 0) {
        // we add the offset as there is a certain amount of overscan
        // and if we don't add a big of vertical padding, you'll see the next frame
        // being drawn on the bottom of the frame
        game_disp_env.y = HEIGHT + DOUBLE_BUFFER_OFFSET;
        game_draw_env.y = 0;
    }
    else {
        game_disp_env.y = 0;
        game_draw_env.y = HEIGHT + DOUBLE_BUFFER_OFFSET;
    }
    GsSetDrawEnv(&game_draw_env);
    GsSetDispEnv(&game_disp_env);
}

void vblank()
{
    tick++;
}

void logic_update()
{

}

void render_update()
{
    unsigned char *image = new unsigned char[WIDTH*HEIGHT];

    for (int y = 0 ; y < HEIGHT; y++) {
        for (int x = 0 ; x < WIDTH; x++) {
            image[y*WIDTH + x] = isin(x * 8) % 255;
        }   
    }

    GsImage game_image;
    game_image.x = WIDTH;
    game_image.y = 0;
    game_image.w = WIDTH;
    game_image.h = HEIGHT;
    game_image.has_clut = 0;
    game_image.pmode = 1;
    game_image.data = image;
    GsUploadImage(&game_image);

    while(GsIsDrawing()) {};
    delete[] image;
}

int main()
{
    // setup
    PSX_Init();
    GsInit();
    GsClearMem();
    InitGeom();
    SsInit();

    game_draw_env.dither = 0;
    game_draw_env.draw_on_display = 0;
    game_draw_env.x = 0;
    game_draw_env.y = 0;
    game_draw_env.w = WIDTH;
    game_draw_env.h = HEIGHT;
    game_draw_env.ignore_mask = 0;
    game_draw_env.set_mask = 0;
    GsSetDrawEnv(&game_draw_env);

    game_disp_env.x = 0;
    game_disp_env.y = HEIGHT;
    GsSetDispEnv(&game_disp_env);
    GsSetList(game_draw_list);

    GsSetVideoMode(WIDTH, HEIGHT, VMODE_PAL);
    SetVBlankHandler(vblank);

    // update
    while(1) {
        logic_update();
        render_update();
        // double buffer swap
        swap_buffers();
    }

    return 0;
}
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

Matrix perspective_matrix;
Matrix view_matrix;

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
    Matrix mvp = perspective_matrix;

    Vector v0(0.0, 0.2, -5.0, 1.0);
    Vector v1(-0.2, -0.2, -5.0, 1.0);
    Vector v2(0.2, -0.2, -5.0, 1.0);

    v0 = mvp*v0;
    v1 = mvp*v1;
    v2 = mvp*v2;

    GsPrintFont(0,0,"x %d y %d", v0.screen_x(), v0.screen_y());
    GsPrintFont(0,10,"x %d y %d", v1.screen_x(), v1.screen_y());
    GsPrintFont(0,20,"x %d y %d", v2.screen_x(), v2.screen_y());

    for (int row = 0 ; row < 4 ; row++) {
        for (int col = 0 ; col < 4 ; col++) {
            GsPrintFont(0 + col*80, 200 + row*10, "%.2f", fix16_to_dbl(mvp.m_values[row][col].value));
        }
    }

    GsPoly3 tri;
    tri.r = 255;
    tri.g = tri.b = 0;

    tri.x[0] = v0.screen_x();
    tri.y[0] = v0.screen_y();

    tri.x[1] = v1.screen_x();
    tri.y[1] = v1.screen_y();

    tri.x[2] = v2.screen_x();
    tri.y[2] = v2.screen_y();

    GsSortPoly3(&tri);
    GsDrawList();

    while(GsIsDrawing()) {};
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

    GsLoadFont(320,1,320,0);
    double aspect = double(WIDTH)/HEIGHT;
    perspective_matrix = Matrix::perspective(30.0, aspect, 0.1, 100.0);

    // update
    while(1) {
        logic_update();
        render_update();
        // double buffer swap
        swap_buffers();
    }

    return 0;
}
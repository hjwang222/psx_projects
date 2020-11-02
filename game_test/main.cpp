extern "C"
{
    #include <psx.h>
}

GsDrawEnv game_draw_env;
GsDispEnv game_disp_env;
unsigned int game_draw_list[0x4000];

unsigned int tick = 0;
unsigned int last_tick = 0;

#define CORNFLOUR_BLUE 100, 149, 237
#define WIDTH 320
#define HEIGHT 240

void swap_buffers()
{
    if (game_disp_env.y == HEIGHT) {
        game_disp_env.y = 0;
        game_draw_env.y = HEIGHT;
    }
    else {
        game_disp_env.y = HEIGHT;
        game_draw_env.y = 0;
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

}

int main()
{
    // setup
    PSX_Init();
    GsInit();
    GsClearMem();
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

    // DrawFBRect(0,0,WIDTH, HEIGHT, CORNFLOUR_BLUE);
    // DrawFBRect(0,HEIGHT, WIDTH, HEIGHT, 255, 0, 0);

    // update
    while(1) {
        logic_update();
        render_update();
        // double buffer swap
        swap_buffers();
    }

    return 0;
}
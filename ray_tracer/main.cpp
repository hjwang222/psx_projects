extern "C"
{
	#include <psx.h>
	#include <stdlib.h>
	#include <math.h>
}

#include "math_utils.h"

GsDrawEnv game_draw_env;
GsDispEnv game_disp_env;
unsigned int game_draw_list[0x4000];

void game_vblank_handler() {
	
}

void render(int width, int height, int fov)
{
	GsRectangle pixel;
	pixel.w = 1;
	pixel.h = 1;
	pixel.attribute = 0;

	double aspect_ratio = width / double(height);
	double angle = tan((30 / 2.0) * (3.14/180));
	double inv_width = 1.0 / width;
	double inv_height = 1.0 / height;

	for (int y = 0 ; y < width ; y++) {
		for (int x = 0 ; x < height ; x++) {
			double xx = (2 * ((x + 0.5) * inv_width) - 1) * angle * aspect_ratio;
			double yy = (1 - 2 * ((y + 0.5) * inv_height)) * angle;

			pixel.x = x;
			pixel.y = y;
			pixel.r = rand() % 255;
			pixel.g = rand() % 255;
			pixel.b = rand() % 255;
			GsSortRectangle(&pixel);
		}
		GsDrawList();
		while(GsIsDrawing()){};
	}
}

int main(){
	// setup playstation
	PSX_Init();
	GsInit();
	GsClearMem();

	// setup draw environment
	game_draw_env.dither = 0;
	game_draw_env.draw_on_display = 1;
	game_draw_env.x = 0;
	game_draw_env.y = 0;
	game_draw_env.w = 320;
	game_draw_env.h = 240;
	game_draw_env.ignore_mask = 0;
	game_draw_env.set_mask = 0;

	GsSetDrawEnv(&game_draw_env);
	
	// setup display environment
	game_disp_env.x = 0;
	game_disp_env.y = 0;
	GsSetDispEnv(&game_disp_env);
	GsSetList(game_draw_list);

	GsSetVideoMode(320, 240, VMODE_PAL);
	SetVBlankHandler(game_vblank_handler);
	
	while(1) {
		render(320, 240, 30);
	}
	
	return 0;
}
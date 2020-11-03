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

struct Material
{
	Vector surface_colour;
	Vector emission_colour;

	bool is_light()
	{
		return emission_colour.magnitude() > 0.0;
	}
};

class Sphere
{
	public:
	Vector pos;
	double radius;
	Material material;

	Sphere()
	{
	}

	Sphere(Vector _pos, double _radius, const Material& _material)
	{
		pos = _pos;
		radius = _radius;
		material = _material;
	}

	bool intersect(const Vector& origin, const Vector& direction, double& t0, double& t1)
	{
		t0 = t1 = 0;
		Vector L = pos - origin;
		double tca = L.dot(direction);
		double d2 = L.dot(L) - (tca*tca);
		if (d2 > (radius*radius))
		{
			return false;
		}

		double thc = sqrt((radius*radius) - d2);
		t0 = tca - thc;
		t1 = tca + thc;

		if (t0 > t1)
		{
			double temp = t0;
			t0 = t1;
			t1 = temp;
		}

		if (t0 < 0.0)
		{
			t0 = t1;
			if (t0 < 0.0) {
				return false;
			}
		}

		return true;
	}
};

int num_spheres = 3;
Sphere scene_objects[3];

void get_intersecting_object(const Vector& origin, const Vector& direction, int& idx, double& tnear)
{
	idx = -1;
	tnear = MAXFLOAT;

	for (int i = 0 ; i < num_spheres ; i++) {
		double t0 = 0;
		double t1 = 0;
		if (scene_objects[i].intersect(origin, direction, t0, t1)){
			if (t0 < 0.0) {
				t0 = t1;
			}
			if (t0 < tnear) {
				tnear = t0;
				idx = i;
			}
		}
	}
}

Vector trace(const Vector& origin, const Vector& direction)
{
	Vector result;

	int cur_idx = -1;
	double tnear = 0;
	get_intersecting_object(origin, direction, cur_idx, tnear);

	if (cur_idx != -1)
	{
		result.x(1.0);
		// Sphere * current_object = &scene_objects[cur_idx];
		// double bias = 0.0;
		// Vector phit = origin + direction*(tnear + bias);
		// Vector nhit = (phit - current_object->pos).normalize();
		
		// if (current_object->material.is_light()) {
		// 	Material * material = &(current_object->material);
		// 	return (material->surface_colour*material->emission_colour)*(1.0/tnear);
		// }

		// for (int idx = 0 ; idx < num_spheres ; idx++) {
		// 	Sphere * object = &scene_objects[idx];
		// 	if (object->material.is_light()) {
		// 		Vector light_dir = object->pos - phit;
		// 		double light_dist = light_dir.magnitude();
		// 		light_dir = light_dir.normalize();

		// 		int shadow_ray_idx;
		// 		get_intersecting_object(phit, light_dir, shadow_ray_idx, tnear);
		// 		if (shadow_ray_idx == idx) {
		// 			Vector object_colour = object->material.surface_colour;
		// 			Vector light_colour = scene_objects[shadow_ray_idx].material.emission_colour;
		// 			double intensity = nhit.dot(light_dir)*(1.0/light_dist);
		// 			if (intensity > 1.0) {
		// 				intensity = 1.0;
		// 			}
		// 			else if (intensity < 0.0) {
		// 				intensity = 0.0;
		// 			}
		// 			result = result + (object_colour*light_colour)*intensity;
		// 		}
		// 	}
		// }
	}

	return result;
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

	Vector origin;

	for (int y = 0 ; y < width ; y++) {
		for (int x = 0 ; x < height ; x++) {
			double xx = (2 * ((x + 0.5) * inv_width) - 1) * angle * aspect_ratio;
			double yy = (1 - 2 * ((y + 0.5) * inv_height)) * angle;

			Vector ray = Vector(xx, yy, -1).normalize();
			Vector colour = trace(origin, ray);

			if (colour.x() > 1.0) {
				colour.x(1.0);
			}

			if (colour.y() > 1.0) {
				colour.y(1.0);
			}

			if (colour.z() > 1.0) {
				colour.z(1.0);
			}

			pixel.x = x;
			pixel.y = y;
			pixel.r = colour.x() * 255;
			pixel.g = colour.y() * 255;
			pixel.b = colour.z() * 255;
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
	game_draw_env.draw_on_display = 0;
	game_draw_env.x = 0;
	game_draw_env.y = 0;
	game_draw_env.w = 320;
	game_draw_env.h = 240;
	game_draw_env.ignore_mask = 0;
	game_draw_env.set_mask = 0;

	GsSetDrawEnv(&game_draw_env);
	
	// setup display environment
	game_disp_env.x = 0;
	game_disp_env.y = 256;
	GsSetDispEnv(&game_disp_env);
	GsSetList(game_draw_list);

	GsSetVideoMode(320, 240, VMODE_NTSC);
	SetVBlankHandler(game_vblank_handler);

	Material light_material;
	light_material.emission_colour = Vector(50,0,0);
	light_material.surface_colour = Vector(1,1,1);

	scene_objects[0] = Sphere(Vector(10,10,-50),2,light_material);

	{
		Material surface_material;
		surface_material.surface_colour = Vector(1,0,0);
		scene_objects[1] = Sphere(Vector(0,0,-300), 50, surface_material);
	}

	{
		Material surface_material;
		surface_material.surface_colour = Vector(0,0,1);
		scene_objects[2] = Sphere(Vector(0,0,-150), 10, surface_material);
	}
	
	while(1) {
		render(320, 240, 30);

		if (game_disp_env.y == 256) {
			game_disp_env.y = 0;
			game_draw_env.y = 256;
		}
		else {
			game_disp_env.y = 256;
			game_draw_env.y = 0;
		}
		
		GsSetDispEnv(&game_disp_env);
		GsSetDrawEnv(&game_draw_env);
	}
	
	return 0;
}
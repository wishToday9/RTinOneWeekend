#include <iostream>
#include <fstream>

#include "rtweekend.h"
#include "hittable_list.h"
#include "sphere.h"


vec3 ray_color(const ray& r, hittable_list& world) {
	hit_record rec;
	if (world.hit(r, 0, infinity, rec)) {
		return 0.5 * (rec.normal + color(1, 1, 1));
	}

	vec3 unit_direction = unit_vector(r.direction());
	auto a = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - a) * vec3(1.0, 1.0, 1.0) + a * vec3(0.5, 0.7, 1.0);
}
int main() {
	//image
	auto aspect_ratio = 16.0 / 9.0;
	int image_width = 400;
	int image_height = static_cast<int>(image_width / aspect_ratio);
	image_height = (image_height < 1) ? 1 : image_height;

	//camera
	auto focal_length = 1.0;
	auto viewport_height = 2.0;
	auto viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);
	auto camera_center = vec3(0.0, 0.0, 0.0);

	//world
	hittable_list world;
	world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
	world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));


	//calculate the vectors across the horizontal and the vertical viewport edges
	auto viewport_u = vec3(viewport_width, 0, 0);
	auto viewport_v = vec3(0, -viewport_height, 0);

	//calculate the horizontal and vertical delta vectors from pixel to pixel
	auto pixel_delta_u = viewport_u / image_width;
	auto pixel_delta_v = viewport_v / image_height;

	//calculate the location of upper left pixel
	auto viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;

	auto pixel100_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

	std::ofstream ofs("./img.ppm");



	ofs << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	for (int j = 0; j < image_height; ++j)
	{
		std::cerr << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
		for (int i = 0; i < image_width; ++i) {
			auto pixel_center = pixel100_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
			auto ray_direction = pixel_center - camera_center;
			ray r(camera_center, ray_direction);

			color pixel_colr = ray_color(r, world);

			pixel_colr.write_color(ofs);
			
		}
	}
	std::cerr << "\nDone.\n ";
}
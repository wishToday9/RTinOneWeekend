#pragma once

#include "rtweekend.h"
#include "vec3.h"
#include "hittable.h"
#include "material.h"

#include <fstream>


class camera {
public:
	double aspect_ratio = 1.0f;
	int image_width = 100;
	int samples_per_pixel = 10;
	int max_depth = 10;

	double vfov = 90; // vertical view angle(field of view)
	point3 lookfrom = point3(0, 0, -1);
	point3 lookat = point3(0, 0, 0);
	vec3 vup = vec3(0, 1, 0);

	double defocus_angle = 0;
	double focus_dist = 10;


	void render(const hittable& world) {
		initialize();
		std::ofstream ofs("./img.ppm");
		ofs << "P3\n" << image_width << ' ' << image_height << "\n255\n";

		for (int j = 0; j < image_height; ++j)
		{
			std::cerr << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
			for (int i = 0; i < image_width; ++i) {
				color pixel_color{ 0.0f, 0.0f, 0.0f };
				for (size_t k = 0; k < samples_per_pixel; k++)
				{
					ray r = get_ray(i, j);
					pixel_color += ray_color(r, max_depth, world);
				}
				

				pixel_color.write_color(ofs, pixel_color, samples_per_pixel);
			}
		}
	}

private:
	ray get_ray(int i, int j) const {
		// Get a randomly sampled camera ray for the pixel at location i,j.

		auto pixel_center = pixel100_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
		auto pixel_sample = pixel_center + pixel_sample_square();

		auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();;
		auto ray_direction = pixel_sample - ray_origin;

		return ray(ray_origin, ray_direction);
	}

	point3 defocus_disk_sample() const {
		// Returns a random point in the camera defocus disk.
		auto p = random_in_unit_disk();
		return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
	}

	vec3 pixel_sample_square() const {
		// Returns a random point in the square surrounding a pixel at the origin.
		auto px = -0.5 + random_double();
		auto py = -0.5 + random_double();
		return (px * pixel_delta_u) + (py * pixel_delta_v);
	}

	void initialize() {
		image_height = static_cast<int>(image_width / aspect_ratio);
		image_height = (image_height < 1) ? 1 : image_height;

		center = lookfrom;

		//camera
		//auto focal_length = (lookfrom - lookat).length();
		auto theta = degrees_to_radians(vfov);
		auto h = tan(theta / 2);

		auto viewport_height = h * 2 * focus_dist;
		auto viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);

		//calculate the u,v,w
		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);

		//calculate the vectors across the horizontal and the vertical viewport edges
		auto viewport_u = viewport_width * u;
		auto viewport_v = -viewport_height * v;

		//calculate the horizontal and vertical delta vectors from pixel to pixel
		pixel_delta_u = viewport_u / image_width;
		pixel_delta_v = viewport_v / image_height;

		//calculate the location of upper left pixel
		auto viewport_upper_left = center - focus_dist * w - viewport_u / 2 - viewport_v / 2;

		pixel100_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

		// Calculate the camera defocus disk basis vectors.
		auto defocus_radius = focus_dist * tan(degrees_to_radians(defocus_angle / 2));
		defocus_disk_u = u * defocus_radius;
		defocus_disk_v = v * defocus_radius;
	}

	color ray_color(const ray& r, int depth,  const hittable& world) const {
		if (depth <= 0) {
			return color{ 0, 0, 0 };
		}

		hit_record rec;
		if (world.hit(r, interval(0.001, infinity), rec)) {
			ray scattered;
			color attenuation;
			if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
				return attenuation * ray_color(scattered, depth - 1, world);
			return color(0, 0, 0);
		}

		vec3 unit_direction = unit_vector(r.direction());
		auto a = 0.5 * (unit_direction.y() + 1.0);
		return (1.0 - a) * vec3(1.0, 1.0, 1.0) + a * vec3(0.5, 0.7, 1.0);
	}
private:
	int image_height;
	point3 center;
	point3 pixel100_loc;
	vec3 pixel_delta_u;
	vec3 pixel_delta_v;
	vec3 u, v, w; //camera frame basis vectors
	vec3   defocus_disk_u;  // Defocus disk horizontal radius
	vec3   defocus_disk_v;  // Defocus disk vertical radius
};


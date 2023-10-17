#pragma once

#include "Texture.h"
#include "perlin.h"
class noise_texture : public texture {
public:
	noise_texture() {}
	noise_texture(double sc) : scale(sc) {}
	virtual vec3 value(double u, double v, const vec3& p) const {
		auto s = scale * p;
		return color(1, 1, 1) * 0.5 * (1 + sin(s.z() + 10 * noise.turb(s)));
	}
private:
	perlin noise;
	double scale;
};
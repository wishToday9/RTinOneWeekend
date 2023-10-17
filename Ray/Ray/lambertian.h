#pragma once
#include "vec3.h"
#include "material.h"

class texture;

class lambertian : public material {
public:
	lambertian(const vec3& a) :albedo(make_shared<solid_color>(a)) {}
	lambertian(shared_ptr<texture> a) :albedo(a) {}

	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const override{
		vec3 scattered_direction = rec.normal + random_unit_vector();
		if (scattered_direction.near_zero()) {
			scattered_direction = rec.normal;
		}
		scattered = ray(rec.p, scattered_direction, r_in.time());
		attenuation = albedo->value(rec.u, rec.v, rec.p);
		return (dot(scattered.direction(), rec.normal) > 0);
	}
private:
	std::shared_ptr<texture> albedo;

};


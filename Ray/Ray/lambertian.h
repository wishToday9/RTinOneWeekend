#pragma once
#include "vec3.h"
#include "material.h"

class lambertian : public material {
public:
	lambertian(const vec3& a) :albedo(a) {}

	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const override{
		vec3 scattered_direction = rec.normal + random_unit_vector();
		if (scattered_direction.near_zero()) {
			scattered_direction = rec.normal;
		}
		scattered = ray(rec.p, scattered_direction);
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}
private:
	vec3 albedo;

};


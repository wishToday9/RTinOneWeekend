#pragma once

#include "material.h"

class dielectric : public material {
public:
	dielectric(double index_of_refraction) : ir(index_of_refraction) {}

	bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const override {
		attenuation = color(1.0, 1.0, 1.0);
		double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

		vec3 unit_direction = unit_vector(r_in.direction());
		double cos_theta = ffmin(dot(-unit_direction, rec.normal), 1.0);
		double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

		if (refraction_ratio * sin_theta > 1.0) {
			vec3 reflected = reflect(unit_direction, rec.normal);
			scattered = ray(rec.p, reflected, r_in.time());
			return true;
		}

		double reflect_prob = reflectance(cos_theta, refraction_ratio);
		if (random_double() < reflect_prob)
		{
			vec3 reflected = reflect(unit_direction, rec.normal);
			scattered = ray(rec.p, reflected, r_in.time());
			return true;
		}

		vec3 refracted = refract(unit_direction, rec.normal, refraction_ratio);
		scattered = ray(rec.p, refracted, r_in.time());
		return true;
	}
private:
	double ir; //index of refraction

	static double reflectance(double cosine, double ref_idx) {
		// Use Schlick's approximation for reflectance.
		auto r0 = (1 - ref_idx) / (1 + ref_idx);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - cosine), 5);
	}

};
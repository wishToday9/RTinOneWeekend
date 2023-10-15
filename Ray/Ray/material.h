#pragma once

#include "rtweekend.h"

#include "hittable.h"

class material {
public:
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
};
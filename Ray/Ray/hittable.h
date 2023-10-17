#pragma once

#include "ray.h"
#include "AABB.h"

class material;

struct hit_record
{
	vec3 p;
	vec3 normal;
	shared_ptr<material> mat_ptr;
	double t;
	bool front_face;
	double u;
	double v;

	inline void set_face_normal(const ray& r, const vec3& outward_normal) {
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class hittable {
public:
	virtual bool hit(const ray& r, interval t_max, hit_record& rec) const = 0;

	virtual aabb bounding_box() const = 0;

};
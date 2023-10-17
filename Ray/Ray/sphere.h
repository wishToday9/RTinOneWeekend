#pragma once

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
public:
	//stationary Sphere
	sphere(point3 _center, double _radius, shared_ptr<material> _material)
		:center1(_center), radius(_radius), mat_ptr(_material), is_moving(false)
	{
		auto rvec = vec3(radius, radius, radius);
		bbox = aabb(center1 - rvec, center1 + rvec);

	}
	//moving sphere
	sphere(point3 _center1, point3 _center2, double _radius, shared_ptr<material> _material) 
		: center1(_center1), radius(_radius), mat_ptr(_material), is_moving(true)
	{
		center_vec = _center2 - _center1;

		auto rvec = vec3(radius, radius, radius);
		aabb box1(_center1 - rvec, _center1 + rvec);
		aabb box2(_center2 - rvec, _center2 + rvec);

		bbox = aabb(box1, box2);
	}

	virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const override;
	aabb bounding_box() const override {
		return bbox;
	}

	static void get_sphere_uv(const point3& p, double& u, double& v) {
		auto theta = acos(-p.y());
		auto phi = atan2(-p.z(), p.x()) + pi;

		u = phi / (2 * pi);
		v = theta / pi;
	}

private:
	vec3 center1;
	double radius;
	shared_ptr<material> mat_ptr;
	bool is_moving;
	vec3 center_vec;
	aabb bbox;

	point3 sphere_center(double time) const {
		return center1 + time * center_vec;
	}
};

bool sphere::hit(const ray& r, interval t, hit_record& rec) const {
	point3 center = is_moving ? sphere_center(r.time()) : center1;
	vec3 oc = r.origin() - center;
	auto a = r.direction().length_squared();
	auto half_b = dot(r.direction(), oc);
	auto c = dot(oc, oc) - radius * radius;
	auto discriminant = half_b * half_b - a * c;
	if (discriminant > 0) {
		auto root = sqrt(discriminant);
		auto temp = (-half_b - root) / a;
		if (t.surrounds(temp)) {
			rec.t = temp;
			rec.p = r.at(rec.t);
			rec.mat_ptr = mat_ptr;
			vec3 outward_normal = (rec.p - center) / radius;
			rec.set_face_normal(r, outward_normal);
			get_sphere_uv(outward_normal, rec.u, rec.v);

			return true;
		}
		temp = (-half_b + root) / a;
		if (t.surrounds(temp)) {
			rec.t = temp;
			rec.p = r.at(rec.t);
			rec.mat_ptr = mat_ptr;
			vec3 outward_normal = (rec.p - center) / radius;
			rec.set_face_normal(r, outward_normal);
			get_sphere_uv(outward_normal, rec.u, rec.v);

			return true;
		}
	}
	return false;
}
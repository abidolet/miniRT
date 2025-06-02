/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjuncker <mjuncker@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 11:01:00 by mjuncker          #+#    #+#             */
/*   Updated: 2025/05/08 14:32:30 by mjuncker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/vector.h"
#include "miniRT.h"
#include "raytracer.h"
#include <limits.h>

t_vec3	ray_to_vec(t_ray r)
{
	return (vec3_add(r.origin, vec3_mult(r.dir, r.length)));
}

t_ray	get_ray(float u, float v, t_viewport vp)
{
	t_ray	ray;
	t_vec3	px_center;

	px_center = vec3_add(vp.px_up_left,
			vec3_add(vec3_mult(vp.horizontal, u), vec3_mult(vp.vertical, v)));
	ray.origin = vp.cam->origin;
	ray.dir = vec3_normalize(vec3_sub(vec3_add(px_center,
				vec3_mult(random_vec(vp.seed), RAY_RANDOMNESS)), ray.origin));
	ray.length = 100;
	return (ray);
}

t_hit	hit_fail(void)
{
	return ((t_hit){
		.point = {0, 0, 0},
		.normal = {0, 0, 0},
		.distance = -1,
		.obj_index = -1,
		.mat = default_mat(),
		.type = NONE,
	});
}

static t_hit	trace_ray2(t_ray ray, t_scene *scene, t_hit *hit)
{
	t_hit	tmp_hit;

	tmp_hit = get_min_dist(cylinders_hit, ray, (struct s_objs_data)
		{scene->cylinders, scene->nb_cylinders, sizeof(t_cylinder)});
	if (is_closest(*hit, tmp_hit, ray))
	{
		*hit = tmp_hit;
		hit->type = CYLINDER;
	}
	tmp_hit = get_min_dist(cone_hit, ray, (struct s_objs_data)
		{scene->cones, scene->nb_cones, sizeof(t_cylinder)});
	if (is_closest(*hit, tmp_hit, ray))
	{
		*hit = tmp_hit;
		hit->type = CONE;
	}
	if (hit->obj_index != (size_t) - 1)
		return (hit_succes(scene, ray, *hit));
	return (hit_fail());
}

t_hit	trace_ray(t_ray ray, t_scene *scene)
{
	t_hit	hit;
	t_hit	tmp_hit;

	ft_bzero(&hit, sizeof(t_hit));
	hit.distance = 2147483648;
	hit.obj_index = -1;
	tmp_hit = get_min_dist(sphere_hit, ray, (struct s_objs_data)
		{scene->spheres, scene->nb_spheres, sizeof(t_sphere)});
	if (tmp_hit.distance != -1 && tmp_hit.distance < ray.length)
	{
		hit = tmp_hit;
		hit.type = SPHERE;
	}
	tmp_hit = get_min_dist(plane_hit, ray, (struct s_objs_data)
		{scene->planes, scene->nb_planes, sizeof(t_plane)});
	if (is_closest(hit, tmp_hit, ray))
	{
		hit = tmp_hit;
		hit.type = PLANE;
	}
	return (trace_ray2(ray, scene, &hit));
}

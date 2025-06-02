/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjuncker <mjuncker@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 13:24:20 by mjuncker          #+#    #+#             */
/*   Updated: 2025/05/08 14:33:04 by mjuncker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/io.h"
#include "libft/math.h"
#include "libft/string.h"
#include "libft/vector.h"
#include "miniRT.h"
#include "texture.h"
#include "window.h"
#include "raytracer.h"
#include <limits.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include "texture.h"

t_hit	bounce(t_scene *scene, t_ray *ray, t_render_pass *pass)
{
	t_hit	hit;
	float	u;
	float	v;

	hit = trace_ray(*ray, scene);
	if (hit.distance == -1)
		return (hit_fail());
	if (vec3_length_square(pass->depth_map) == 0)
	{
		pass->depth_map.x = hit.distance / scene->camera.focal_length;
		pass->depth_map.y = hit.distance / scene->camera.focal_length;
		pass->depth_map.z = hit.distance / scene->camera.focal_length;
		pass->depth_map = vec3_clamp(pass->depth_map, 0, 1);
	}
	if (hit.mat.texture_map.header.type[0] != -1)
	{
		get_uv(scene, hit, &u, &v);
		hit.mat.albedo = vec3_multv(hit.mat.albedo,
				get_px(u, v, &hit.mat.texture_map));
	}
	if (hit.mat.use_checker)
		hit.mat.albedo = checker_color(hit, hit.mat);
	return (hit);
}

t_vec3	get_px_col(int i, int j, t_viewport vp, t_scene *scene)
{
	t_frame_data	frame;
	int				x;

	vp.seed = (i + (j * vp.win->width)) * scene->frame_count;
	frame = frame_data(scene, &vp, i, j);
	x = -1;
	while (++x < scene->nb_bounces)
	{
		vp.seed *= x + 1;
		frame.hit = bounce(scene, &frame.ray, frame.pass);
		if (frame.hit.distance == -1)
		{
			if (vp.win->vp_flags & AMBIENT)
				frame.final_color = vec3_add(frame.final_color,
						vec3_divide(sky_col(frame.ray, scene), x + 1));
			break ;
		}
		apply_shading(scene, &frame, &vp);
		frame.ray.origin = vec3_add(frame.hit.point,
				vec3_mult(frame.hit.normal, 0.0001));
		frame.ray.dir = vec3_reflect(frame.ray.dir, vec3_add(frame.hit.normal,
					vec3_mult(random_vec(vp.seed), frame.hit.mat.roughtness)));
	}
	return (vec3_clamp(frame.final_color, 0, 1));
}

static void	*thread_routine(void *pcontext)
{
	t_thread_context	*ctx;
	t_vec3				color;
	t_vec3				*accumulation_data;

	ctx = (t_thread_context *)pcontext;
	accumulation_data = ctx->vp.win->accumulation_data;
	while (ctx->j < ctx->vp.win->height)
	{
		while (ctx->i < ctx->vp.win->width)
		{
			color = get_px_col(ctx->i, ctx->j, ctx->vp, &ctx->scene);
			set_pixel(&ctx->vp.win->img, ctx->i, ctx->j, vec_to_int(
					process_accumulation(accumulation_data, ctx, color)
					));
			ctx->i += MAX_THREAD;
		}
		ctx->i -= ctx->vp.win->width;
		ctx->j++;
	}
	return (NULL);
}

void	render(t_viewport vp, t_scene *scene)
{
	t_thread_context	ctx[MAX_THREAD];
	size_t				i;

	i = 0;
	while (i < MAX_THREAD)
	{
		ctx[i].id = i;
		ctx[i].i = i;
		ctx[i].j = 0;
		ctx[i].scene = *scene;
		ctx[i].vp = vp;
		pthread_create(&ctx[i].th, NULL, thread_routine, (void *)&(ctx[i]));
		i++;
	}
	i = 0;
	while (i < MAX_THREAD)
	{
		pthread_join(ctx[i].th, NULL);
		i++;
	}
}

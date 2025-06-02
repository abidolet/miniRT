/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjuncker <mjuncker@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 10:27:30 by mjuncker          #+#    #+#             */
/*   Updated: 2025/05/08 14:03:44 by mjuncker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raytracer.h"

t_vec3	process_accumulation(
	t_vec3 *accumulation_data, t_thread_context *ctx, t_vec3 color)
{
	t_vec3	accumulation;

	accumulation_data[ctx->i + ctx->j * ctx->vp.win->width] = vec3_add(
			accumulation_data[ctx->i + ctx->j * ctx->vp.win->width], color
			);
	accumulation = accumulation_data[ctx->i + ctx->j * ctx->vp.win->width];
	accumulation = vec3_divide(accumulation, ctx->scene.frame_count);
	accumulation = vec3_clamp(accumulation, 0, 1);
	return (accumulation);
}

t_frame_data	frame_data(t_scene *scene, t_viewport *vp, int i, int j)
{
	return ((t_frame_data){
		.final_color = vec3_zero(),
		.hit = hit_fail(),
		.pass = &vp->win->pass[i + j * vp->width],
		.radiance = vec3_mult(scene->ambient_light.color,
			scene->ambient_light.ratio),
		.ray = get_ray((float)i / ((float)vp->width),
			(float)j / (float)vp->height, *vp)
	});
}

t_viewport	viewport(t_win_scene *win, t_scene *scene)
{
	t_viewport	vp;

	vp.win = win;
	vp.cam = &scene->camera;
	vp.width = win->width;
	vp.height = win->height;
	vp.vp_height = 2 * tan((float)vp.cam->fov / 2 * 3.1415 / 180)
		* vp.cam->focal_length;
	vp.vp_width = vp.vp_height * win->aspect_ratio;
	vp.px_up_left = vec3_add(vp.cam->origin,
			vec3_add(
				vec3_sub(
					vec3_mult(vp.cam->forward, vp.cam->focal_length),
					vec3_mult(vp.cam->right, vp.vp_width / 2)
					),
				vec3_mult(vp.cam->up, vp.vp_height / 2)
				)
			);
	vp.horizontal = vec3_mult(vp.cam->right, vp.vp_width);
	vp.vertical = vec3_mult(vec3_mult(vp.cam->up, -1), vp.vp_height);
	return (vp);
}

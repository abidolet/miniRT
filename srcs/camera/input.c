/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjuncker <mjuncker@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:33:51 by mjuncker          #+#    #+#             */
/*   Updated: 2025/05/06 13:32:36 by mjuncker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "camera.h"
#include "libft/io.h"
#include "miniRT.h"
#include "raytracer.h"
#include "window.h"
#include "parser.h"
#include "texture.h"

int	mouse_down(int button, int x, int y, t_prog *prog)
{
	(void)x;
	(void)y;
	if (button == 4)
	{
		prog->scene->camera.fov += 5;
		reset_accumulation(prog);
	}
	if (button == 5)
	{
		prog->scene->camera.fov -= 5;
		reset_accumulation(prog);
	}
	prog->scene->camera.fov = ft_clamp(prog->scene->camera.fov, 0, 180);
	if (button == 3)
	{
		mlx_mouse_get_pos(prog->win_scene->mlx_ptr,
			prog->win_scene->win_ptr, &prog->scene->camera.last_x,
			&prog->scene->camera.last_y);
		prog->scene->camera.movement_enable = 1;
		prog->scene->old_bounces = prog->scene->nb_bounces;
		prog->scene->nb_bounces = 1;
		ft_log(LOG, "camera rotation enable");
	}
	return (0);
}

int	mouse_up(int button, int x, int y, t_prog *prog)
{
	(void)x;
	(void)y;
	if (button == 3)
	{
		prog->scene->nb_bounces = prog->scene->old_bounces;
		prog->scene->camera.movement_enable = 0;
		ft_log(LOG, "camera rotation disable");
	}
	return (0);
}

void	reload(t_prog *prog)
{
	ft_log(LOG, "reloading scene...");
	init(prog, NULL);
	reset_accumulation(prog);
	ft_log(SUCCESS, "scene loaded");
}

int	key_down(int keycode, t_prog *prog)
{
	if (toggle_effects(keycode, prog))
		return (0);
	if (toggle_movement(keycode, prog))
		return (0);
	if (toggle_pass(keycode, prog))
		return (0);
	if (cam_input(keycode, prog))
		return (0);
	else if (keycode == ESC)
		free_all(prog);
	else if (keycode == 'k')
		print_cam(&prog->scene->camera);
	else if (keycode == '0')
		reload(prog);
	else if (keycode == 'm')
		print_scene(prog->scene);
	else if (keycode == 'f')
		prog->win_scene->vp_flags ^= SHOW_FRAME;
	else if (keycode == 'g')
		save_image_to_ppm(prog->win_scene);
	return (0);
}

int	key_up(int keycode, t_prog *prog)
{
	if (toggle_movement(keycode, prog))
		return (0);
	return (0);
}

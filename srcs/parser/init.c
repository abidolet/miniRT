/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjuncker <mjuncker@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 10:22:15 by abidolet          #+#    #+#             */
/*   Updated: 2025/05/08 08:59:46 by mjuncker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include "parser.h"
#include "window.h"
#include "raytracer.h"
#include "button.h"

void	switch_identifier(t_prog *prog, t_parser *parser)
{
	if (!ft_strcmp(parser->tokens[0], "A"))
	{
		if (parser->ambient_is_set)
			print_exit(prog, "Ambient light can only be declared once");
		parser->ambient_is_set = true;
	}
	else if (!ft_strcmp(parser->tokens[0], "C"))
	{
		if (parser->camera_is_set)
			print_exit(prog, "Camera can only be declared once");
		parser->camera_is_set = true;
	}
	else if (!ft_strcmp(parser->tokens[0], "L"))
		prog->scene->nb_lights++;
	else if (!ft_strcmp(parser->tokens[0], "sp"))
		prog->scene->nb_spheres++;
	else if (!ft_strcmp(parser->tokens[0], "pl"))
		prog->scene->nb_planes++;
	else if (!ft_strcmp(parser->tokens[0], "cy"))
		prog->scene->nb_cylinders++;
	else if (!ft_strcmp(parser->tokens[0], "co"))
		prog->scene->nb_cones++;
	else if (parser->tokens[0])
		print_exit(prog, "Invalid identifier");
}

void	init_malloc(t_prog *prog)
{
	check_mem((t_info){__FILE__, __LINE__, __func__},
		malloc(sizeof(t_light_source) * (prog->scene->nb_lights + 1)),
		(void **)&prog->scene->lights, prog);
	check_mem((t_info){__FILE__, __LINE__, __func__},
		malloc(sizeof(t_sphere) * (prog->scene->nb_spheres + 1)),
		(void **)&prog->scene->spheres, prog);
	check_mem((t_info){__FILE__, __LINE__, __func__},
		malloc(sizeof(t_plane) * (prog->scene->nb_planes + 1)),
		(void **)&prog->scene->planes, prog);
	check_mem((t_info){__FILE__, __LINE__, __func__},
		malloc(sizeof(t_cylinder) * (prog->scene->nb_cylinders + 1)),
		(void **)&prog->scene->cylinders, prog);
	check_mem((t_info){__FILE__, __LINE__, __func__},
		malloc(sizeof(t_cylinder) * (prog->scene->nb_cones + 1)),
		(void **)&prog->scene->cones, prog);
}

void	parse_file(t_prog *prog, t_parser *parser, char *file)
{
	t_list	*new_node;

	prog->parser->fd = open(file, O_RDONLY);
	if (prog->parser->fd == -1)
		print_exit(prog, "File not found");
	parser->line = ft_get_next_line(parser->fd);
	while (parser->line)
	{
		if (ft_strchr(parser->line, '\n'))
			parser->line[ft_strlen(parser->line) - 1] = '\0';
		check_mem((t_info){__FILE__, __LINE__, __func__},
			ft_split(parser->line, ' '),
			(void **)&parser->tokens, prog);
		switch_identifier(prog, parser);
		check_mem((t_info){__FILE__, __LINE__, __func__},
			ft_lstnew(parser->tokens), (void **)&new_node, prog);
		ft_lstadd_back(&parser->map, new_node);
		free(parser->line);
		parser->line = ft_get_next_line(parser->fd);
	}
	parser->tokens = NULL;
	if (parser->ambient_is_set == false
		|| parser->camera_is_set == false
		|| prog->scene->nb_lights == 0)
		print_exit(prog, "Missing mandatory elements");
}

void	init(t_prog *prog, char *file)
{
	static char	*filename = NULL;

	if (filename == NULL)
		filename = file;
	if (SSAA_FACTOR != (int)SSAA_FACTOR || SSAA_FACTOR < 1)
		print_exit(NULL, "SSAA_FACTOR is invalid");
	if (DEFAULT_BOUNCE != (int)DEFAULT_BOUNCE || DEFAULT_BOUNCE < 1)
		print_exit(NULL, "DEFAULT_BOUNCE is invalid");
	if (WIDTH * HEIGHT * SSAA_FACTOR * SSAA_FACTOR * SSAA_FACTOR * SSAA_FACTOR
		* sizeof(t_render_pass) > (MAX_RAM * (long long)1000000000))
		print_exit(NULL, "not enough memory");
	ft_bzero(prog->parser, sizeof(t_parser));
	ft_bzero(prog->scene, sizeof(t_scene));
	prog->scene->camera.right = (t_vec3){1, 0, 0};
	prog->scene->camera.up = (t_vec3){0, 1, 0};
	parse_material(prog, prog->parser);
	parse_file(prog, prog->parser, filename);
	init_malloc(prog);
	parse(prog, prog->parser, prog->scene, prog->parser->map);
	prog->scene->frame_count = 1;
	prog->scene->nb_bounces = DEFAULT_BOUNCE;
	print_scene(prog->scene);
	ft_log(SUCCESS, "No error has been found");
}

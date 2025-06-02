/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjuncker <mjuncker@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 12:21:55 by abidolet          #+#    #+#             */
/*   Updated: 2025/05/06 12:33:22 by mjuncker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "window.h"
#include "parser.h"
#include "texture.h"
#include "button.h"

void	free_arr(void **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

void	print_exit(t_prog *prog, char *str)
{
	print_error(str);
	if (prog)
		free_all(prog);
	exit(1);
}

void	free_parser(t_parser *parser)
{
	if (parser->is_free == false)
	{
		if (parser->fd_mat > 0)
			close(parser->fd_mat);
		if (parser->fd > 0)
			close(parser->fd);
		ft_lstclear(&parser->mat_chained, (void *)free_arr);
		free(parser->line);
		free_arr((void **)parser->tokens);
		ft_lstclear(&parser->map, (void *)free_arr);
		parser->is_free = true;
	}
}

void	clean_materials(t_mat *mats, size_t nb_mat)
{
	size_t	i;

	if (!mats)
		return ;
	i = 0;
	while (i < nb_mat)
	{
		if (is_header_valid(&mats[i].normal_map.header))
			free(mats[i].normal_map.values);
		if (is_header_valid(&mats[i].texture_map.header))
			free(mats[i].texture_map.values);
		i++;
	}
}

void	free_all(t_prog *prog)
{
	free(prog->scene->lights);
	free(prog->scene->spheres);
	free(prog->scene->planes);
	free(prog->scene->cylinders);
	free(prog->scene->cones);
	clean_materials(prog->scene->materials, prog->scene->nb_materials);
	free(prog->scene->materials);
	free_parser(prog->parser);
	free_win(prog->win_scene, prog->win_button);
	exit(0);
}

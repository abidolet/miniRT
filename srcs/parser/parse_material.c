/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_material.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjuncker <mjuncker@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 10:00:21 by abidolet          #+#    #+#             */
/*   Updated: 2025/05/06 12:34:45 by mjuncker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "parser.h"
#include <fcntl.h>
#include "texture.h"

static void	check_duplicate_name(t_prog *prog, t_mat *mat, int nb_mat)
{
	int	i;
	int	j;

	i = 0;
	while (i < nb_mat)
	{
		j = i + 1;
		while (j < nb_mat)
		{
			if (ft_strcmp(mat[i].name, mat[j].name) == 0)
				print_exit(prog, "Duplicate material name");
			j++;
		}
		i++;
	}
}

int	find_material_index(t_prog *prog, char *material_name)
{
	unsigned int	i;

	if (!prog->scene->materials)
		print_exit(prog, "No materials loaded");
	i = 0;
	while (i < prog->scene->nb_materials)
	{
		if (ft_strcmp(prog->scene->materials[i].name, material_name) == 0)
			return (i);
		i++;
	}
	return (-1);
}

static void	fill_material(t_prog *prog, t_mat *mat, t_list *current)
{
	char	**tokens;
	int		i;

	prog->parser->tokens = NULL;
	check_mem((t_info){__FILE__, __LINE__, __func__},
		ft_calloc(sizeof(t_mat), (prog->scene->nb_materials + 1)),
		(void **)&mat, prog);
	prog->scene->materials = mat;
	i = 0;
	while (current)
	{
		tokens = (char **)current->content;
		if (ft_arrlen(tokens) != 9)
			print_exit(prog, "Invalid material format");
		mat[i].name = tokens[0];
		parse_color(prog, &mat[i].albedo, tokens[1]);
		mat[i].emission_power = check_atof(prog, tokens[2]);
		mat[i].roughtness = check_atof(prog, tokens[3]);
		mat[i].shyniness = check_atof(prog, tokens[4]);
		mat[i].spec_coef = check_atof(prog, tokens[5]);
		mat[i].use_checker = (bool)check_atof(prog, tokens[6]);
		mat[i].texture_map = ppm_image(tokens[7], prog);
		mat[i++].normal_map = ppm_image(tokens[8], prog);
		current = current->next;
	}
}

void	parse_material(t_prog *prog, t_parser *parser)
{
	t_list	*new_node;

	parser->fd_mat = open(MATERIAL_FILE, O_RDONLY);
	if (parser->fd_mat == -1)
		print_exit(prog, "material file not found");
	parser->line = ft_get_next_line(parser->fd_mat);
	while (parser->line)
	{
		if (ft_strchr(parser->line, '\n'))
			parser->line[ft_strlen(parser->line) - 1] = '\0';
		check_mem((t_info){__FILE__, __LINE__, __func__},
			ft_split(parser->line, ' '),
			(void **)&parser->tokens, prog);
		check_mem((t_info){__FILE__, __LINE__, __func__},
			ft_lstnew(parser->tokens), (void **)&new_node, prog);
		ft_lstadd_back(&parser->mat_chained, new_node);
		prog->scene->nb_materials++;
		free(parser->line);
		parser->line = ft_get_next_line(parser->fd_mat);
	}
	if (prog->scene->nb_materials == 0)
		print_exit(prog, "No materials found");
	fill_material(prog, prog->scene->materials, parser->mat_chained);
	check_duplicate_name(prog, prog->scene->materials,
		prog->scene->nb_materials);
}

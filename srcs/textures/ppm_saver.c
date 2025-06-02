/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ppm_saver.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjuncker <mjuncker@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 11:11:54 by mjuncker          #+#    #+#             */
/*   Updated: 2025/05/06 12:00:50 by mjuncker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "texture.h"
#include "window.h"
#include "libft/io.h"
#include "raytracer.h"
#include <unistd.h>
#include <fcntl.h>

void	write_header(t_ppm_header *header, int fd)
{
	ft_dprintf(fd, "P6\n", header->type);
	ft_dprintf(fd, "%u %u\n", WIDTH, HEIGHT);
	ft_dprintf(fd, "%u\n", header->max_values);
}

void	write_values(t_win_scene *win, int fd)
{
	int		i;
	int		j;
	t_vec3	color;

	j = 0;
	while (j < HEIGHT)
	{
		i = 0;
		while (i < WIDTH)
		{
			color = int_to_vec(get_pixel(&win->final_image, i, j));
			color = vec3_mult(color, 255);
			ft_dprintf(fd, "%c%c%c",
				(char)color.x, (char)color.y, (char)color.z);
			i++;
		}
		show_progress(i + j * WIDTH,
			HEIGHT * WIDTH, "saving image");
		j++;
	}
}

char	*create_save_path(void)
{
	char				*name;
	struct s_string_ass	strings[5];

	strings[0].string = DEFAULT_RENDER_PATH;
	strings[0].to_free = 0;
	strings[1].string = "miniRT_render_";
	strings[1].to_free = 0;
	strings[2].string = ft_itoa(ft_abs((int)get_current_time_ms()));
	strings[2].to_free = 1;
	strings[3].string = ".ppm";
	strings[3].to_free = 0;
	strings[4].string = NULL;
	name = ft_strassemble(strings);
	if (!name)
		ft_log(ERROR, "cannot create path");
	return (name);
}

void	save_image_to_ppm(t_win_scene *win)
{
	t_ppm_header	header;
	char			*name;
	int				fd;

	header.type[0] = 'P';
	header.type[0] = '6';
	header.width = win->width;
	header.height = win->height;
	header.max_values = 255;
	name = create_save_path();
	if (name == NULL)
		return ;
	fd = open(name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		ft_log(ERROR, "can't open %s", name);
		free(name);
		return ;
	}
	write_header(&header, fd);
	write_values(win, fd);
	ft_log(SUCCESS, "%s has been saved", name);
	free(name);
	close(fd);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ppm_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjuncker <mjuncker@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 12:29:14 by mjuncker          #+#    #+#             */
/*   Updated: 2025/05/06 09:48:06 by mjuncker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/vector.h"
#include "texture.h"
#include "libft/io.h"

int	is_header_valid(t_ppm_header *header)
{
	return (header->type[0] == 'P' && header->type[1] == '6');
}

void	print_ppm_header(t_ppm_header header, const char *name)
{
	ft_log(LOG, "%s header:", name);
	ft_printf("\ttype:........%s\n", header.type);
	ft_printf("\twidth:.......%u\n", header.width);
	ft_printf("\theight:......%u\n", header.height);
	ft_printf("\tvalues:......%u\n", header.max_values);
}

t_vec3	get_px(float u, float v, t_ppm_image *image)
{
	int	x;
	int	y;

	x = image->header.width * u;
	y = image->header.height * v;
	x = ft_clamp(x, 0, image->header.width - 1);
	y = ft_clamp(y, 0, image->header.height - 1);
	return (image->values[x + image->header.width * y]);
}

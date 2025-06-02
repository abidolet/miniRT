/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjuncker <mjuncker@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 15:02:36 by mjuncker          #+#    #+#             */
/*   Updated: 2025/05/08 14:32:08 by mjuncker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WINDOW_H
# define WINDOW_H

# include "miniRT.h"
# include <mlx.h>
# include <sys/types.h>

/* -------------------------------------------------------------------------- */
/*                                window config                               */
/* -------------------------------------------------------------------------- */

# ifndef WIDTH
#  define WIDTH 480
# endif

# ifndef HEIGHT
#  define HEIGHT 360
# endif

# ifndef SSAA_FACTOR
#  define SSAA_FACTOR 1
# endif

# ifndef MAX_RAM
#  define MAX_RAM 1
# endif

/* --------------------------------- effect --------------------------------- */
# define INVERT 0b00000001
# define DEPTH_OF_FIELD 0b00000010
# define PIXEL 0b00010000
# define CHROMA 0b00100000
# define GRAYSCALE 0b01000000
# define POSTERIZE 0b10000000
# define ANTIALIASING 0b100000000

/* ----------------------------- viewport flags ----------------------------- */
# define AMBIENT 0b0001
# define DIFFUSE 0b0010
# define SPECULAR 0b0100
# define NORMAL 0b1000
# define UV 0b10000
# define DEPTH_MAP 0b100000

/* ----------------------------------- MLX ---------------------------------- */

# define ON_KEYDOWN 2
# define ON_KEYUP 3
# define ON_MOUSEDOWN 4
# define ON_MOUSEUP 5
# define ON_MOUSEMOVE 6
# define ON_EXPOSE 12
# define ON_DESTROY 17

/* -------------------------------------------------------------------------- */
typedef struct s_data
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}	t_data;

typedef struct gaussien_dof
{
	float	*kernel;
	int		size;
	int		half;
	t_vec3	final;
}	t_gaussien_dof;

typedef struct render_pass
{
	t_vec3	ambient;
	t_vec3	diffuse;
	t_vec3	specular;
	t_vec3	depth_map;
	t_vec3	uv;
	t_vec3	normal;
	t_vec3	merged_pass;
}	t_render_pass;

typedef struct s_win_scene
{
	int				width;
	int				height;
	int				half_width;
	int				half_height;
	int				scale_factor;

	int				view_width;
	int				view_height;
	double			aspect_ratio;

	t_vec3			*accumulation_data;
	t_render_pass	*pass;

	void			*win_ptr;
	void			*mlx_ptr;

	t_data			img;
	t_data			final_image;
	t_uint			img_flags;
	t_uint			vp_flags;

	bool			paused;
}	t_win_scene;

/* ------------------------------- END BUTTON ------------------------------ */

// image.c
int				create_img(t_win_scene *win);
void			set_pixel(t_data *data, int x, int y, int color);
int				create_trgb(int t, int r, int g, int b);
unsigned int	get_pixel(t_data *data, int x, int y);
void			display_frame(t_win_scene *win, t_scene *scene);

// window.c
void			init_win(t_prog *prog);
int				window_close(void *prog);
void			free_win(t_win_scene *win, t_win_button *win_btn);

void			run_pipeline(t_prog *prog);
int				new_frame(t_prog *prog);

// button.c
void			init_button_window(t_prog *prog);

// effects.c
void			invert_effect(t_win_scene *win, int i, int j);
void			depth_of_field(t_win_scene *win, int i, int j);
void			vp_filter(t_win_scene *win, int i, int j);
void			chromatic_aberation(t_win_scene *win, int i, int j);
void			grayscale(t_win_scene *win, int i, int j);
void			pixelate(t_win_scene *win, int i, int j);
void			posterize(t_win_scene *win, int i, int j);

// depth_of_field.c
t_gaussien_dof	new_dof(int size);
void			create_blur_pixel(t_gaussien_dof *dof,
					int i, int j, t_win_scene *win);

// input_utils.c
int				toggle_effects(int keycode, t_prog *prog);
int				toggle_movement(int keycode, t_prog *prog);
int				toggle_pass(int keycode, t_prog *prog);
int				cam_input(int keycode, t_prog *prog);

#endif

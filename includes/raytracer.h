/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjuncker <mjuncker@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 09:09:16 by mjuncker          #+#    #+#             */
/*   Updated: 2025/05/08 14:32:00 by mjuncker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RAYTRACER_H
# define RAYTRACER_H

# include "miniRT.h"
# include <math.h>
# include <pthread.h>
# include <stddef.h>
# include "window.h"

# ifndef DEFAULT_BOUNCE
#  define DEFAULT_BOUNCE 10
# endif

# ifndef RAY_RANDOMNESS
#  define RAY_RANDOMNESS 0.02
# endif

# ifndef MAX_THREAD
#  define MAX_THREAD 20
# endif

# define RENDER_PAUSE 0b100000
# define SHOW_FRAME 0b1000000
# define LIGHT_RANGE 10

# ifndef M_PI
#  define M_PI 3.14159265358979323846
# endif

enum	e_object_type
{
	NONE = 0,
	PLANE,
	SPHERE,
	CYLINDER,
	CONE
};

typedef struct ray
{
	t_vec3	origin;
	t_vec3	dir;

	int		length;
}	t_ray;

typedef struct hit_info
{
	t_vec3				point;
	t_vec3				normal;
	double				distance;

	size_t				obj_index;
	t_mat				mat;
	enum e_object_type	type;

}	t_hit;

typedef struct viewport
{
	t_win_scene		*win;

	double			vp_height;
	double			vp_width;

	t_vec3			px_up_left;

	t_camera		*cam;

	t_vec3			vertical;
	t_vec3			horizontal;

	int				width;
	int				height;

	int				seed;
}	t_viewport;

typedef struct s_light_info
{
	t_light_source	light;
	t_vec3			light_dir;
	float			attenuation;

	t_mat			mat;

	t_hit			hit;
	t_ray			ray;
}	t_light_info;

struct s_objs_data
{
	void	*objs;
	size_t	n_obj;
	size_t	size;
};

typedef struct s_thread_contex
{
	t_uint		id;

	int			i;
	int			j;

	pthread_t	th;
	t_viewport	vp;
	t_win_scene	*win;
	t_scene		scene;
}	t_thread_context;

typedef struct s_thread_aliasing
{
	t_uint		id;

	int			start_row;
	int			end_row;

	pthread_t	th;
	t_win_scene	*win;
}	t_thread_aliasing;

typedef struct frame_data
{
	t_ray			ray;
	t_vec3			final_color;
	t_vec3			radiance;
	t_hit			hit;
	t_render_pass	*pass;
}	t_frame_data;

// colors.c
t_vec3			normal_color(t_hit hit);
t_uint			vec_to_int(const t_vec3 color);
t_vec3			int_to_vec(t_uint color);
t_vec3			sky_col(t_ray ray, t_scene *scene);

// ray.c
t_ray			get_ray(float u, float v, t_viewport vp);
t_hit			hit_fail(void);
t_vec3			ray_to_vec(t_ray r);

// intersection.c
t_hit			trace_ray(t_ray ray, t_scene *scene);
t_vec3			cylinder_normal(t_cylinder cy, t_vec3 hit_point);
t_vec3			cone_normal(t_cylinder co, t_vec3 hit_point);
t_hit			get_min_dist(double (*f)(void*, t_ray), t_ray ray,
					struct s_objs_data data);
int				is_closest(t_hit hit, t_hit new_hit, t_ray ray);

// renderer_utils.c
t_vec3			process_accumulation(
					t_vec3 *accumulation_data, t_thread_context *ctx,
					t_vec3 color);
t_frame_data	frame_data(t_scene *scene, t_viewport *vp, int i, int j);
t_viewport		viewport(t_win_scene *win, t_scene *scene);

// renderer.c
t_vec3			get_px_col(int i, int j, t_viewport vp, t_scene *scene);
void			render(t_viewport vp, t_scene *scene);
t_viewport		viewport(t_win_scene *win, t_scene *scene);

// sphere.c
double			sphere_hit(void *p_sphere, const t_ray ray);
t_hit			hit_result(const t_vec3 origin, const t_ray r, const double t,
					const size_t idx);

// place.c
double			plane_hit(void *p_plane, const t_ray ray);
double			cylinders_hit(void *p, t_ray ray);

// cone.c
double			cone_hit(void *p, t_ray ray);

// lighting.c
void			apply_shading(t_scene *scene,
					t_frame_data *frame, t_viewport *vp);
t_vec3			recalculate_normal(t_scene *scene, t_hit hit,
					t_render_pass *pass, t_vec3 *world_normal);
t_light_info	new_info(
					t_light_source light, t_hit hit, t_mat mat, t_ray ray);
int				in_light(t_scene *scene, t_hit hit, struct s_light_info infos);

// phong_model.c
void			phong_shading(t_scene *scene, t_frame_data *frame);
t_vec3			phong_diffuse(struct s_light_info info);
t_vec3			phong_ambient(t_frame_data *frame);
t_vec3			phong_specular(struct s_light_info info);

// checker.c
t_vec3			checker_color(t_hit	hit, t_mat mat);

// hit.c
t_hit			hit_succes(t_scene *scene, t_ray ray, t_hit hit);

#endif

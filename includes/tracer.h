/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tracer.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 13:30:17 by abaudot           #+#    #+#             */
/*   Updated: 2021/04/21 13:39:37 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TRACER_H

# define TRACER_H
# include "vectors.h"
# include <pthread.h>
# include <mlx.h>
# include <float.h>

# define PI_180			0.01745329251
# define INVPI			0.31830988618
# define DIV			0.00003051757
# define POINT_OFFSET	3893343

# define TILESIZE		16

# ifndef NUM_THREADS
#  define NUM_THREADS	1
# endif
# ifndef ANTIALIASING
#  define ANTIALIASING	1
# elif ANTIALIASING < 1
#  define ANTIALIASING	1
# endif
# ifndef ANTI2
#  define ANTI2		ANTIALIASING
# endif

typedef struct			s_tdata
{
	const struct s_bvhl	*s;
	const t_cam			*cam;
	uint32_t			ntlsx;
	uint32_t			ntlsy;
	uint32_t			numtiles;
	uint32_t			res;
	uint32_t			id;
}						t_tdata;

/*
** SetUp Functions
*/
void					set_cam(t_cam *cam, const float xres);
uint8_t					setup(struct s_scene *s, struct s_mlx *mlx);

/*
**	Mains render functions
*/
void					genray(const t_cam *cam, t_ray *ray, const float i,
		const float j);
void					render(const struct s_scene *s, const t_cam *cam);
void					getcolor(const struct s_bvhl *s, t_ray *r,
		t_vec3f color, uint32_t d);
void					getbackground(const struct s_skybox *sky,
		const t_vec3f ray, t_vec3f color);
/*
** Normals functions
*/
typedef void			(*t_normals)(const void *, t_hinfo *);

void					sphr_nrml(const void *sphr, t_hinfo *hi);
void					pln_nrml(const void *const pln, t_hinfo *hi);
void					trgl_nrml(const void *const trgl, t_hinfo *hi);
void					sqr_nrml(const void *const sqr, t_hinfo *hi);
void					cyl_nrml(const void *const cyl, t_hinfo *hi);
void					cps_nrml(const void *const cps, t_hinfo *hi);

/*
**	Materials
*/
typedef void			(*t_shaders)(const struct s_bvhl*, const t_hinfo*,
		t_vec3f);

void					specularl(const struct s_bvhl *s, const t_hinfo *hi,
		t_vec3f color);
void					diffusel(const struct s_bvhl *s, const t_hinfo *hi,
		t_vec3f color);
void					rflctl(const struct s_bvhl *s, const t_hinfo *hi,
		t_vec3f color);
void					reflectvec(const t_vec3f v, const t_vec3f n,
		t_ray *r);

/*
**	Texture - colors
*/
typedef void			(*t_colors)(t_vec3f, const t_hinfo*,
		const t_material*);

void					onlycolor(t_vec3f color, const t_hinfo *h,
		const t_material *m);
void					normalraindow(t_vec3f color, const t_hinfo *h,
		const t_material *m);
void					checkerboard(t_vec3f color, const t_hinfo *h,
		const t_material *m);
void					marble(t_vec3f color, const t_hinfo *h,
		const t_material *m);
void					turbulence(t_vec3f color, const t_hinfo *h,
		const t_material *m);
void					wood(t_vec3f color, const t_hinfo *h,
		const t_material *m);
void					fromtext(t_vec3f color, const t_hinfo *h,
		const t_material *m);
/*
**	Color utils
*/
float					noise(t_vec3f in);
void					setcolorfromint(int32_t	c, t_vec3f color);

#endif

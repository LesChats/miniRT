/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tracer.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 13:30:17 by abaudot           #+#    #+#             */
/*   Updated: 2021/04/20 15:22:27 by abaudot          ###   ########.fr       */
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

# define TILESIZE		16

# ifndef NUM_THREADS
#  define NUM_THREADS	1
# endif
# ifndef ANTIALIASING
#  define ANTIALIASING	1
# elif ANTIALIASING < 1
#  define ANTIALIASING	1
# endif
# define ANTI2			ANTIALIASING * ANTIALIASING

typedef struct			s_tdata
{
	const struct s_bvhL	*s;
	const t_cam			*cam;
	uint32_t			ntlsX;
	uint32_t			ntlsY;
	uint32_t			numtiles;
	uint32_t			res;
	uint32_t			id;
}						t_tdata;

/*
** SetUp Functions
*/

void			set_cam(t_cam *cam, const float xres);
uint8_t			setup(struct s_scene *s, struct s_mlx *mlx);

/*
**	Mains render functions
*/

void			genRay(const t_cam *cam, t_ray *ray, const float i,
	   	const float j);
void			render(const struct s_scene *s, const t_cam *cam);
void			getColor(const struct s_bvhL *s, t_ray *r, t_vec3f color,
	   	uint32_t d);
void			getBackGround(const struct s_skybox *sky, const t_vec3f ray,
		t_vec3f color);
/*
** Normals functions
*/
typedef void	(*t_normals)(const void *, t_hInfo *);

void			sphr_nrml(const void * sphr, t_hInfo *hi);
void			pln_nrml(const void *const pln,	t_hInfo *hin);
void			trgl_nrml(const void *const trgl, t_hInfo *hi);
void			sqr_nrml(const void *const sqr, t_hInfo *hi);
void			cyl_nrml(const void *const cyl, t_hInfo *hi);
void			cps_nrml(const void *const cps, t_hInfo *hi);

/*
**	Materials
*/
typedef void	(*t_shaders)(const struct s_bvhL*, const t_hInfo*, t_vec3f);

void			specularL(const struct s_bvhL *s, const t_hInfo *hi,
		t_vec3f color);
void			diffuseL(const struct s_bvhL *s, const t_hInfo *hi,
		t_vec3f color);
void			rflctL(const struct s_bvhL *s, const t_hInfo *hi,
		t_vec3f color);
void			reflectVec(const t_vec3f v, const t_vec3f n,
		t_ray *r);

/*
**	Texture - colors
*/
typedef void	(*t_colors)(t_vec3f, const t_hInfo*, const t_material*);

void			onlycolor(t_vec3f color, const t_hInfo *h,
		const t_material *m);
void			normalRaindow(t_vec3f color, const t_hInfo *h,
		const t_material *m);
void			checkerboard(t_vec3f color, const t_hInfo *h,
		const t_material *m);
void			marble(t_vec3f color, const t_hInfo *h,
		const t_material *m);
void			turbulence(t_vec3f color, const t_hInfo *h,
		const t_material *m);
void			wood(t_vec3f color, const t_hInfo *h,
		const t_material *m);
void 			fromText(t_vec3f color, const t_hInfo *h,
		const t_material *m);
/*
**	Color utils
*/
float			noise(t_vec3f in);
void			setColorFromInt(int32_t	c, t_vec3f color);
#endif


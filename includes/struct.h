/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <abaudot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/23 11:57:15 by abaudot           #+#    #+#             */
/*   Updated: 2021/04/20 19:04:05 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H

# define STRUCT_H
# include <stdint.h>

typedef float	t_vec3f[3];

/*
**	number of primitives : spheres, plane, square, cylinder, trinangle
** +	cam & light
*/
enum				e_mat
{
	diff = 0,
	spec,
	refl
};

enum				e_col
{
	simple = 0,
	rainbow,
	checkerboard_c,
	marble_c,
	turbulance_c,
	wood_c,
	text_c
};

struct				s_text
{
	void			*img;
	int				*addr;
	int				hght;
	int				wdth;
};

typedef struct		s_material
{
	struct s_text	text;
	t_vec3f			color;
	t_vec3f			color2;
	uint32_t		spec;
	float			reflr;
	float			scale;
	float			bump;
	enum e_col		colortype;
	enum e_mat		type;
}					t_material;

/*
**	PRIMITIVES
*/

typedef struct		s_sphr
{
	t_vec3f			center;
	float			radius;
	float			radius2;

}					t_sphr;

typedef struct		s_pln
{
	t_vec3f			pts;
	t_vec3f			n;
}					t_pln;

typedef struct		s_sqr
{
	t_vec3f			pts;
	t_vec3f			n;
	t_vec3f			max;
	t_vec3f			min;
}					t_sqr;

typedef struct		s_cyl
{
	float			radius;
	float			radius2;
	t_vec3f			aa;
	float			aa_length;
	t_vec3f			pa;
	t_vec3f			pb;
}					t_cyl;

typedef struct		s_trgl
{
	t_vec3f			p1;
	t_vec3f			p2;
	t_vec3f			p3;
	t_vec3f			e1;
	t_vec3f			e2;
	t_vec3f			n;
}					t_trgl;

typedef struct		s_cps
{
	t_vec3f			pa;
	t_vec3f			pb;
	t_vec3f			ba;
	float			baba;
	float			r;
	float			r2;
}					t_cps;

/*
**	BVH structures
*/
typedef struct		s_prmtv
{
	void			*prmtv;
	t_material		*mtrl;
	uint8_t			type;
}					t_prmtv;

struct				s_prmtvs
{
	t_prmtv			*prmtvs;
	uint32_t		size;
};

typedef struct		s_box
{
	t_vec3f			max;
	t_vec3f			min;
	t_vec3f			extent;
}					t_box;

typedef struct		s_node
{
	t_box			bbox;
	uint32_t		start;
	uint32_t		prmtv_cnt;
	uint32_t		offset;
}					t_node;

struct				s_bvh
{
	t_node			*nodes;
	struct s_prmtvs	prmtvs;
};

/*
**	lights
*/
typedef struct		s_lght
{
	t_vec3f			color;
	t_vec3f			pts;
}					t_lght;

struct				s_lghts
{
	t_lght			*lghts;
	t_vec3f			amb;
	uint32_t		count;
};

/*
**	cameras
*/

typedef struct		s_cam
{
	uint32_t		*addr;
	void			*img;
	t_vec3f			pts;
	t_vec3f			orient;
	t_vec3f			left;
	t_vec3f			hrzt;
	t_vec3f			vrtl;
	float			fov;
	float			inv_x;
	float			inv_y;
	uint8_t			filter;
}					t_cam;

struct				s_cams
{
	t_cam			*cams;
	uint32_t		count;
};

struct				s_skybox
{
	void			*skyimg[6];
	int				*skybox[6];
	int32_t			widthu;
	int32_t			heightv;
};

struct				s_bvhl
{
	struct s_bvh	bvh;
	struct s_lghts	lghts;
	struct s_skybox	sky;
};

/*
**	Main data structure
*/
struct				s_scene
{
	struct s_cams	cams;
	struct s_bvhl	render;
	void			*prmtvs;
	t_material		*mtrls;
	uint32_t		count;
	uint32_t		rsltn[2];
};

/*
**	mlx_struct
*/
struct				s_mlx
{
	struct s_scene	*s;
	void			*mlx;
	void			*mlx_win;
	uint32_t		cam_id;
	int				endian;
};

/*
**	ray
*/
typedef struct		s_ray
{
	t_vec3f			o;
	t_vec3f			d;
	t_vec3f			inv_d;
}					t_ray;

struct				s_hit
{
	float			t;
	float			min;
	uint32_t		p;
};

typedef struct		s_hinfo
{
	t_ray			*r;
	t_vec3f			view;
	t_vec3f			n;
	uint32_t		spec;
	float			refl;
	uint32_t		deep;
}					t_hinfo;
#endif

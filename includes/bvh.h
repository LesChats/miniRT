/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <abaudot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/20 14:15:50 by abaudot           #+#    #+#             */
/*   Updated: 2021/05/23 20:37:00 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BVH_H
# define BVH_H
# include "struct.h"
# include "vectors.h"

# define UNTOUCH		0xffffffff
# define TOUCH			0xfffffffd
# define LEAF			2

struct					s_buildentry
{
	uint32_t			parent;
	uint32_t			start;
	uint32_t			end;
};

struct					s_buildstak
{
	struct s_buildentry	stack[128];
	uint32_t			ptr;
};

typedef struct s_traversal
{
	uint32_t			i;
	float				mint;
}						t_traversal;

struct					s_travarray
{
	t_traversal			arr[64];
	int32_t				stkptr;
};

/*
**	Bounding box functions
*/
void					expandtoinclude_vec(const t_vec3f v, t_box *bbox);
void					expandtoinclude(const t_box *v, t_box *bbox);
void					getcenter(const t_box *bbox, t_vec3f v);
uint32_t				maxdimension(const t_box *bbox);
void					swapp(t_prmtv *a, t_prmtv *b);

/*
**	Bounding functions
*/
typedef uint8_t			(*t_bounder)(const void*, t_box*);
uint8_t					sphr_bounding(const void *sphr, t_box *bbox);
uint8_t					pln_bounding(const void *pln, t_box *bbox);
uint8_t					trgl_bounding(const void *trgl, t_box *bbox);
uint8_t					sqr_bounding(const void *sqr, t_box *bbox);
uint8_t					cyl_bounding(const void *cyl, t_box *bbox);
uint8_t					cps_bounding(const void *cps, t_box *bbox);

/*
** Constructor for BVH
*/
uint8_t					builder(struct s_bvh *bvh);

/*
**	Traversal of BVH
*/
uint8_t					traverser(const struct s_bvh *bvh, const t_ray *r,
							struct s_hit *hi, uint8_t shad);

/*
** Hits functions
*/
typedef void			(*t_collider)(const void*, const t_ray*, struct s_hit*,
		const uint32_t);
void					sphr_hit(const void *sphr, const t_ray *r,
							struct s_hit *h, const uint32_t res);
void					pln_hit(const void *pln, const t_ray *r,
							struct s_hit *h, const uint32_t res);
void					trgl_hit(const void *trgl, const t_ray *r,
							struct s_hit *h, const uint32_t res);
void					sqr_hit(const void *sqr, const t_ray *r,
							struct s_hit *h, const uint32_t res);
void					cyl_hit(const void *cyl, const t_ray *r,
							struct s_hit *h, const uint32_t res);
void					cps_hit(const void *cps, const t_ray *r,
							struct s_hit *h, const uint32_t res);
#endif

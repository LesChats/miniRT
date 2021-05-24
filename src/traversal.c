/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   traversal.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <abaudot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/14 11:30:54 by abaudot           #+#    #+#             */
/*   Updated: 2021/05/23 19:42:16 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"
#include "bvh.h"

static inline void	swap32(void *a, void *b)
{
	uint32_t	tmp;

	tmp = *(uint32_t *)a;
	*(uint32_t *)a = *(uint32_t *)b;
	*(uint32_t *)b = tmp;
}

static uint8_t	inter(const t_ray *ray, const t_box *bbox,
		float *near, float *far)
{
	float	ttmm[4];

	ttmm[0] = (bbox->min[0] - ray->o[0]) * ray->inv_d[0];
	ttmm[1] = (bbox->max[0] - ray->o[0]) * ray->inv_d[0];
	if (ttmm[0] > ttmm[1])
		swap32(ttmm, ttmm + 1);
	ttmm[2] = (bbox->min[1] - ray->o[1]) * ray->inv_d[1];
	ttmm[3] = (bbox->max[1] - ray->o[1]) * ray->inv_d[1];
	if (ttmm[2] > ttmm[3])
		swap32(ttmm + 2, ttmm + 3);
	if ((ttmm[0] > ttmm[3]) | (ttmm[2] > ttmm[1]))
		return (0);
	ttmm[0] = fmax(ttmm[0], ttmm[2]);
	ttmm[1] = fmin(ttmm[1], ttmm[3]);
	ttmm[2] = (bbox->min[2] - ray->o[2]) * ray->inv_d[2];
	ttmm[3] = (bbox->max[2] - ray->o[2]) * ray->inv_d[2];
	if (ttmm[2] > ttmm[3])
		swap32(ttmm + 2, ttmm + 3);
	if ((ttmm[0] > ttmm[3]) | (ttmm[2] > ttmm[1]))
		return (0);
	*near = fmax(ttmm[0], ttmm[2]);
	*far = fmin(ttmm[1], ttmm[3]);
	return (1);
}

static inline void	function_02(const struct s_node *n,
		const t_ray *r, struct s_travarray *td, const int32_t ni)
{
	float			bh[4];
	const uint8_t	ht0 = inter(r, &n[ni + 1].bbox, bh, bh + 1);
	const uint8_t	ht1 = inter(r, &n[ni + n[ni].offset].bbox, bh + 2, bh + 3);
	int32_t			closer_other[2];

	if (ht0 && ht1)
	{
		closer_other[0] = ni + 1;
		closer_other[1] = ni + n[ni].offset;
		if (bh[2] < bh[0])
		{
			swap32(bh, bh + 2);
			swap32(bh + 1, bh + 3);
			swap32(closer_other, closer_other + 1);
		}
		td->arr[++td->stkptr] = (t_traversal){closer_other[1], bh[2]};
		td->arr[++td->stkptr] = (t_traversal){closer_other[0], bh[0]};
	}
	else if (ht0)
		td->arr[++td->stkptr] = (t_traversal){ni + 1, bh[0]};
	else if (ht1)
		td->arr[++td->stkptr] = (t_traversal){ni + n[ni].offset, bh[2]};
}

static uint8_t	function_01(const t_prmtv *prmtvs, const t_ray *r,
		struct s_hit *h, const t_node *n)
{
	static const t_collider	hits_func[] = {sphr_hit, pln_hit, sqr_hit,
		cyl_hit, trgl_hit, cps_hit};
	const uint32_t			start = n->start;
	const uint32_t			end = n->prmtv_cnt;
	struct s_hit			tmp_h;
	uint32_t				o;

	tmp_h = *h;
	o = 0;
	while (o < end)
	{
		hits_func[prmtvs[start + o].type](prmtvs[start + o].prmtv,
				r, &tmp_h, start + o);
		++o;
	}
	if (h->t > tmp_h.t)
	{
		*h = tmp_h;
		return (1);
	}
	return (0);
}

uint8_t	traverser(const struct s_bvh *bvh, const t_ray *r,
		struct s_hit *h, uint8_t shad)
{
	struct s_travarray	todo;
	int32_t				ni;
	float				near;

	todo.stkptr = 0;
	todo.arr[todo.stkptr].i = 0;
	todo.arr[todo.stkptr].mint = -999999.f;
	while (todo.stkptr >= 0)
	{
		ni = todo.arr[todo.stkptr].i;
		near = todo.arr[todo.stkptr--].mint;
		if (near > h->t)
			continue ;
		if (bvh->nodes[ni].offset == 0)
		{
			if (function_01(bvh->prmtvs.prmtvs, r, h, bvh->nodes + ni) && shad)
				return (1);
		}
		else
			function_02(bvh->nodes, r, &todo, ni);
	}
	return (0);
}

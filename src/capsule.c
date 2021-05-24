/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   capsule.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/18 13:49:03 by abaudot           #+#    #+#             */
/*   Updated: 2021/05/23 20:26:15 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bvh.h"

void	cps_nrml(const void *const cps, t_hinfo *hi)
{
	const t_cps	*cp = (t_cps *)cps;
	t_vec3f		pa;
	t_vec3f		tmp;
	float		h;

	sub_(hi->r->o, cp->pa, pa);
	h = dotp(pa, cp->ba) / cp->baba;
	if (h < 0.0)
		h = 0.001;
	else if (h > 1.0)
		h = 0.99f;
	s_scale(cp->ba, h, tmp);
	sub_(pa, tmp, hi->n);
	s_scale(hi->n, 1.f / cp->r, hi->n);
}

static float	setxyabc(const t_cps *cp, const t_ray *r, float *comp,
		float *abch)
{
	t_vec3f	oa;

	sub_(r->o, cp->pa, oa);
	comp[0] = dotp(cp->ba, r->d);
	comp[1] = dotp(cp->ba, oa);
	comp[2] = dotp(r->d, oa);
	comp[3] = dotp(oa, oa);
	abch[0] = cp->baba - comp[0] * comp[0];
	abch[1] = cp->baba * comp[2] - comp[1] * comp[0];
	abch[2] = cp->baba * comp[3] - comp[1] * comp[1] - cp->r2 * cp->baba;
	abch[3] = abch[1] * abch[1] - abch[0] * abch[2];
	return (abch[3]);
}

static inline void	setreturn(struct s_hit *h, const uint32_t res, float t)
{
	h->t = t;
	h->p = res;
}

void	cps_hit(const void *cps, const t_ray *r, struct s_hit *h,
		const uint32_t res)
{
	const t_cps	*cp = (t_cps*)cps;
	t_vec3f		oc;
	float		comp[4];
	float		abch[4];
	float		ty[2];

	if (setxyabc(cp, r, comp, abch) < 0.0)
		return ;
	ty[0] = (-abch[1] - sqrtf(abch[3])) / abch[0];
	ty[1] = comp[1] + ty[0] * comp[0];
	if (ty[1] > 0.0 && ty[1] < cp->baba && ty[0] < h->t && ty[0] > h->min)
		return (setreturn(h, res, ty[0]));
	if (ty[1] <= 0.f)
		sub_(r->o, cp->pa, oc);
	else
		sub_(r->o, cp->pb, oc);
	abch[1] = dotp(r->d, oc);
	abch[2] = dotp(oc, oc) - cp->r2;
	abch[3] = abch[1] * abch[1] - abch[2];
	if (abch[3] <= 0.0)
		return ;
	ty[0] = -abch[1] - sqrtf(abch[3]);
	if (ty[0] < h->t && ty[0] > h->min)
		return (setreturn(h, res, ty[0]));
}

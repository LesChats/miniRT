/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   whitted_rt.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <abaudot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/20 19:57:11 by abaudot           #+#    #+#             */
/*   Updated: 2021/04/24 14:47:11 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tracer.h"
#include "bvh.h"

static void			bump(t_vec3f normal, const t_vec3f pts, const float scale,
		const float bump)
{
	t_vec3f tmp;
	t_vec3f noi;

	s_scale(pts, scale, tmp);
	noi[0] = noise(tmp);
	noi[1] = noise((t_vec3f){tmp[1], tmp[2], tmp[0]});
	noi[2] = noise((t_vec3f){tmp[2], tmp[0], tmp[1]});
	s_scale(noi, bump, noi);
	add_(normal, noi, normal);
	normalize(normal);
}

static inline void	setinfo(t_hinfo *hi, t_ray *r, const struct s_hit *h,
		const t_prmtv *hp)
{
	static const t_normals grabnormal[] = {sphr_nrml, pln_nrml, sqr_nrml,
		cyl_nrml, trgl_nrml, cps_nrml};

	s_scale(r->d, h->t, hi->n);
	add_(r->o, hi->n, r->o);
	hi->r = r;
	inv_(hi->n, hi->view);
	normalize(hi->view);
	grabnormal[hp->type](hp->prmtv, hi);
	if ((dotp(hi->n, r->d)) > 0.0f)
		inv_(hi->n, hi->n);
	hi->spec = hp->mtrl->spec;
	hi->refl = hp->mtrl->reflr;
	if (hp->mtrl->bump != 0.0)
		bump(hi->n, hi->r->o, hp->mtrl->scale, hp->mtrl->bump);
}

void				getcolor(const struct s_bvhl *s, t_ray *r, t_vec3f color,
		uint32_t d)
{
	static const t_shaders	lightings[] = {diffusel, specularl, rflctl};
	static const t_colors	getcolor[] = {onlycolor, normalraindow,
		checkerboard, marble, turbulence, wood, fromtext};
	struct s_hit			h;
	t_hinfo					hi;
	t_prmtv					*hprmtv;

	h.min = 0.1f;
	h.t = FLT_MAX;
	traverser(&s->bvh, r, &h, 0);
	if (h.t < FLT_MAX)
	{
		hprmtv = s->bvh.prmtvs.prmtvs + h.p;
		hi.deep = d;
		setinfo(&hi, r, &h, hprmtv);
		getcolor[hprmtv->mtrl->colortype](color, &hi, hprmtv->mtrl);
		lightings[hprmtv->mtrl->type](s, &hi, color);
	}
	else
		getbackground(&s->sky, r->d, color);
}

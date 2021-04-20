/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   whittedRT.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 15:27:23 by abaudot           #+#    #+#             */
/*   Updated: 2021/04/18 20:59:15 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tracer.h"
#include "bvh.h"


static void bump(t_vec3f normal, const t_vec3f pts, const float scale)
{
	t_vec3f tmp;
	t_vec3f noi;

	s_scale(pts, scale, tmp);
	noi[0] = noise(tmp);
	noi[1] = noise((t_vec3f){tmp[1], tmp[2], tmp[0]});
	noi[2] = noise((t_vec3f){tmp[2], tmp[0], tmp[1]});
	//s_scale(noi, 1.5, noi); <-- coriger ca
	add_(normal, noi, normal);
	normalize(normal);
}

static inline void setInfo(t_hInfo *hi, t_ray *r, const struct s_hit *h,
		const t_prmtv *hp)
{
	static const t_normals grabNormal[] = {sphr_nrml, pln_nrml, sqr_nrml,
		cyl_nrml, trgl_nrml, cps_nrml};
	
	s_scale(r->d, h->t, hi->n);
	add_(r->o, hi->n, r->o);
	hi->r = r;
	inv_(hi->n, hi->view);
	normalize(hi->view);
	grabNormal[hp->type](hp->prmtv, hi);
	//change this soon
	if ((dotp(hi->n, r->d)) > 0.0f)
	{
		//printf("inside\n");
	  	inv_(hi->n, hi->n);
	}
	hi->spec = hp->mtrl->spec;
	hi->refl = hp->mtrl->reflr;
}

void	getColor(const struct s_bvhL *s, t_ray *r, t_vec3f color, uint32_t d)
{
	static const t_shaders lightings[] = {diffuseL, specularL, rflctL};
	static const t_colors	getColor[] = {onlycolor, normalRaindow,
	   	checkerboard, marble, turbulence, wood, fromText};
	struct s_hit h;
	t_hInfo	hi;
	t_prmtv	*hprmtv;

	h.min = 0.01f;
	h.t = FLT_MAX;
	traverser(&s->bvh, r, &h, 0);
	if (h.t < FLT_MAX)
	{
		//printf("ok\n");
		hprmtv = s->bvh.prmtvs.prmtvs + h.p;
		hi.deep = d;
		setInfo(&hi, r, &h, hprmtv);
		getColor[hprmtv->mtrl->colortype](color, &hi, hprmtv->mtrl);
		if (hprmtv->mtrl->bump != 0.0)
			bump(hi.n, hi.r->o, hprmtv->mtrl->bump);
		lightings[hprmtv->mtrl->type](s, &hi, color);
	}
	else 
		getBackGround(&s->sky, r->d, color);
}

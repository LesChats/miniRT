/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lightings.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 14:05:15 by abaudot           #+#    #+#             */
/*   Updated: 2021/04/19 11:51:05 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bvh.h"
#include "tracer.h"
#include <stdio.h>

static inline uint8_t diffuse(const struct s_bvh *bvh,
		const t_lght *lght, const t_hInfo *hi, t_vec3f diff)
{
	t_vec3f			light;
	struct s_hit	h;
	float			dp;
	float			length;
	
	sub_(lght->pts, hi->r->o, hi->r->d);
	length = norm(hi->r->d);
	h.min = 0.01f;//norm(hi->r->d);
	h.t = length;
	normalize(hi->r->d);
	dp = dotp(hi->n, hi->r->d);
	if (dp > 0.0f)
	{
		invert_(hi->r->d, hi->r->inv_d);
		traverser(bvh, hi->r, &h, 1);
		if (h.t < length /* && bvh->prmtvs.prmtvs[h.p].mtrl->type != 3*/)
			return (0);
		s_scale(lght->color, dp, light);
//		s_scale(light, 1 + 0.25 * 1.f / h.min, light);
		add_(light, diff, diff);
		return (1);
	}
	return (0);
}

static inline void specular(const t_lght *const lght, const t_hInfo *hi,
		t_vec3f spec)
{
	t_vec3f h;
	t_vec3f light;

	add_(hi->r->d, hi->view, h);
	normalize(h);
	s_scale(lght->color, fmax(0, pow(dotp(hi->n, h), hi->spec)), light);
	add_(spec, light, spec);
}


void	diffuseL(const struct s_bvhL *s, const t_hInfo *hi, t_vec3f color)
{
	t_vec3f			ambi;
	t_vec3f			diff;
	uint32_t		i;

	i = 0;
	mult_(s->lghts.amb, color, ambi);
	set_vector(diff, 0.f, 0.f, 0.f);
//	equal_(hi->r->o, hi->pts);
	while (i < s->lghts.count)
	{
		diffuse(&s->bvh, s->lghts.lghts + i, hi, diff);
		++i;
	}
	mult_(color, diff, color);
	add_(ambi, color, color);
//	printf("diff ?\n");
//	verify(color);
}

void	specularL(const struct s_bvhL *s, const t_hInfo *hi, t_vec3f color)
{
	t_vec3f			ambi;
	t_vec3f			diff;
	t_vec3f			spec;
	uint32_t		i;
	
	
	//normalize(hi->view);
	i = 0;
	set_vector(diff, 0.f, 0.f, 0.f);
	set_vector(spec, 0.f, 0.f, 0.f);
	mult_(s->lghts.amb, color, ambi);
	while (i < s->lghts.count)
	{
		if (diffuse(&s->bvh, s->lghts.lghts + i, hi, diff))
			specular(s->lghts.lghts + i, hi, spec);
		++i;
	}
	mult_(color, diff, color);
	add_(color,spec, color);
	add_(ambi, color, color);
//	verify(color);
}

void	rflctL(const struct s_bvhL *s, const t_hInfo *hi, t_vec3f color)
{
	t_vec3f		ambi;
	t_vec3f 	diff;
	t_vec3f		spec;
	t_vec3f		relected;
	uint32_t	i;
	
	if (hi->deep > 5)
		return (specularL(s, hi, color));
	reflectVec(hi->r->d, hi->n, hi->r);
	getColor(s, hi->r, relected, hi->deep + 1);
	s_scale(relected, hi->refl, relected);
	i = 0;
	set_vector(diff, 0.f, 0.f, 0.f);
	set_vector(spec, 0.f, 0.f, 0.f);
	mult_(s->lghts.amb, color, ambi);
	while (i < s->lghts.count)
	{
		if (diffuse(&s->bvh, s->lghts.lghts + i, hi, diff))
			specular(s->lghts.lghts + i, hi, spec);
		++i;
	}
	mult_(color, diff, color);
	add_(relected, color, color);
	add_(color,spec, color);
	add_(ambi, color, color);
}


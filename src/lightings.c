/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lightings.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <abaudot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 12:45:48 by abaudot           #+#    #+#             */
/*   Updated: 2021/04/21 13:31:27 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bvh.h"
#include "tracer.h"

static inline uint8_t	diffuse(const struct s_bvh *bvh,
	const t_lght *lght, const t_hinfo *hi, t_vec3f diff)
{
	t_vec3f			light;
	struct s_hit	h;
	float			dp;
	float			length;

	sub_(lght->pts, hi->r->o, hi->r->d);
	length = norm(hi->r->d);
	h.min = 0.01f;
	h.t = length;
	normalize(hi->r->d);
	dp = dotp(hi->n, hi->r->d);
	if (dp > 0.0f)
	{
		invert_(hi->r->d, hi->r->inv_d);
		traverser(bvh, hi->r, &h, 1);
		if (h.t < length)
			return (0);
		s_scale(lght->color, dp, light);
		add_(light, diff, diff);
		return (1);
	}
	return (0);
}

static inline void		specular(const t_lght *const lght, const t_hinfo *hi,
		t_vec3f spec)
{
	t_vec3f h;
	t_vec3f light;

	add_(hi->r->d, hi->view, h);
	normalize(h);
	s_scale(lght->color, fmax(0, pow(dotp(hi->n, h), hi->spec)), light);
	add_(spec, light, spec);
}

void					diffusel(const struct s_bvhl *s, const t_hinfo *hi,
		t_vec3f color)
{
	t_vec3f			ambi;
	t_vec3f			diff;
	uint32_t		i;

	i = 0;
	mult_(s->lghts.amb, color, ambi);
	set_vector(diff, 0.f, 0.f, 0.f);
	while (i < s->lghts.count)
	{
		diffuse(&s->bvh, s->lghts.lghts + i, hi, diff);
		++i;
	}
	mult_(color, diff, color);
	add_(ambi, color, color);
}

void					specularl(const struct s_bvhl *s, const t_hinfo *hi,
		t_vec3f color)
{
	t_vec3f			ambi;
	t_vec3f			diff;
	t_vec3f			spec;
	uint32_t		i;

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
	add_(color, spec, color);
	add_(ambi, color, color);
}

void					rflctl(const struct s_bvhl *s, const t_hinfo *hi,
		t_vec3f color)
{
	t_vec3f		ambi;
	t_vec3f		dsr[3];
	uint32_t	i;

	if (hi->deep > 5)
		return (specularl(s, hi, dsr[2]));
	reflectvec(hi->r->d, hi->n, hi->r);
	getcolor(s, hi->r, dsr[2], hi->deep + 1);
	s_scale(dsr[2], hi->refl, dsr[2]);
	i = 0;
	set_vector(dsr[0], 0.f, 0.f, 0.f);
	set_vector(dsr[1], 0.f, 0.f, 0.f);
	mult_(s->lghts.amb, color, ambi);
	while (i < s->lghts.count)
	{
		if (diffuse(&s->bvh, s->lghts.lghts + i, hi, dsr[0]))
			specular(s->lghts.lghts + i, hi, dsr[1]);
		++i;
	}
	mult_(color, dsr[0], color);
	add_(dsr[2], color, color);
	add_(color, dsr[1], color);
	add_(ambi, color, color);
}

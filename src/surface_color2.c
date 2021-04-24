/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   surface_color2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <abaudot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 10:05:53 by abaudot           #+#    #+#             */
/*   Updated: 2021/04/24 23:44:48 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tracer.h"

void	wood(t_vec3f color, const t_hinfo *h, const t_material *m)
{
	float		xyz[3];
	float		noisecoef;

	xyz[0] = h->r->o[0] * m->scale;
	xyz[1] = h->r->o[1] * m->scale;
	xyz[2] = h->r->o[2] * m->scale;
	noisecoef = fabsf(noise(xyz)) * 5;
	noisecoef -= (int32_t)noisecoef;
	s_scale(m->color, noisecoef, color);
	s_scale(m->color2, (1.0f - noisecoef), xyz);
	add_(color, xyz, color);
}

void	fromtext(t_vec3f color, const t_hinfo *h, const t_material *m)
{
	uint32_t u;
	uint32_t v;

	if (h->deep > 1)
		return (onlycolor(color, h, m));
	if (!m->text.addr)
		return (equal_(color, (t_vec3f){0, 0, 0}));
	u = (uint32_t)((0.5f + (atan2f(h->n[0], h->n[2]) * INVPI * 0.5f)) *
			(m->text.wdth - 1));
	v = (uint32_t)((0.5f - (asinf(h->n[1] * INVPI))) * (m->text.hght - 1));
	setcolorfromint(((int *)m->text.addr)[v * m->text.wdth + u],
			color);
}

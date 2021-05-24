/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   surface_color.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <abaudot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 11:41:56 by abaudot           #+#    #+#             */
/*   Updated: 2021/05/23 19:47:22 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tracer.h"

void	normalraindow(t_vec3f color, const t_hinfo *h, const t_material *m)
{
	(void)m;
	set_vector(color, h->n[0] + 1, h->n[1] + 1, h->n[2] + 1);
}

void	onlycolor(t_vec3f color, const t_hinfo *h, const t_material *m)
{
	(void)m;
	(void)h;
	equal_(color, m->color);
}

void	checkerboard(t_vec3f color, const t_hinfo *h, const t_material *m)
{
	const uint8_t	x = ((int)((h->r->o[0] + POINT_OFFSET)
				/ m->scale) & 1) == 0;
	const uint8_t	y = ((int)((h->r->o[1] + POINT_OFFSET)
				/ m->scale) & 1) == 0;
	const uint8_t	z = ((int)((h->r->o[2] + POINT_OFFSET)
				/ m->scale) & 1) == 0;

	if (x ^ y ^ z)
		return (equal_(color, m->color));
	equal_(color, m->color2);
}

void	marble(t_vec3f color, const t_hinfo *h, const t_material *m)
{
	float		xyz[3];
	float		noisecoef;
	uint32_t	i;

	xyz[0] = h->r->o[0] * m->scale;
	xyz[1] = h->r->o[1] * m->scale;
	xyz[2] = h->r->o[2] * m->scale;
	noisecoef = 0.f;
	i = 1;
	while (i < 10)
	{
		noisecoef += (1.f / i) * fabsf(noise((t_vec3f){
					i * 0.05 * xyz[0],
					i * 0.15 * xyz[1],
					i * 0.05 * xyz[2]}));
		++i;
	}
	noisecoef = 0.5f * sinf((xyz[0] + xyz[1]) * 0.05f + noisecoef) + 0.5f;
	s_scale(m->color, noisecoef, color);
	s_scale(m->color2, (1.0f - noisecoef), xyz);
	add_(color, xyz, color);
}

void	turbulence(t_vec3f color, const t_hinfo *h, const t_material *m)
{
	float		xyz[3];
	float		noisecoef;
	uint32_t	i;

	xyz[0] = h->r->o[0] * m->scale;
	xyz[1] = h->r->o[1] * m->scale;
	xyz[2] = h->r->o[2] * m->scale;
	noisecoef = 0.f;
	i = 1;
	while (i < 10)
	{
		noisecoef += (1.f / i) * fabsf(noise((t_vec3f){
					i * 0.05 * xyz[0],
					i * 0.05 * xyz[1],
					i * 0.05 * xyz[2]}));
		++i;
	}
	s_scale(m->color, noisecoef, color);
	s_scale(m->color2, (1.0f - noisecoef), xyz);
	add_(color, xyz, color);
}

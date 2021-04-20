/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getSurfaceColor.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/28 14:32:19 by abaudot           #+#    #+#             */
/*   Updated: 2021/04/18 17:43:23 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tracer.h"
# define POINT_OFFSET 3893343

void	normalRaindow(t_vec3f color, const t_hInfo *h, const t_material *m)
{
	(void)m;
	set_vector(color, h->n[0] + 1, h->n[1] + 1, h->n[2] + 1);
}

void	onlycolor(t_vec3f color, const t_hInfo *h, const t_material *m)
{
	(void)m;
	(void)h;
	equal_(color, m->color);
}

void	checkerboard(t_vec3f color, const t_hInfo *h, const t_material *m)
{
	const uint8_t x = ((int)((h->r->o[0] + POINT_OFFSET) / m->scale) & 1) == 0;
	const uint8_t y = ((int)((h->r->o[1] + POINT_OFFSET) / m->scale) & 1) == 0;
	const uint8_t z = ((int)((h->r->o[2] + POINT_OFFSET) / m->scale) & 1) == 0;

	if (x ^ y ^ z)
		return equal_(color, m->color);
	equal_(color, m->color2);
}

void	marble(t_vec3f color, const t_hInfo *h, const t_material *m)
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
					i * 0.05 * xyz[2]
					}));
		++i;
	}
	noisecoef = 0.5f * sinf((xyz[0] + xyz[1]) * 0.05f + noisecoef) + 0.5f;
	s_scale(m->color, noisecoef, color);
	s_scale(m->color2, (1.0f - noisecoef), xyz);
	add_(color, xyz, color);
}

void	turbulence(t_vec3f color, const t_hInfo *h, const t_material *m)
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
					i * 0.05 * xyz[2]
					}));
		++i;
	}
	s_scale(m->color, noisecoef, color);
	s_scale(m->color2, (1.0f - noisecoef), xyz);
	add_(color, xyz, color);
}


void	wood(t_vec3f color, const t_hInfo *h, const t_material *m)
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

void fromText(t_vec3f color, const t_hInfo *h, const t_material *m)
{
//	printf("getcolor\n");
	//for sphere
	uint32_t u;
	uint32_t v;

	if (!m->text.addr)
		return (equal_(color, (t_vec3f){0, 0, 0}));
	u = (uint32_t)((0.5f + (atan2f(h->n[0], h->n[2]) * INVPI * 0.5f)) * (m->text.wdth - 1)) ;
	v = (uint32_t)((0.5f - (asinf(h->n[1] * INVPI))) * (m->text.hght - 1));

	setColorFromInt(((int *)m->text.addr)[v * m->text.wdth + u],
			color);
}




/*
void	checkerboard(t_vec3f color, const t_hInfo *h, const t_material *m)
{
	const int sines = (sin(10* h->r->o[0]) * sin(10 * h->r->o[1]) * sin(10 * h->r->o[2]));
	if (sines < 0)
		return equal_(color, m->color);
	equal_(color, m->color2);
}
*/


//void getSurfaceColor(t_vec3f color, const t_hInfo *h, const t_material *m)
//{

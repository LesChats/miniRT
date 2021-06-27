/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skybox.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <abaudot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/20 20:02:49 by abaudot           #+#    #+#             */
/*   Updated: 2021/06/27 15:50:06 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "tracer.h"

void	setcolorfromint(int32_t c, t_vec3f color)
{
	color[0] = (float)((c & 0x00FF0000) >> 16);
	color[1] = (float)((c & 0x0000FF00) >> 8);
	color[2] = (float)(c & 0x000000FF);
	s_scale(color, COLOR_INV, color);
}

static uint32_t	getmaxindice(const t_vec3f r, float *u, float *v)
{
	const float	absx = fabs(r[0]);
	const float	absy = fabs(r[1]);
	const float	absz = fabs(r[2]);

	if (absx >= absy && absx >= absz)
	{
		*u = 1.f - (r[2] / r[0] + 1.f) * 0.5f;
		*v = (1.f - (r[1] / r[0] + 1.f) * 0.5f) * (-1 + 2 * (r[0] > 0))
			+ (r[0] < 0);
		return (0 + (r[0] < 0.f));
	}
	if (absy >= absx && absy >= absz)
	{
		*u = ((r[0] / r[1] + 1.f) * 0.5f) * (-1 + 2 * (r[1] > 0)) + (r[1] < 0);
		*v = ((r[2] / r[1] + 1.f) * 0.5f);
		return (2 + (r[1] < 0.f));
	}
	*u = (r[0] / r[2] + 1.f) * 0.5f;
	*v = (1.f - (r[1] / r[2] + 1.f) * 0.5f) * (-1 + 2 * (r[2] > 0))
		+ (r[2] < 0);
	return (4 + (r[2] < 0.f));
}

static void	getskyboxtexture(const t_vec3f ray, t_vec3f color,
		const struct s_skybox *sky)
{
	float		u;
	float		v;
	uint32_t	index;

	index = getmaxindice(ray, &u, &v);
	u *= (sky->widthu - 1);
	v *= (sky->heightv - 1);
	setcolorfromint(((int *)sky->skybox[index])[(int)v * sky->widthu
		+ (int)u], color);
}

void	getbackground(const struct s_skybox *sky, const t_vec3f ray,
		t_vec3f color)
{
	if (*sky->skybox == NULL)
		return (set_vector(color, 0.f, 0.f, 0.f));
	return (getskyboxtexture(ray, color, sky));
}

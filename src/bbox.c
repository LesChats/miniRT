/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bbox.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aime <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/10 13:47:36 by aime              #+#    #+#             */
/*   Updated: 2021/04/21 12:05:14 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bvh.h"

void		getcenter(const t_box *bbox, t_vec3f v)
{
	add_(bbox->max, bbox->min, v);
	s_scale(v, 0.5f, v);
}

uint32_t	maxdimension(const t_box *bbox)
{
	uint32_t	result;

	result = 0;
	if (bbox->extent[1] > bbox->extent[result])
		result = 1;
	if (bbox->extent[2] > bbox->extent[result])
		result = 2;
	return (result);
}

void		expandtoinclude_vec(const t_vec3f v, t_box *bbox)
{
	set_vector(bbox->min, fmin(v[0], bbox->min[0]), fmin(v[1], bbox->min[1]),
			fmin(v[2], bbox->min[2]));
	set_vector(bbox->max, fmax(v[0], bbox->max[0]), fmax(v[1], bbox->max[1]),
			fmax(v[2], bbox->max[2]));
	sub_(bbox->max, bbox->min, bbox->extent);
}

void		expandtoinclude(const t_box *v, t_box *bbox)
{
	set_vector(bbox->min, fmin(v->min[0], bbox->min[0]), fmin(v->min[1],
				bbox->min[1]),
			fmin(v->min[2], bbox->min[2]));
	set_vector(bbox->max, fmax(v->max[0], bbox->max[0]), fmax(v->max[1],
				bbox->max[1]),
			fmax(v->max[2], bbox->max[2]));
	sub_(bbox->max, bbox->min, bbox->extent);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reflexiveAndRefractive.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 12:16:24 by abaudot           #+#    #+#             */
/*   Updated: 2021/03/28 15:56:13 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tracer.h"

void	reflectVec(const t_vec3f v, const t_vec3f n, t_ray *r)
{
	t_vec3f tmp;

	s_scale(n, 2.0f * dotp(v, n), tmp);
	sub_(v, tmp, r->d);
//	normalize(r->d);
//	set_vector(r->inv_d, 1.f / r->d[0], 1.f / r->d[1], 1.f / r->d[2]);
	invert_(r->d, r->inv_d);
}

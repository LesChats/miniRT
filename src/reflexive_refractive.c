/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reflexive_refractive.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 12:16:24 by abaudot           #+#    #+#             */
/*   Updated: 2021/04/21 12:05:37 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tracer.h"

void	reflectvec(const t_vec3f v, const t_vec3f n, t_ray *r)
{
	t_vec3f tmp;

	s_scale(n, 2.0f * dotp(v, n), tmp);
	sub_(v, tmp, r->d);
	invert_(r->d, r->inv_d);
}

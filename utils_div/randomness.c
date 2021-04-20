/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   randomness.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 13:35:54 by abaudot           #+#    #+#             */
/*   Updated: 2021/03/18 13:43:57 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tracer.h"

float	frand(void)
{
	static uint32_t g_seed = 0x17f3ad37;

    g_seed = (214013 * g_seed + 2531011);
    return ((g_seed >> 16) & 0x7FFF) * DIV;
}

void	random_in_unit_sphere(t_vec3f tmp)
{
		set_vector(tmp, frand(), frand(), frand());
		s_scale(tmp, 2.0f, tmp);
		sub_(tmp, (t_vec3f){1.f, 1.f, 1.f}, tmp);
		normalize(tmp);
}
/*
void random_in_hemisphere(t_vec3f tmp, const t_vec3f n)
{
	random_in_unit_sphere(tmp);
	if (dotp(tmp, n) > 0.0)
		return ;
	inv_(tmp, tmp);
}
*/

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vectors.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/13 18:00:51 by abaudot           #+#    #+#             */
/*   Updated: 2021/04/21 12:50:27 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"

void			set_vector(t_vec3f v, float a, float b, float c)
{
	v[0] = a;
	v[1] = b;
	v[2] = c;
}

void			equal_(t_vec3f dst, const t_vec3f src)
{
	dst[0] = src[0];
	dst[1] = src[1];
	dst[2] = src[2];
}

float			dotp(const t_vec3f v, const t_vec3f w)
{
	return (v[0] * w[0] + v[1] * w[1] + v[2] * w[2]);
}

inline float	norm2(const t_vec3f v)
{
	return (dotp(v, v));
}

inline float	norm(const t_vec3f v)
{
	return (sqrtf(norm2(v)));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vectors.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/13 18:00:51 by abaudot           #+#    #+#             */
/*   Updated: 2021/04/14 13:18:41 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "struct.h"
#include "vectors.h"

void	set_vector(t_vec3f v, float a, float b, float c)
{
	v[0] = a;
	v[1] = b;
	v[2] = c;
}

void	equal_(t_vec3f dst, const t_vec3f src)
{
	dst[0] = src[0];
	dst[1] = src[1];
	dst[2] = src[2];
}

float dotp(const t_vec3f v, const t_vec3f w)
{
	return (v[0] * w[0] + v[1] * w[1] + v[2] * w[2]);
}

inline float norm2(const t_vec3f v)
{
	return (dotp(v, v));
}

inline float norm(const t_vec3f v)
{
	return sqrtf(norm2(v));
}

inline void	crossp(const t_vec3f v, const t_vec3f w, t_vec3f dst)
{
	dst[0] = v[1] * w[2] - v[2] * w[1];
	dst[1] = v[2] * w[0] - v[0] * w[2]; //-(v[0] * w[2] - v[2] * w[0]);
	dst[2] = v[0] * w[1] - v[1] * w[0];
}

void	set_vzero(t_vec3f v)
{
	v[0] = 0;
	v[1] = 0;
	v[2] = 0;
}

void	s_scale(const t_vec3f v, const float s, t_vec3f dest)
{
	dest[0] = v[0] * s;
	dest[1] = v[1] * s;
	dest[2] = v[2] * s;
}

void	normalize(t_vec3f v)
{
	float n = norm(v);
	
	if (!n)
		return (set_vzero(v));
	s_scale(v, 1.0f / n, v);
}

void	add_(const t_vec3f v, const t_vec3f w, t_vec3f dst)
{
	dst[0] = v[0] + w[0];
	dst[1] = v[1] + w[1];
	dst[2] = v[2] + w[2];
}

void	sub_(const t_vec3f v, const t_vec3f w, t_vec3f dst)
{
	dst[0] = v[0] - w[0];
	dst[1] = v[1] - w[1];
	dst[2] = v[2] - w[2];
}

void mult_(const t_vec3f v, const t_vec3f w, t_vec3f dst)
{
	dst[0] = v[0] * w[0];
	dst[1] = v[1] * w[1];
	dst[2] = v[2] * w[2];
}

void	div_(const t_vec3f v, const t_vec3f w, t_vec3f dst)
{
	dst[0] = v[0] / w[0];
	dst[1] = v[1] / w[1];
	dst[2] = v[2] / w[2];
}

void invert_(const t_vec3f v, t_vec3f dst)
{
	dst[0] = 1.f / v[0];
	dst[1] = 1.f / v[1];
	dst[2] = 1.f / v[2];
}

void	inv_(const t_vec3f v, t_vec3f dst)
{
	dst[0] = -v[0];
	dst[1] = -v[1];
	dst[2] = -v[2];
}

void	unit_x(t_vec3f v)
{
	v[0] = 1;
	v[1] = 0;
	v[2] = 0;
}

void	unit_y(t_vec3f v)
{
	v[0] = 0;
	v[1] = 1;
	v[2] = 0;
}

void	unit_z(t_vec3f v)
{
	v[0] = 0;
	v[1] = 0;
	v[2] = 1;
}

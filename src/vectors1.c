/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vectors1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <abaudot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 12:14:53 by abaudot           #+#    #+#             */
/*   Updated: 2021/05/24 18:20:05 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"

inline void	crossp(const t_vec3f v, const t_vec3f w, t_vec3f dst)
{
	dst[0] = v[1] * w[2] - v[2] * w[1];
	dst[1] = v[2] * w[0] - v[0] * w[2];
	dst[2] = v[0] * w[1] - v[1] * w[0];
}

inline void	s_scale(const t_vec3f v, const float s, t_vec3f dest)
{
	dest[0] = v[0] * s;
	dest[1] = v[1] * s;
	dest[2] = v[2] * s;
}

void	normalize(t_vec3f v)
{
	const float	n = norm(v);

	if (!n)
		return (set_vector(v, 0.f, 0.f, 0.f));
	s_scale(v, 1.0f / n, v);
}

inline void	add_(const t_vec3f v, const t_vec3f w, t_vec3f dst)
{
	dst[0] = v[0] + w[0];
	dst[1] = v[1] + w[1];
	dst[2] = v[2] + w[2];
}

inline void	sub_(const t_vec3f v, const t_vec3f w, t_vec3f dst)
{
	dst[0] = v[0] - w[0];
	dst[1] = v[1] - w[1];
	dst[2] = v[2] - w[2];
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vectors2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <abaudot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 12:17:34 by abaudot           #+#    #+#             */
/*   Updated: 2021/05/23 20:17:41 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vectors.h"

inline void	mult_(const t_vec3f v, const t_vec3f w, t_vec3f dst)
{
	dst[0] = v[0] * w[0];
	dst[1] = v[1] * w[1];
	dst[2] = v[2] * w[2];
}

inline void	div_(const t_vec3f v, const t_vec3f w, t_vec3f dst)
{
	dst[0] = v[0] / w[0];
	dst[1] = v[1] / w[1];
	dst[2] = v[2] / w[2];
}

inline void	invert_(const t_vec3f v, t_vec3f dst)
{
	dst[0] = 1.f / v[0];
	dst[1] = 1.f / v[1];
	dst[2] = 1.f / v[2];
}

inline void	inv_(const t_vec3f v, t_vec3f dst)
{
	dst[0] = -v[0];
	dst[1] = -v[1];
	dst[2] = -v[2];
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vectors.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <abaudot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/25 22:35:19 by abaudot           #+#    #+#             */
/*   Updated: 2021/04/20 15:24:02 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTORS_H
# define VECTORS_H

# include "struct.h"
# include <math.h>

void	set_vector(t_vec3f v, float a, float b, float c);
void	equal_(t_vec3f dst, const t_vec3f src);
float	dotp(const t_vec3f v, const t_vec3f w);
void	crossp(const t_vec3f v, const t_vec3f w, t_vec3f dst);
float	norm(const t_vec3f v);
void	set_vzero(t_vec3f v);
void	s_scale(const t_vec3f v, const float s, t_vec3f dest);
void	normalize(t_vec3f v);
void	add_(const t_vec3f v, const t_vec3f w, t_vec3f dst);
void	sub_(const t_vec3f v, const t_vec3f w, t_vec3f dst);
void	mult_(const t_vec3f v, const t_vec3f w, t_vec3f dst);
void	div_(const t_vec3f v, const t_vec3f w, t_vec3f dst);
void	inv_(const t_vec3f v, t_vec3f dst);
void	invert_(const t_vec3f v, t_vec3f dst);
#endif

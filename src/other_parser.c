/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   other_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <abaudot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 11:10:39 by abaudot           #+#    #+#             */
/*   Updated: 2021/04/24 23:45:24 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "vectors.h"

uint8_t	rsltn_parser(struct s_prescene *ps, const char *s,
		uint32_t *pos_num)
{
	*pos_num = *pos_num;
	ps->rsltn[0] = ft_atoi(&s);
	ps->rsltn[1] = ft_atoi(&s);
	if (ps->rsltn[0] < 1 || ps->rsltn[1] < 1)
		return (return_message("Please enter a correct resolution"));
	return (1);
}

uint8_t	lght_parser(struct s_prescene *ps, const char *s,
		uint32_t *pos_num)
{
	static uint32_t lght_pos = 0;
	t_lght *const	lght = ps->lghts.lghts + lght_pos++;
	float			intensity;
	t_vec3f			tmp;

	*pos_num = *pos_num;
	if (!(vect_parse(lght->pts, &s)))
		return (return_message("Bad vector parse for lights position"));
	intensity = ft_atof(&s);
	if (!(vect_parse(tmp, &s)))
		return (return_message("Bad vector parse for lights color"));
	s_scale(tmp, COLOR_INV, tmp);
	s_scale(tmp, intensity, lght->color);
	return (1);
}

uint8_t	ambnt_parser(struct s_prescene *ps, const char *s,
		uint32_t *pos_num)
{
	t_vec3f		tmp;
	const float	intensity = ft_atof(&s);

	*pos_num = *pos_num;
	if (!(vect_parse(tmp, &s)))
		return (return_message("Bad vector parse for ambiante color"));
	s_scale(tmp, COLOR_INV, tmp);
	s_scale(tmp, intensity, ps->lghts.amb);
	return (1);
}

uint8_t	parse_mat(t_material *m, const char **s, void *mlx)
{
	t_vec3f tmp;

	if (!(vect_parse(tmp, s)))
		return (return_message("bad color format"));
	s_scale(tmp, COLOR_INV, m->color);
	m->type = ft_atoi(s);
	m->colortype = ft_atoi(s);
	m->spec = ft_atoi(s);
	m->reflr = ft_atof(s);
	if (m->colortype != 6)
		m->scale = ft_atof(s);
	if (m->colortype > 1 && m->colortype != 6)
	{
		if (!(vect_parse(tmp, s)))
			return (return_message("bad color format (color 2)"));
		s_scale(tmp, COLOR_INV, m->color2);
	}
	if (m->colortype > 6 || m->type > 2 || m->colortype < 0 || m->type < 0)
		return (return_message("type error in materials parsing"));
	if (m->colortype == 6)
		grabtexture(mlx, &m->text, s);
	m->bump = ft_atof(s);
	return (1);
}

uint8_t	cams_parser(struct s_prescene *ps, const char *s,
		uint32_t *pos_num)
{
	static uint32_t	cam_nb = 0;
	t_cam *const	cam = (ps->cams.cams + cam_nb++);

	*pos_num = *pos_num;
	if (!(vect_parse(cam->pts, &s)))
		return (return_message("Bad vector parse for camera position"));
	if (!(vect_parse(cam->orient, &s)))
		return (return_message("Bad vector parse for camera orientation"));
	cam->fov = ft_atof(&s);
	cam->filter = ft_atoi(&s);
	return (1);
}

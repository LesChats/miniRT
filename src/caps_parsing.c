/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   caps_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/18 18:56:36 by abaudot           #+#    #+#             */
/*   Updated: 2021/04/21 13:40:11 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "bvh.h"

uint8_t	cps_bounding(const void *cps, t_box *bbox)
{
	const t_cps *const	cp = (t_cps *)cps;
	t_vec3f				e;
	t_vec3f				t1;
	t_vec3f				t2;
	t_vec3f				ray;

	set_vector(ray, cp->r, cp->r, cp->r);
	mult_(cp->ba, cp->ba, e);
	s_scale(e, 1.f / cp->baba, e);
	sub_((t_vec3f){1, 1, 1}, e, e);
	set_vector(e, sqrtf(e[0]), sqrtf(e[1]), sqrtf(e[2]));
	s_scale(e, cp->r, e);
	add_(e, ray, e);
	add_(cp->pa, e, t1);
	add_(cp->pb, e, t2);
	set_vector(bbox->max, fmaxf(t1[0], t2[0]), fmaxf(t1[1], t2[1]),
			fmaxf(t1[2], t2[2]));
	sub_(cp->pa, e, t1);
	sub_(cp->pb, e, t2);
	set_vector(bbox->min, fminf(t1[0], t2[0]), fminf(t1[1], t2[1]),
			fminf(t1[2], t2[2]));
	sub_(bbox->max, bbox->min, bbox->extent);
	return (1);
}

uint8_t	cps_parser(struct s_prescene *ps, const char *s, uint32_t *pos_num)
{
	t_cps *cp;

	cp = (t_cps*)(ps->prmtvs_data + *pos_num);
	ps->prmtvs.prmtvs[pos_num[1]].prmtv = cp;
	ps->prmtvs.prmtvs[pos_num[1]].mtrl = ps->mtrls_data + pos_num[1];
	ps->prmtvs.prmtvs[pos_num[1]].type = CP;
	if (!(vect_parse(cp->pa, &s)))
		return (return_message("bad vector for capsule point 1"));
	if (!(vect_parse(cp->pb, &s)))
		return (return_message("bad vector for capsule point 2"));
	cp->r = ft_atof(&s);
	cp->r2 = cp->r * cp->r;
	sub_(cp->pb, cp->pa, cp->ba);
	if (cp->ba[0] == 0 && cp->ba[1] == 0 && cp->ba[2] == 0)
		return (return_message("Extremum of capsul must be different"));
	cp->baba = dotp(cp->ba, cp->ba);
	if (!parse_mat(ps->mtrls_data + pos_num[1], &s, ps->mlx))
		return (return_message("Bad material for capsule"));
	++pos_num[1];
	*pos_num += sizeof(t_cps);
	return (1);
}

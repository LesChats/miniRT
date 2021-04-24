/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <abaudot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 10:02:55 by abaudot           #+#    #+#             */
/*   Updated: 2021/04/21 13:40:41 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "vectors.h"

static void	parse_sqr(t_sqr *s, const float hgth, const uint32_t i)
{
	static const t_vec3f	nor[3] = {{1.f, 0.f, 0.f},
		{0.f, 1.f, 0.f}, {0.f, 0.f, 1.f}};
	const float				size = hgth * (1.f - 2.f * (i % 2));
	t_vec3f					pos;
	t_vec3f					larg;

	equal_(s->n, nor[i % 3]);
	s_scale(s->n, size * INVSQ2, pos);
	add_(s->pts, pos, s->pts);
	s_scale(nor[i % 3], 0.0001, larg);
	add_(nor[(i + 1) % 3], nor[(i + 2) % 3], pos);
	s_scale(pos, hgth * INVSQ2, pos);
	add_(pos, larg, pos);
	add_(s->pts, pos, s->max);
	sub_(s->pts, pos, s->min);
}

static void	set_sqr(struct s_prescene *ps, uint32_t *pos_num, t_material *m,
		t_sqr *sqr)
{
	ps->prmtvs.prmtvs[pos_num[1]].prmtv = sqr;
	ps->prmtvs.prmtvs[pos_num[1]].mtrl = ps->mtrls_data + pos_num[1];
	ps->prmtvs.prmtvs[pos_num[1]].type = SQ;
	*(ps->mtrls_data + pos_num[1]) = *m;
	++pos_num[1];
	*pos_num += sizeof(t_sqr);
}

uint8_t		cub_parser(struct s_prescene *ps, const char *s, uint32_t *pos_num)
{
	t_sqr		*sqr;
	t_material	*mat;
	float		hgth;
	t_vec3f		center;
	uint32_t	i;

	if (!(vect_parse(center, &s)))
		return (return_message("bad vector for cube center"));
	hgth = ft_atof(&s);
	mat = ps->mtrls_data + pos_num[1];
	if (!parse_mat(mat, &s, ps->mlx))
		return (return_message("bad material for cube"));
	i = 0;
	while (i < 6)
	{
		sqr = (t_sqr*)(ps->prmtvs_data + *pos_num);
		set_sqr(ps, pos_num, mat, sqr);
		equal_(sqr->pts, center);
		parse_sqr(sqr, hgth, i);
		++i;
	}
	return (1);
}

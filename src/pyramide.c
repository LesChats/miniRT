/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pyramide.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <abaudot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/14 10:56:38 by abaudot           #+#    #+#             */
/*   Updated: 2021/04/17 17:51:15 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "vectors.h"

static void set_box(t_sqr *sqr, t_vec3f *co, float circum)
{
	t_vec3f u;
	t_vec3f v;
	t_vec3f coo;
	t_vec3f large;
	t_vec3f diag1;
	t_vec3f	diag2;

	if (!sqr->n[0])
		crossp(sqr->n, (t_vec3f){1, 0, 0}, u);
	else
		crossp(sqr->n, (t_vec3f){0, 1, 0}, u);
	normalize(u);
	crossp(sqr->n, u , v);
	normalize(v);
	add_(u, v, diag1);
	sub_(u, v, diag2);
	s_scale(diag1, circum, diag1);
	s_scale(diag2, circum, diag2);
	add_(sqr->pts, diag1, co[0]);
	add_(sqr->pts, diag2, co[1]);
	sub_(sqr->pts, diag1, co[2]);
	sub_(sqr->pts, diag2, co[3]);
	set_vector(large, fabs(sqr->n[0]) * 0.001, fabs(sqr->n[1]) * 0.001,
			fabs(sqr->n[2]) * 0.001);
	set_vector(coo, fabs(co[0][0]), fabs(co[0][1]),fabs(co[0][2]));
	add_(coo, large, coo);
	add_(sqr->pts, coo, sqr->max);
	sub_(sqr->pts, coo, sqr->min);
}

static uint8_t parse_sqr(struct s_preScene *ps, const char *s, t_vec3f *co,
		uint32_t *pos_num)
{
	t_sqr *sqr;

	sqr = (t_sqr*)(ps->prmtvs_data + *pos_num);
	ps->prmtvs.prmtvs[pos_num[1]].prmtv = sqr;
	ps->prmtvs.prmtvs[pos_num[1]].mtrl = ps->mtrls_data + pos_num[1];
	ps->prmtvs.prmtvs[pos_num[1]].type = SQ;
	if (!(vect_parse(sqr->pts, &s)))
		return (return_message("fail to pase square center of pyramide"));
	if (!(vect_parse(sqr->n, &s)))
		return (return_message("fail to parse pyramide direction"));
	normalize(sqr->n);
	set_box(sqr,co,  ft_atof(&s) / sqrtf(2));
	s_scale(sqr->n, ft_atof(&s), co[4]);
	add_(sqr->pts, co[4], co[4]);
	if (!parse_mat(ps->mtrls_data + pos_num[1],  &s, ps->mlx))
		return (return_message("Bad material for pyramide"));
	++pos_num[1];
	*pos_num += sizeof(t_sqr);
	return (1);
}

static void parse_trg(t_trgl *trgl, t_vec3f p1, t_vec3f p2, t_vec3f p3)
{
	equal_(trgl->p1, p1);
	equal_(trgl->p2, p2);
	equal_(trgl->p3, p3);
	sub_(trgl->p2, trgl->p1, trgl->e1);
	sub_(trgl->p3, trgl->p1, trgl->e2);
	crossp(trgl->e1, trgl->e2, trgl->n);
	normalize(trgl->n);
}

uint8_t	pyr_parser(struct s_preScene *ps, const char *s, uint32_t *pos_num)
{
	t_trgl *trgl;
	t_vec3f co[5];
	uint32_t i;
	t_material *m;

	m = ps->mtrls_data + (pos_num[1]);
	if (!(parse_sqr(ps, s, co, pos_num)))
		return (return_message("fail to parse pyramide"));
	i = 0;
	while (i < 4)
	{
		trgl = (t_trgl*)(ps->prmtvs_data + *pos_num);
		ps->prmtvs.prmtvs[pos_num[1]].prmtv = trgl;
		ps->prmtvs.prmtvs[pos_num[1]].mtrl = ps->mtrls_data + pos_num[1];
		ps->prmtvs.prmtvs[pos_num[1]].type = T;
		parse_trg(trgl, co[i], co[(i + 1) % 4], co[4]);
		*(ps->mtrls_data + pos_num[1]) = *m;
		++pos_num[1];
		*pos_num += sizeof(t_trgl);
		++i;
	}
	return (1);
}

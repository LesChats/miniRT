/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   square.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <abaudot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 10:45:03 by abaudot           #+#    #+#             */
/*   Updated: 2021/04/21 12:03:24 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "bvh.h"

void		sqr_nrml(const void *const sqr, t_hinfo *hi)
{
	const t_sqr *s = (t_sqr*)sqr;

	equal_(hi->n, s->n);
}

void		sqr_hit(const void *sqr, const t_ray *r, struct s_hit *h,
		const uint32_t res)
{
	const t_sqr		*sq = (t_sqr*)sqr;
	const float		deno = dotp(r->d, sq->n);
	t_vec3f			tmp;
	t_vec3f			hit;

	if (deno == 0)
		return ;
	sub_(sq->pts, r->o, tmp);
	tmp[0] = (dotp(tmp, sq->n) / deno);
	if (tmp[0] < h->t && tmp[0] > h->min)
	{
		s_scale(r->d, tmp[0], hit);
		add_(r->o, hit, hit);
		if (hit[0] > sq->max[0] || hit[1] > sq->max[1] || hit[2] > sq->max[2])
			return ;
		if (hit[0] < sq->min[0] || hit[1] < sq->min[1] || hit[2] < sq->min[2])
			return ;
		h->t = tmp[0];
		h->p = res;
	}
}

uint8_t		sqr_bounding(const void *sqr, t_box *bbox)
{
	const t_sqr *const s = (t_sqr *)sqr;

	equal_(bbox->max, s->max);
	equal_(bbox->min, s->min);
	sub_(bbox->max, bbox->min, bbox->extent);
	return (1);
}

static void	getsqr_box(t_sqr *sqr, float cir)
{
	t_vec3f uv[2];
	t_vec3f large;

	if (!sqr->n[0])
		crossp(sqr->n, (t_vec3f){1, 0, 0}, uv[0]);
	else
		crossp(sqr->n, (t_vec3f){0, 1, 0}, uv[0]);
	normalize(uv[0]);
	crossp(sqr->n, uv[0], uv[1]);
	normalize(uv[1]);
	set_vector(uv[0], fabs(uv[0][0]), fabs(uv[0][1]), fabs(uv[0][2]));
	set_vector(uv[1], fabs(uv[1][0]), fabs(uv[1][1]), fabs(uv[1][2]));
	set_vector(large, fabs(sqr->n[0]) * 0.001, fabs(sqr->n[1]) * 0.001,
			fabs(sqr->n[2]) * 0.001);
	add_(uv[1], uv[0], uv[0]);
	s_scale(uv[0], cir, uv[0]);
	add_(uv[0], large, uv[0]);
	add_(sqr->pts, uv[0], sqr->max);
	sub_(sqr->pts, uv[0], sqr->min);
}

uint8_t		sqr_parser(struct s_prescene *ps, const char *s, uint32_t *pos_num)
{
	t_sqr *sqr;

	sqr = (t_sqr*)(ps->prmtvs_data + *pos_num);
	ps->prmtvs.prmtvs[pos_num[1]].prmtv = sqr;
	ps->prmtvs.prmtvs[pos_num[1]].mtrl = ps->mtrls_data + pos_num[1];
	ps->prmtvs.prmtvs[pos_num[1]].type = SQ;
	if (!(vect_parse(sqr->pts, &s)))
		return (return_message("bad vector for square center"));
	if (!(vect_parse(sqr->n, &s)))
		return (return_message("bad vector for square normal"));
	normalize(sqr->n);
	getsqr_box(sqr, ft_atof(&s) / sqrtf(2));
	if (!parse_mat(ps->mtrls_data + pos_num[1], &s, ps->mlx))
		return (return_message("bad material for square"));
	++pos_num[1];
	*pos_num += sizeof(t_sqr);
	return (1);
}

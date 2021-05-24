/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/04 19:00:21 by abaudot           #+#    #+#             */
/*   Updated: 2021/05/23 19:40:35 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "bvh.h"

void	pln_nrml(const void *const pln, t_hinfo *hi)
{
	const t_pln *const	pl = (t_pln*)pln;

	equal_(hi->n, pl->n);
}

void	pln_hit(const void *pln, const t_ray *r, struct s_hit *h,
		const uint32_t res)
{
	const t_pln	*pl = (t_pln*)pln;
	const float	deno = dotp(r->d, pl->n);
	t_vec3f		tmp;

	if (deno < 0.01 && deno > -0.01)
		return ;
	sub_(pl->pts, r->o, tmp);
	tmp[0] = (dotp(tmp, pl->n) / deno);
	if (tmp[0] < h->t && tmp[0] > h->min)
	{
		h->t = tmp[0];
		h->p = res;
		return ;
	}
}

uint8_t	pln_bounding(const void *pln, t_box *bbox)
{
	const t_pln *const	p = (t_pln *)pln;
	t_vec3f				u;
	t_vec3f				v;
	t_vec3f				tmp;
	t_vec3f				larg;

	if (!p->n[0])
		crossp(p->n, (t_vec3f){1, 0, 0}, u);
	else
		crossp(p->n, (t_vec3f){0, 1, 0}, u);
	normalize(u);
	crossp(p->n, u, v);
	normalize(v);
	add_(u, v, tmp);
	set_vector(tmp, fabs(tmp[0]), fabs(tmp[1]), fabs(tmp[2]));
	s_scale(p->n, 0.01, larg);
	add_(tmp, larg, tmp);
	s_scale(tmp, 5000.f, bbox->max);
	s_scale(tmp, -5000.f, bbox->min);
	sub_(bbox->max, bbox->min, bbox->extent);
	return (1);
}

uint8_t	pln_parser(struct s_prescene *ps, const char *s, uint32_t *pos_num)
{
	t_pln	*pln;

	pln = (t_pln *)(ps->prmtvs_data + *pos_num);
	ps->prmtvs.prmtvs[pos_num[1]].prmtv = pln;
	ps->prmtvs.prmtvs[pos_num[1]].mtrl = ps->mtrls_data + pos_num[1];
	ps->prmtvs.prmtvs[pos_num[1]].type = P;
	if (!(vect_parse(pln->pts, &s)))
		return (return_message("Bad vector parsing -plane position"));
	if (!(vect_parse(pln->n, &s)))
		return (return_message("Bad vector parsing -plane normal"));
	normalize(pln->n);
	if (!parse_mat(ps->mtrls_data + pos_num[1], &s, ps->mlx))
		return (return_message("Bad material for plane"));
	++pos_num[1];
	*pos_num += sizeof(t_pln);
	return (1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphr.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 18:14:08 by abaudot           #+#    #+#             */
/*   Updated: 2021/04/21 12:50:56 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "bvh.h"

void	sphr_nrml(const void *const sphr, t_hinfo *hi)
{
	const t_sphr	*s = (t_sphr*)sphr;

	sub_(hi->r->o, s->center, hi->n);
	s_scale(hi->n, 1.f / s->radius, hi->n);
}

void	sphr_hit(const void *sphr, const t_ray *r, struct s_hit *h,
		const uint32_t res)
{
	const t_sphr	*sp = (t_sphr*)sphr;
	t_vec3f			oc;
	float			bc[2];

	sub_(r->o, sp->center, oc);
	bc[0] = dotp(oc, r->d);
	bc[1] = dotp(oc, oc) - (sp->radius2);
	bc[1] = bc[0] * bc[0] - bc[1];
	if (bc[1] < 0)
		return ;
	oc[0] = sqrtf(bc[1]);
	oc[1] = (-bc[0] - oc[0]);
	if (oc[1] < h->t && oc[1] > h->min)
	{
		h->t = oc[1];
		h->p = res;
		return ;
	}
	oc[1] = (-bc[0] + oc[0]);
	if (oc[1] < h->t && oc[1] > h->min)
	{
		h->t = oc[1];
		h->p = res;
	}
}

uint8_t	sphr_bounding(const void *sphr, t_box *bbox)
{
	const t_sphr *const s = (t_sphr *)sphr;

	set_vector(bbox->min, -s->radius, -s->radius, -s->radius);
	set_vector(bbox->max, s->radius, s->radius, s->radius);
	add_(bbox->min, s->center, bbox->min);
	add_(bbox->max, s->center, bbox->max);
	sub_(bbox->max, bbox->min, bbox->extent);
	return (1);
}

uint8_t	sphrs_parser(struct s_prescene *ps, const char *s, uint32_t *pos_num)
{
	t_sphr *sphr;

	sphr = (t_sphr*)(ps->prmtvs_data + *pos_num);
	ps->prmtvs.prmtvs[pos_num[1]].prmtv = sphr;
	ps->prmtvs.prmtvs[pos_num[1]].mtrl = ps->mtrls_data + pos_num[1];
	ps->prmtvs.prmtvs[pos_num[1]].type = SP;
	if (!(vect_parse(sphr->center, &s)))
		return (return_message("Bad vector fot sphere center"));
	sphr->radius = ft_atof(&s);
	sphr->radius2 = sphr->radius * sphr->radius;
	if (!parse_mat(ps->mtrls_data + pos_num[1], &s, ps->mlx))
		return (return_message("materials parsing just fail || sphere"));
	++pos_num[1];
	*pos_num += sizeof(t_sphr);
	return (1);
}

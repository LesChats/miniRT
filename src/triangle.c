/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangle.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <abaudot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/13 14:13:40 by abaudot           #+#    #+#             */
/*   Updated: 2021/04/21 12:26:12 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "bvh.h"

void	trgl_nrml(const void *const trgl, t_hinfo *hi)
{
	const t_trgl *t = (t_trgl*)trgl;

	equal_(hi->n, t->n);
}

void	trgl_hit(const void *trgl, const t_ray *r, struct s_hit *h,
		const uint32_t res)
{
	const t_trgl	*tr = (t_trgl*)trgl;
	t_vec3f			vect[2];
	float			ab[3];

	crossp(r->d, tr->e2, vect[0]);
	ab[0] = dotp(tr->e1, vect[0]);
	if (ab[0] == 0)
		return ;
	ab[0] = 1.f / ab[0];
	sub_(r->o, tr->p1, vect[1]);
	ab[1] = ab[0] * dotp(vect[1], vect[0]);
	if (ab[1] < 0.0 || ab[1] > 1.0)
		return ;
	crossp(vect[1], tr->e1, vect[0]);
	ab[2] = ab[0] * dotp(r->d, vect[0]);
	if (ab[2] < 0.0 || ab[1] + ab[2] > 1.0)
		return ;
	ab[1] = ab[0] * dotp(tr->e2, vect[0]);
	if (ab[1] >= h->t || ab[1] <= h->min)
		return ;
	h->t = ab[1];
	h->p = res;
}

uint8_t	trgl_bounding(const void *trgl, t_box *bbox)
{
	const t_trgl *const t = (t_trgl *)trgl;

	set_vector(bbox->max, fmaxf(t->p1[0], fmaxf(t->p2[0], t->p3[0])),
			fmaxf(t->p1[1], fmaxf(t->p2[1], t->p3[1])),
			fmaxf(t->p1[2], fmaxf(t->p2[2], t->p3[2])));
	set_vector(bbox->min, fminf(t->p1[0], fminf(t->p2[0], t->p3[0])),
			fminf(t->p1[1], fminf(t->p2[1], t->p3[1])),
			fminf(t->p1[2], fminf(t->p2[2], t->p3[2])));
	sub_(bbox->max, bbox->min, bbox->extent);
	return (1);
}

uint8_t	trgl_parser(struct s_prescene *ps, const char *s, uint32_t *pos_num)
{
	t_trgl *trgl;

	trgl = (t_trgl*)(ps->prmtvs_data + *pos_num);
	ps->prmtvs.prmtvs[pos_num[1]].prmtv = trgl;
	ps->prmtvs.prmtvs[pos_num[1]].mtrl = ps->mtrls_data + pos_num[1];
	ps->prmtvs.prmtvs[pos_num[1]].type = T;
	if (!(vect_parse(trgl->p1, &s)))
		return (return_message("Bad vector parsing point n1 in triangle"));
	if (!(vect_parse(trgl->p2, &s)))
		return (return_message("Bad vector parsing point n2 in triangle"));
	if (!(vect_parse(trgl->p3, &s)))
		return (return_message("Bad vector parsing point n3 in triangle"));
	sub_(trgl->p2, trgl->p1, trgl->e1);
	sub_(trgl->p3, trgl->p1, trgl->e2);
	crossp(trgl->e1, trgl->e2, trgl->n);
	normalize(trgl->n);
	if (!parse_mat(ps->mtrls_data + pos_num[1], &s, ps->mlx))
		return (return_message("Bad material for triangle"));
	++pos_num[1];
	*pos_num += sizeof(t_trgl);
	return (1);
}

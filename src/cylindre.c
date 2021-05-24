/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylindre.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <abaudot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/11 13:38:32 by abaudot           #+#    #+#             */
/*   Updated: 2021/05/23 20:30:58 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "bvh.h"

void	cyl_nrml(const void *const cyl, t_hinfo *hi)
{
	const t_cyl	*c = (t_cyl*)cyl;
	t_vec3f		oc;
	t_vec3f		tmp;
	float		y;

	sub_(hi->r->o, hi->n, oc);
	sub_(oc, c->pa, oc);
	y = dotp(c->aa, oc) + dotp(c->aa, hi->n);
	if (y > 0.001 && y + 0.001 < c->aa_length)
	{
		s_scale(c->aa, y / c->aa_length, tmp);
		add_(oc, hi->n, hi->n);
		sub_(hi->n, tmp, hi->n);
		return (s_scale(hi->n, 1.f / c->radius, hi->n));
	}
	s_scale(c->aa, 1.f / (c->aa_length * (1 - 2 * (y < 0))), hi->n);
}

static float	setxyabc(const t_cyl *cyl, const t_ray *r, float *xyhabc)
{
	t_vec3f	oc;

	sub_(r->o, cyl->pa, oc);
	xyhabc[0] = dotp(cyl->aa, r->d);
	xyhabc[1] = dotp(cyl->aa, oc);
	xyhabc[3] = cyl->aa_length - xyhabc[0] * xyhabc[0];
	xyhabc[4] = cyl->aa_length * dotp(oc, r->d) - xyhabc[1] * xyhabc[0];
	xyhabc[5] = cyl->aa_length * dotp(oc, oc) - xyhabc[1] * xyhabc[1]
		- cyl->radius2 * cyl->aa_length;
	xyhabc[2] = xyhabc[4] * xyhabc[4] - xyhabc[3] * xyhabc[5];
	return (xyhabc[2]);
}

void	cyl_hit(const void *cyl, const t_ray *r, struct s_hit *h,
		const uint32_t res)
{
	const t_cyl	*cy = (t_cyl*)cyl;
	float		xyhabc[6];
	float		ty[2];

	if (setxyabc(cy, r, xyhabc) < 0.0f)
		return ;
	xyhabc[2] = sqrtf(xyhabc[2]);
	ty[0] = (-xyhabc[4] - xyhabc[2]) / xyhabc[3];
	ty[1] = xyhabc[1] + ty[0] * xyhabc[0];
	if (ty[1] > 0 && ty[1] < cy->aa_length && ty[0] < h->t && ty[0] > h->min)
	{
		h->t = ty[0];
		h->p = res;
		return ;
	}
	ty[0] = (cy->aa_length * (ty[1] >= 0) - xyhabc[1]) / xyhabc[0];
	if (fabs(xyhabc[4] + xyhabc[3] * ty[0]) < xyhabc[2] && ty[0] < h->t
		&& ty[0] > h->min)
	{
		h->t = ty[0];
		h->p = res;
	}
}

uint8_t	cyl_bounding(const void *cyl, t_box *bbox)
{
	const t_cyl *const	cy = (t_cyl *)cyl;
	t_vec3f				e;
	t_vec3f				t1;
	t_vec3f				t2;

	mult_(cy->aa, cy->aa, e);
	s_scale(e, 1.f / cy->aa_length, e);
	sub_((t_vec3f){1, 1, 1}, e, e);
	set_vector(e, sqrtf(e[0]), sqrtf(e[1]), sqrtf(e[2]));
	s_scale(e, cy->radius, e);
	add_(cy->pa, e, t1);
	add_(cy->pb, e, t2);
	set_vector(bbox->max, fmaxf(t1[0], t2[0]), fmaxf(t1[1], t2[1]),
		fmaxf(t1[2], t2[2]));
	sub_(cy->pa, e, t1);
	sub_(cy->pb, e, t2);
	set_vector(bbox->min, fminf(t1[0], t2[0]), fminf(t1[1], t2[1]),
		fminf(t1[2], t2[2]));
	sub_(bbox->max, bbox->min, bbox->extent);
	return (1);
}

uint8_t	cyl_parser(struct s_prescene *ps, const char *s,
		uint32_t *pos_num)
{
	t_cyl	*cyl;
	t_vec3f	center_o[2];

	cyl = (t_cyl *)(ps->prmtvs_data + *pos_num);
	ps->prmtvs.prmtvs[pos_num[1]].prmtv = cyl;
	ps->prmtvs.prmtvs[pos_num[1]].mtrl = ps->mtrls_data + pos_num[1];
	ps->prmtvs.prmtvs[pos_num[1]].type = CY;
	if (!(vect_parse(center_o[0], &s)))
		return (return_message("bad vector for cylinder center"));
	if (!(vect_parse(center_o[1], &s)))
		return (return_message("bad vector for cylinder orientation"));
	normalize(center_o[1]);
	cyl->radius = ft_atof(&s) * 0.5;
	cyl->radius2 = cyl->radius * cyl->radius;
	s_scale(center_o[1], ft_atof(&s) * 0.5, center_o[1]);
	add_(center_o[0], center_o[1], cyl->pb);
	sub_(center_o[0], center_o[1], cyl->pa);
	sub_(cyl->pb, cyl->pa, cyl->aa);
	cyl->aa_length = dotp(cyl->aa, cyl->aa);
	if (!parse_mat(ps->mtrls_data + pos_num[1], &s, ps->mlx))
		return (return_message("Bad material for cylinder"));
	++pos_num[1];
	*pos_num += sizeof(t_cyl);
	return (1);
}

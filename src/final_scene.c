/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   final_scene.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <abaudot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/20 20:14:41 by abaudot           #+#    #+#             */
/*   Updated: 2021/04/25 00:53:45 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "bvh.h"

inline void		swapp(t_prmtv *a, t_prmtv *b)
{
	const t_prmtv tmp = *a;

	*a = *b;
	*b = tmp;
}

static void		destroy_presene(struct s_prescene *ps)
{
	free(ps->tracker.s);
	free(ps->prmtvs_data);
	free(ps->mtrls_data);
}

static uint8_t	reorder_data(struct s_scene *s, t_prmtv *p,
		const uint32_t n)
{
	const uint32_t	size_arr[] = {sizeof(t_sphr), sizeof(t_pln),
		sizeof(t_sqr), sizeof(t_cyl), sizeof(t_trgl), sizeof(t_cps)};
	const uint32_t	size = s->render.bvh.prmtvs.size;
	uint32_t		i;
	uint32_t		pos;

	if (!(s->prmtvs = malloc(n)))
		return (return_message("Fail to allocated memorie for prmtvs"));
	if (!(s->mtrls = (t_material*)malloc(sizeof(t_material) * size)))
		return (return_message("Fail to allocated memorie for mtrls"));
	i = 0;
	pos = 0;
	while (i < size)
	{
		s->mtrls[i] = *(p[i].mtrl);
		p[i].mtrl = &s->mtrls[i];
		ft_memcpy(s->prmtvs + pos, (p[i].prmtv), size_arr[p[i].type]);
		p[i].prmtv = s->prmtvs + pos;
		pos += size_arr[p[i].type];
		++i;
	}
	return (1);
}

uint8_t			finalscene(struct s_prescene *ps, struct s_scene *const scene)
{
	scene->cams = ps->cams;
	scene->rsltn[0] = ps->rsltn[0];
	scene->rsltn[1] = ps->rsltn[1];
	*scene->render.sky.skybox = 0;
	scene->render.lghts = ps->lghts;
	scene->count = ps->prmtvs.size;
	scene->render.bvh.prmtvs = ps->prmtvs;
	if (scene->count < 1)
	{
		free_prescene(ps, ps->mlx);
		printf("there is nothing to render\n");
		exit(0);
	}
	if (!(builder(&scene->render.bvh)))
		return (return_message("Fail to counstruct the BVH !"));
	if (!(reorder_data(scene, scene->render.bvh.prmtvs.prmtvs,
					ps->total_size)))
		return (return_message("Fail to reorder the data || finalScene.c"));
	destroy_presene(ps);
	return (1);
}

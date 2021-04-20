/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finalScene.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 14:00:04 by abaudot           #+#    #+#             */
/*   Updated: 2021/04/19 10:20:28 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "bvh.h"

static void destroy_preSene(struct s_preScene *ps)
{
	free(ps->tracker.s);
	free(ps->prmtvs_data);
	free(ps->mtrls_data);
}

static uint8_t reOrder_data(struct s_scene *s, t_prmtv *p,
		const uint32_t n)
{
	const uint32_t size_arr[] = {sizeof(t_sphr), sizeof(t_pln),
		sizeof(t_sqr), sizeof(t_cyl), sizeof(t_trgl), sizeof(t_cps)};
	const uint32_t size = s->render.bvh.prmtvs.size;
	uint32_t i;
	uint32_t pos;

	if (!(s->prmtvs = malloc(n)))
		return (return_message("Fail to allocated memorie for prmtvs"));
	if (!(s->mtrls = (t_material*)malloc(sizeof(t_material) * size)))
	   return (return_message("Fail to allocated memorie for mtrls"));
	i = 0;
	pos = 0;
//	printf("Rendering %d primitives...\n", size);
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

uint8_t	finalScene(struct s_preScene *ps, struct s_scene *const scene)
{
	scene->cams = ps->cams;
	scene->rsltn[0] = ps->rsltn[0];
	scene->rsltn[1] = ps->rsltn[1];
	*scene->render.sky.skybox = 0;
	scene->render.lghts = ps->lghts;
	scene->count = ps->prmtvs.size;
//	printf("light num: %d\n", scene->render.lghts.count);
	scene->render.bvh.prmtvs = ps->prmtvs;
	if (!(builder(&scene->render.bvh)))
		return (return_message("Fail to counstruct the BVH !"));
	if (!(reOrder_data(scene, scene->render.bvh.prmtvs.prmtvs,
				   	ps->total_size)))
		return (return_message("Fail to reorder the data || finalScene.c"));
	destroy_preSene(ps);
	return (1);
}

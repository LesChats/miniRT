/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_assange.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/19 13:44:33 by abaudot           #+#    #+#             */
/*   Updated: 2021/06/27 15:29:42 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

/*
 **	Seriously... free him
*/

static void	free_scene(struct s_scene *s, void *mlx)
{
	uint32_t	i;

	i = 0;
	while (i < s->cams.count)
		mlx_destroy_image(mlx, s->cams.cams[i++].img);
	free(s->cams.cams);
	free(s->render.lghts.lghts);
	i = 0;
	while (i < s->count)
	{
		if (s->mtrls[i].colortype == 6 && s->mtrls[i].text.img)
			mlx_destroy_image(mlx, s->mtrls[i].text.img);
		++i;
	}
	free(s->prmtvs);
	free(s->mtrls);
	free(s->render.bvh.prmtvs.prmtvs);
	free(s->render.bvh.nodes);
}

static void	free_sky(struct s_skybox *sky, void *mlx)
{
	uint32_t	i;

	i = 0;
	while (i < 6)
		mlx_destroy_image(mlx, sky->skyimg[i++]);
}

int	end_wind(struct s_mlx *m)
{
	mlx_destroy_window(m->mlx, m->mlx_win);
	if (*m->s->render.sky.skybox)
		free_sky(&m->s->render.sky, m->mlx);
	free_scene(m->s, m->mlx);
# ifdef LINUX
	mlx_destroy_display(m->mlx);
# endif
	free(m->mlx);
	exit(0);
}

void	free_prescene(struct s_prescene *ps, void *mlx)
{
	if (ps->tracker.s)
		free(ps->tracker.s);
	if (ps->prmtvs_data)
		free(ps->prmtvs_data);
	if (ps->prmtvs.prmtvs)
		free(ps->prmtvs.prmtvs);
	if (ps->mtrls_data)
		free(ps->mtrls_data);
	if (ps->cams.cams)
		free(ps->cams.cams);
	if (ps->lghts.lghts)
		free(ps->lghts.lghts);
# ifdef LINUX
	mlx_destroy_display(mlx);
# endif
	free(mlx);
}

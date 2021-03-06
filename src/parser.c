/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <abaudot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/14 14:48:09 by abaudot           #+#    #+#             */
/*   Updated: 2021/05/23 19:10:13 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static uint8_t	prmtvs_arrinit(struct s_prescene *ps, uint32_t *prmtvs_arr,
		const int fd)
{
	uint32_t	i;

	i = 0;
	while (i < PRMTVNB + 2)
		prmtvs_arr[i++] = 0;
	if (!count_prmtvs(prmtvs_arr, &ps->tracker, fd))
		return (return_message("Could not count the nth of primitives"));
	if (!prmtvs_arr[C])
		return (return_message("The sceen needs a least one camera"));
	i = 0;
	ps->prmtvs.size = 0;
	while (i < PRMTVNB)
		ps->prmtvs.size += prmtvs_arr[i++];
	close(fd);
	i = 0;
	return (1);
}

static uint8_t	struct_malloc(struct s_prescene *ps, uint32_t *prmtvs_arr)
{
	ps->total_size = prmtvs_arr[SP] * sizeof(t_sphr)
		+ prmtvs_arr[P] * sizeof(t_pln) + prmtvs_arr[SQ] * sizeof(t_sqr)
		+ prmtvs_arr[CY] * sizeof(t_cyl) + prmtvs_arr[T] * sizeof(t_trgl)
		+ prmtvs_arr[CP] * sizeof(t_cps);
	ps->prmtvs_data = malloc(ps->total_size);
	if (!ps->prmtvs_data)
		return (return_message("malloc prmtvs_data fail|| file : parser.c"));
	ps->prmtvs.prmtvs = malloc(sizeof(t_prmtv) * ps->prmtvs.size);
	if (!ps->prmtvs.prmtvs)
		return (return_message("malloc prmtvs fail || file : parser.c"));
	ps->mtrls_data = malloc(sizeof(t_material) * ps->prmtvs.size);
	if (!ps->mtrls_data)
		return (return_message("malloc mtrls_data fail || file : parser.c"));
	ps->cams.count = prmtvs_arr[C];
	ps->cams.cams = (t_cam *)malloc(sizeof(t_cam) * ps->cams.count);
	if (!ps->cams.cams)
		return (return_message("malloc camera fail || file : parser.c"));
	ps->lghts.count = prmtvs_arr[L];
	ps->lghts.lghts = (t_lght *)malloc(sizeof(t_lght) * ps->lghts.count);
	if (!ps->lghts.lghts)
		return (return_message("malloc lghts fail || file : parser.c"));
	return (1);
}

static uint8_t	first_read(const char *rt_file, struct s_prescene *ps)
{
	const int	fd = open(rt_file, O_RDONLY);
	uint32_t	prmtvs_arr[PRMTVNB + 2];

	if (fd < 0)
	{
		printf("Error: could not open \"%s\"\n", rt_file);
		mlx_destroy_display(ps->mlx);
		free(ps->mlx);
		exit(1);
	}
	if (!prmtvs_arrinit(ps, prmtvs_arr, fd))
	{
		free(ps->tracker.s);
		mlx_destroy_display(ps->mlx);
		free(ps->mlx);
		printf("Error: Could not Initializ array || file: parcer.c\n");
		exit(1);
	}
	if (!(struct_malloc(ps, prmtvs_arr)))
	{
		free_prescene(ps, ps->mlx);
		return (return_message("first read fail || parcer.c"));
	}
	return (1);
}

static uint8_t	second_read(const char *rt_file, struct s_prescene *ps)
{
	const int	fd = open(rt_file, O_RDONLY);

	if (fd < 0)
	{
		printf("%s\t:", rt_file);
		return (return_message("Could not open this file a secound time wtf"));
	}
	findambiant(ps->lghts.amb, ps->tracker.s, ps->tracker.len);
	if (!fill_myscene(ps, fd))
		return (return_message("Second read fail || parcer.c"));
	close(fd);
	return (1);
}

uint8_t	parse(const char *rt_file, struct s_scene *const scene,
		struct s_mlx *mlx)
{
	struct s_prescene	pres;

	if (!end_with_rt(rt_file))
		return (return_message("The file is not an .rt file"));
	mlx->mlx = mlx_init();
	if (!mlx->mlx)
		return (return_message("Fatal error > Fail to initialize MLX !"));
	pres.mlx = mlx->mlx;
	if (!(first_read(rt_file, &pres)))
		return (return_message("parsing fail !!\n"));
	if (!(second_read(rt_file, &pres)))
	{
		free_prescene(&pres, mlx->mlx);
		return (return_message("parsing fail !!\n"));
	}
	if (!(finalscene(&pres, scene)))
	{
		free_prescene(&pres, mlx->mlx);
		return (return_message("parsing fail !!\n"));
	}
	return (1);
}

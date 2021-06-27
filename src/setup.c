/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <abaudot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 10:31:27 by abaudot           #+#    #+#             */
/*   Updated: 2021/06/27 16:34:33 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tracer.h"

#include <stdio.h>

static void	setupcams(struct s_scene *s, struct s_mlx *mlx)
{
	const float		invx = 1.f / s->rsltn[0];
	const float		invy = 1.f / s->rsltn[1];
	const float		resx = (float)s->rsltn[0];
	t_cam			*cams;
	uint32_t		i;
	//////////////////////////
	int bit_per_pixel, line_lenght, endian;

	cams = s->cams.cams;
	i = 0;
	while (i < s->cams.count)
	{
		cams[i].img = mlx_new_image(mlx->mlx, s->rsltn[0],
				s->rsltn[1]);
		//cams[i].addr = (uint32_t *)(((t_img *)cams[i].img)->data);
		printf("%p\n", cams[i].addr);
		cams[i].addr = (uint32_t *)mlx_get_data_addr(cams[i].img,
				&bit_per_pixel, &line_lenght, &endian);
		printf("%p\n", cams[i].addr);
		cams[i].inv_x = invx;
		cams[i].inv_y = invy;
		set_cam(cams + i, resx);
		++i;
	}
}

void	setup(struct s_scene *s, struct s_mlx *mlx, uint8_t save)
{
	uint32_t	xy_size[2];

	if (save == 1)
	{
#ifdef LINUX
		mlx_get_screen_size(mlx->mlx, (int32_t *)xy_size,
			(int32_t *)(xy_size + 1));
#else
		xy_size[0] = 2560; xy_size[1] = 1440;
#endif
		if (s->rsltn[0] > (uint32_t)xy_size[0] || s->rsltn[0] < 1)
			s->rsltn[0] = xy_size[0];
		if (s->rsltn[1] > (uint32_t)xy_size[1] || s->rsltn[1] < 1)
			s->rsltn[1] = xy_size[1];
		mlx->mlx_win = mlx_new_window(mlx->mlx, s->rsltn[0],
				s->rsltn[1], "MiniRT");
	}
	mlx->s = s;
	mlx->cam_id = 0;
	setupcams(s, mlx);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <abaudot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 10:42:34 by abaudot           #+#    #+#             */
/*   Updated: 2021/05/23 20:08:08 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "tracer.h"
#include "mlxhandler.h"
/*
static int	keyboardhandle(uint32_t key, struct s_mlx *m)
{
	if (key == ESC)
		return (end_wind(m));
	if (key == SPACE)
		cameraroll(m->s->cams.cams, m);
	else if (key == A)
		cameramove(&m->s->cams.cams[m->cam_id], m, 1);
	else if (key == W)
		cameramove(&m->s->cams.cams[m->cam_id], m, 3);
	else if (key == D)
		cameramove(&m->s->cams.cams[m->cam_id], m, 4);
	else if (key == S)
		cameramove(&m->s->cams.cams[m->cam_id], m, 0);
	else if (key == Q)
		cameramove(&m->s->cams.cams[m->cam_id], m, 5);
	else if (key == E)
		cameramove(&m->s->cams.cams[m->cam_id], m, 2);
	else if (key == P_SAVE)
		scenetobmp(m->s, &m->s->cams.cams[m->cam_id]);
	else if (key == C_INFO)
		camera_message(&m->s->cams.cams[m->cam_id], m->cam_id);
	return (1);
}

static int	mousehandle(uint32_t key, int x, int y, struct s_mlx *m)
{
	float	*orient;
	int32_t	resx;
	int32_t	resy;
	t_vec3f	tmp;
	t_vec3f	tmp2;

	if (key == 1)
	{
		orient = m->s->cams.cams[m->cam_id].orient;
		resx = m->s->rsltn[0] / 2;
		resy = m->s->rsltn[1] / 2;
		mlx_mouse_get_pos(m->mlx, m->mlx_win, &x, &y);
		s_scale(m->s->cams.cams[m->cam_id].ref[2],
			(y > resy) - (y < resy), tmp);
		s_scale(m->s->cams.cams[m->cam_id].ref[1],
			(x > resx) - (x < resx), tmp2);
		add_(tmp, tmp2, tmp);
		add_(orient, tmp, orient);
		set_cam(m->s->cams.cams + m->cam_id, m->s->rsltn[0]);
		render(m->s, m->s->cams.cams + m->cam_id);
		mlx_put_image_to_window(m->mlx, m->mlx_win,
			m->s->cams.cams[m->cam_id].img, 0, 0);
	}
	return (0);
}
static void	hookloop(struct s_mlx *mlx, uint8_t save)
{
	if (save != 2)
	{
		mlx_put_image_to_window(mlx->mlx, mlx->mlx_win,
			mlx->s->cams.cams[0].img, 0, 0);
		mlx_hook(mlx->mlx_win, CLOSE, 0L, end_wind, mlx);
		mlx_hook(mlx->mlx_win, 2, 1, keyboardhandle, mlx);
		mlx_hook(mlx->mlx_win, 4, 4, mousehandle, mlx);
		mlx_loop(mlx->mlx);
	}
}
*/
static uint8_t	option(int ac, char **av, struct s_scene *s, void *mlx)
{
	uint8_t	sky;
	uint8_t	res;
	int32_t	i;

	sky = 0;
	res = 1;
	i = 0;
	while (i < ac)
	{
		if (!ft_strncmp(av[i], "--save", 6))
			res = 2;
		else if (!sky)
		{
			if (!setskybox(&s->render.sky, av[i], mlx))
				return (return_message("accepted --save or skymap dir"));
			sky = 1;
		}
		else
			return (return_message("To many or invalid argument !"));
		++i;
	}
	return (res);
}

int	main(int ac, char **av)
{
	struct s_scene	scene;
	struct s_mlx	mlx;
	uint32_t		i;
	uint8_t			save;

	save = 1;
	if (ac < 2)
		return (return_message("MiniRT should get at least one argv"));
	if (!parse(av[1], &scene, &mlx))
		return (return_message("Parsing error >>>> EXIT"));
	if (ac > 2)
		save = option(ac - 2, av + 2, &scene, mlx.mlx);
	if (!save)
		return (1);
	setup(&scene, &mlx, save);
	info_message(&scene);
	i = 0;
	while (i < scene.cams.count)
		render(&scene, &scene.cams.cams[i++]);
	if (save == 2)
		if (!scenetobmp(&scene, scene.cams.cams))
			return_messageonly("While saving scene to .bmp !");
	hookloop(&mlx, save);
	return (0);
}

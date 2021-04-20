/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 18:27:48 by abaudot           #+#    #+#             */
/*   Updated: 2021/04/19 19:29:44 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "tracer.h"
#include "mlxHandler.h"

int	keyboardHandle(uint32_t key, struct s_mlx *m)
{
	if (key == ESC)
		return (end_wind(m));
	if (key == SPACE)
		cameraRoll(m->s->cams.cams, m);
	else if (key == A)
		cameraMove(&m->s->cams.cams[m->camID], m, 4);
	else if ( key == W)
		cameraMove(&m->s->cams.cams[m->camID], m, 0);
	else if (key == D)
		cameraMove(&m->s->cams.cams[m->camID], m, 5);
	else if (key == S)
		cameraMove(&m->s->cams.cams[m->camID], m, 1);
	else if (key == Q)
		cameraMove(&m->s->cams.cams[m->camID], m, 3);
	else if (key == E)
		cameraMove(&m->s->cams.cams[m->camID], m, 2);
	else if (key == PK)
		sceneToBmp(m->s, &m->s->cams.cams[m->camID]);
	//else if (key == C)
	//	printCam()
	return (1);
}

int MouseHandle(uint32_t key, int x, int y, struct s_mlx *m)
{
	float *orient;
	const int32_t	resX = m->s->rsltn[0] / 2;
	const int32_t	resY = m->s->rsltn[1] / 2;

	orient = m->s->cams.cams[m->camID].orient;
	if (key == 1)
	{
		mlx_mouse_get_pos(m->mlx, m->mlx_win, &x, &y);
		add_(orient, (t_vec3f){(x > resX) * 2, (y > resY) * -0.25, 0}, orient);
		add_(orient, (t_vec3f){(x < resX) * -2, (y < resY) * 0.25, 0}, orient);
		set_cam(m->s->cams.cams + m->camID, m->s->rsltn[0]);
		render(m->s, m->s->cams.cams + m->camID);
		mlx_clear_window(m->mlx, m->mlx_win);
		mlx_put_image_to_window(m->mlx, m->mlx_win, m->s->cams.cams[m->camID].img, 0, 0);
	}
	return (0);
}

static void hookLoop(struct s_mlx *mlx)
{
	mlx_put_image_to_window(mlx->mlx, mlx->mlx_win,
		   	mlx->s->cams.cams[0].img, 0, 0);
	mlx_hook(mlx->mlx_win, CLOSE, 0L, end_wind, mlx);
	mlx_hook(mlx->mlx_win, 2, 1, keyboardHandle, mlx); 
	mlx_hook(mlx->mlx_win, 4, 4, MouseHandle, mlx); 
	mlx_loop(mlx->mlx);
}

static uint8_t option(int ac, char **av, struct s_scene *s, struct s_mlx *mlx)
{
	uint8_t sky;
	uint8_t res;
	int32_t i;

	sky = 0;
	res = 1;
	i = 0;
	//printf("%d\n",ac);

		while (i < ac)
		{
			if (!ft_strncmp(av[i], "--save", 6))
				res = 2;
			else if (!sky)
			{
				if (!setSkybox(&s->render.sky, av[i], mlx->mlx))
					return (return_message("accepted --save or skymap dir"));
				sky = 1;
			}
			else
				return (return_message("To many or invalid argument !"));
			++i;
		}
		return (res);
}

int main(int ac, char **av)
{
	struct s_scene	scene;
	struct s_mlx	mlx;
	uint32_t		i;
	uint8_t			save;

	save = 1;
	if (ac < 2)
		return (return_message("MiniRT should get at least one argv"));
	if (!parse(av[1], &scene, &mlx) || !mlx.mlx)
		return (return_message("Parsing error >>>> EXIT"));
	setup(&scene, &mlx);
//	infoMessage(&scene);
	if (ac > 2)
		save = option(ac - 2, av + 2, &scene, &mlx);
	if (!save)
		return (1);
	i = 0;
	while (i < scene.cams.count)
		render(&scene, &scene.cams.cams[i++]);
	if (save == 2)
		if (!sceneToBmp(&scene, scene.cams.cams))
			return_messageOnly("While saving scene to .bmp !");
	hookLoop(&mlx);
	return (0);
}

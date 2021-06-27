/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hookloop.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <abaudot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 10:42:34 by abaudot           #+#    #+#             */
/*   Updated: 2021/06/27 15:51:38 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "tracer.h"
#include "mlxhandler.h"

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
#ifdef LINUX
		mlx_mouse_get_pos(m->mlx, m->mlx_win, &x, &y);
#else
		mlx_mouse_get_pos(m->mlx,  &x, &y);
#endif
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

static int	frame(struct s_mlx *mlx)
{
	mlx_put_image_to_window(mlx->mlx, mlx->mlx_win,
		mlx->s->cams.cams[mlx->cam_id].img, 0, 0);
	return (0);
}

void	hookloop(struct s_mlx *mlx, uint8_t save)
{
	if (save != 2)
	{
		mlx_loop_hook(mlx->mlx, frame, mlx);
		mlx_hook(mlx->mlx_win, CLOSE, 0L, end_wind, mlx);
		mlx_hook(mlx->mlx_win, 2, 1, keyboardhandle, mlx);
		mlx_hook(mlx->mlx_win, 4, 4, mousehandle, mlx);
		mlx_loop(mlx->mlx);
	}
}

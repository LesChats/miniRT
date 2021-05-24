/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <abaudot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 10:27:47 by abaudot           #+#    #+#             */
/*   Updated: 2021/05/24 18:21:04 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tracer.h"

void	genray(const t_cam *cam, t_ray *ray, const float i,
		const float j)
{
	t_vec3f		a;
	t_vec3f		b;
	const float	u = i * cam->inv_x;
	const float	v = j * cam->inv_y;

	equal_(ray->o, cam->pts);
	s_scale(cam->hrzt, u, a);
	s_scale(cam->vrtl, v, b);
	add_(a, b, a);
	add_(a, cam->left, ray->d);
	normalize(ray->d);
	set_vector(ray->inv_d, 1.f / ray->d[0], 1.f / ray->d[1], 1.f / ray->d[2]);
}

static inline float	degrees_to_radians(const float degrees)
{
	return (degrees * PI_180);
}

void	set_cam(t_cam *cam, const float xres)
{
	float	tmp;
	t_vec3f	u;
	t_vec3f	v;

	sub_(cam->pts, cam->orient, cam->ref[0]);
	normalize(cam->ref[0]);
	crossp((t_vec3f){0.f, 1.f, 0.f}, cam->ref[0], cam->ref[1]);
	normalize(cam->ref[1]);
	crossp(cam->ref[1], cam->ref[0], cam->ref[2]);
	tmp = tan(degrees_to_radians(cam->fov) * 0.5f) * 2.f;
	s_scale(cam->ref[2], tmp, cam->vrtl);
	tmp *= (xres * cam->inv_y);
	s_scale(cam->ref[1], tmp, cam->hrzt);
	s_scale(cam->hrzt, 0.5, u);
	s_scale(cam->vrtl, 0.5, v);
	add_(u, v, u);
	add_(u, cam->ref[0], v);
	inv_(v, cam->left);
}

void	cameramove(t_cam *cam, const struct s_mlx *m, uint32_t a)
{
	if (a % 2)
		sub_(cam->pts, cam->ref[a % 3], cam->pts);
	else
		add_(cam->pts, cam->ref[a % 3], cam->pts);
	set_cam(cam, m->s->rsltn[0]);
	render(m->s, cam);
	mlx_put_image_to_window(m->mlx, m->mlx_win, cam->img, 0, 0);
}

void	cameraroll(const t_cam *cam, struct s_mlx *m)
{
	m->cam_id = (m->cam_id + 1) % m->s->cams.count;
	mlx_clear_window(m->mlx, m->mlx_win);
	mlx_put_image_to_window(m->mlx, m->mlx_win, cam[m->cam_id].img, 0, 0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 13:28:34 by abaudot           #+#    #+#             */
/*   Updated: 2021/04/18 12:20:15 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tracer.h"

void	genRay(const t_cam *cam, t_ray *ray, const float i,
	   	const float j)
{
	t_vec3f a;
	t_vec3f b;
	const float u = i * cam->inv_x;
	const float v = j * cam->inv_y;

	equal_(ray->o, cam->pts);
	s_scale(cam->hrzt, u, a);
	s_scale(cam->vrtl, v, b);
	add_(a, b, a);
	add_(a, cam->left, ray->d);
	normalize(ray->d);
	set_vector(ray->inv_d, 1.f / ray->d[0], 1.f / ray->d[1], 1.f / ray->d[2]);
}

static inline float degrees_to_radians(const float degrees)
{
	return (degrees * PI_180);
}

void	set_cam(t_cam *cam, const float xres)
{
	float	tmp;
	t_vec3f	v;
	t_vec3f u;
	t_vec3f w;

	sub_(cam->pts, cam->orient, w);
	normalize(w);
	crossp((t_vec3f){0,1,0}, w, u);
	normalize(u);
	crossp(u, w, v);
	tmp = tan(degrees_to_radians(cam->fov) * 0.5f) * 2.f;
	s_scale(v, tmp, cam->vrtl);
	tmp *= (xres * cam->inv_y);
	s_scale(u, tmp, cam->hrzt);
	s_scale(cam->hrzt, 0.5, u);
	s_scale(cam->vrtl, 0.5, v);
	add_(u, v, u);
	add_(u, w, v);
	inv_(v, cam->left);
}

void cameraMove(t_cam *cam, const struct s_mlx *m, uint32_t a)
{
	static const t_vec3f moves[] = {{-2, 0, 0}, {2, 0, 0}, {0, 2, 0},
		{0, -2, 0}, {0, 0, 2}, {0, 0, -2}};
	
	add_(cam->pts, moves[a], cam->pts);
	set_cam(cam, m->s->rsltn[0]);
	render(m->s, cam);
	mlx_clear_window(m->mlx, m->mlx_win);
	mlx_put_image_to_window(m->mlx, m->mlx_win, cam->img, 0, 0);
}

void cameraRoll(const t_cam *cam, struct s_mlx *m)
{
	m->camID = (m->camID + 1) % m->s->cams.count;
	mlx_clear_window(m->mlx, m->mlx_win);
	mlx_put_image_to_window(m->mlx, m->mlx_win, cam[m->camID].img, 0, 0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/18 20:53:34 by abaudot           #+#    #+#             */
/*   Updated: 2021/04/19 10:29:21 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tracer.h"

static uint32_t	return_trgb(t_vec3f color, uint8_t filter)
{
	if (filter)
	{
		color[0] = color[0] * 0.393 + color[1] * 0.769 + color[2] * 0.189;
		color[1] = color[0] * 0.349 + color[1] * 0.686 + color[2] * 0.168;
		color[2] = color[0] * 0.272 + color[1] * 0.534 + color[2] * 0.131;
	}
	s_scale(color, 255.f, color);
	if (color[0] > 255.f)
		color[0] = 255.f;
	if (color[1] > 255.f)
		color[1] = 255.f;
	if (color[2] > 255.f)
		color[2] = 255.f;

	return((uint32_t)color[0] << 16 | (uint32_t)color[1] << 8 |
			(uint32_t)color[2]);
}

static void sum_color(t_vec3f *colors, const uint32_t num)
{
	uint32_t i;

	i = 1;
	while (i < num)
	{
		colors[0][0] += colors[i][0];
		colors[0][1] += colors[i][1];
		colors[0][2] += colors[i][2];
		++i;
	}
	s_scale(colors[0], 1.f / num, colors[0]);
}

static inline uint32_t	sampler(const struct s_bvhL *s, const t_cam *cam,
	   const float a, const float b)
{
	t_ray r[ANTI2];
	t_vec3f colors[ANTI2];
	const float offset = 1.f /  ANTIALIASING;
	int32_t i;
	int32_t j;

	i = -1;
	while (++i < ANTIALIASING)
	{
		j = -1;
		while (++j < ANTIALIASING)
			genRay(cam, r + i * ANTIALIASING + j, a + (((float)i) * offset),
				   	b + (((float)j) * offset));
	}
	i = -1;
	while (++i < ANTIALIASING)
	{
		j = -1;
		while (++j < ANTIALIASING)
			getColor(s, r + i * ANTIALIASING + j,
				   	colors[i * ANTIALIASING + j], 0);
	}
	sum_color(colors, ANTI2);
	return(return_trgb(colors[0], cam->filter));
}

static void	rendeTr(void *data)
{
	t_tdata *d = (t_tdata *)data;
	uint32_t		ij[2];
	int32_t		i;
	int32_t		j;
	uint32_t		tile;

	while (1)
	{
		tile = d->id++;
		if (tile > d->numtiles)
		   break ;
		ij[0] = TILESIZE * (tile % d->ntlsX);
		ij[1] = TILESIZE * (tile / d->ntlsX);
		j = -1;
		while (++j < TILESIZE)
		{
			i = -1;
			while (++i < TILESIZE)
			{
				d->cam->addr[(ij[1] + j) * d->res + (ij[0] + i)] =
				   	sampler(d->s, d->cam, ij[0] + i, ij[1] + j);
			}
		}
	}
}

void	render(const struct s_scene *s, const t_cam *cam)
{
	t_tdata		tdata;
	pthread_t	thread[NUM_THREADS];
	uint32_t	i;

	tdata.id = 0;
	tdata.s = &s->render;
	tdata.cam = cam;
	tdata.ntlsX = s->rsltn[0] / TILESIZE;
	tdata.ntlsY = s->rsltn[1] / TILESIZE;
	tdata.numtiles = tdata.ntlsX * tdata.ntlsY;
	tdata.res = s->rsltn[0];
	i = 0;
	while (i < NUM_THREADS)
		pthread_create(&thread[i++], NULL,(void *)&rendeTr, &tdata);
	i = 0;
	while (i < NUM_THREADS)
		pthread_join(thread[i++], NULL);
}

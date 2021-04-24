/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/22 11:19:23 by abaudot           #+#    #+#             */
/*   Updated: 2021/04/23 15:48:28 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "tracer.h"

static void	functionality(void)
{
	printf("\033[0;33m Camera mouve with: Q W E A S D and camera orient with");
	printf(" mouse clic.\n Camera roll with SPACE barre. Press P");
	printf(" for save your image or C to get camera informations <3\33[0m\n");
}

void		info_message(struct s_scene *s)
{
	t_string	st;
	int			fd;

	fd = open("assets/minirt.txt", O_RDONLY);
	if (fd < 0)
		return (return_messageonly("Could not find intro message"));
	if (!initilize_string(&st, 64))
		return (return_messageonly("string init error: file info.c"));
	while (getnextline(fd, &st) > 0)
	{
		printf("\033[0;36m%s\033[0m\n", st.s);
		st.len = 0;
	}
	close(fd);
	printf("\n\033[0;31m Rendering info:\033[0m\n");
	printf("\033[0;32m - Resolution: %d/%d\n", s->rsltn[0], s->rsltn[1]);
	printf(" - Primitives number:\t\t\t%d\n", s->count);
	printf(" - Number of cameras:\t\t\t%d\n", s->cams.count);
	printf(" - tNumber of lights:\t\t\t%d\n", s->render.lghts.count);
	printf(" - Number of thread:\t\t\t%d\n", NUM_THREADS);
	printf(" - Ray per pixel (antialiasing):\t%d\033[0m\n\n", ANTI2);
	functionality();
	free(st.s);
}

void		return_messageonly(const char *mess)
{
	printf("error:\t%s\n", mess);
	return ;
}

uint8_t		return_message(const char *mess)
{
	printf("error:\t%s\n", mess);
	return (0);
}

void		camera_message(t_cam *c, uint32_t cam_id)
{
	printf("\n\033[0;31m Cam info:\033[0m\n");
	printf("\033[0;33m - Cam ID:\t\t%d\n", cam_id);
	printf(" - Cam fov:\t\t%.3f\n", c->fov);
	printf(" - Cam pos:\t\t%.3f/%.3f/%.3f\n", c->pts[0], c->pts[1], c->pts[2]);
	printf(" - Cam orientaton:\t%.3f/%.3f/%.3f\033[0m\n", c->orient[0],
			c->orient[1], c->orient[2]);
}

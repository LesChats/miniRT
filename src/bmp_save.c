/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bmp_save.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/01 16:07:46 by abaudot           #+#    #+#             */
/*   Updated: 2021/04/19 21:42:16 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static uint8_t	writeHeader(const struct s_scene *s, const int fd)
{
	static uint8_t header[54];

	header[0] = 0x42;
	header[1] = 0x4D;
	*(uint32_t*)(header + 2) = ((s->rsltn[0] * s->rsltn[1]) + 54);
	header[10] = 0x36;
	header[14] = 40;
	*(uint32_t*)(header + 18) = (s->rsltn[0]);
	*(uint32_t*)(header + 22) = (s->rsltn[1]);
	header[26] = 1;
	header[28] = 32;
	*(uint32_t*)(header + 34) = ((s->rsltn[0] * s->rsltn[1]));
	*(uint32_t*)(header + 38) = 2835;
	*(uint32_t*)(header + 42) = 2835;
	return (write(fd, header, 54) + 1);
}

static uint8_t writeImage(const struct s_scene *s, const t_cam *cam, const int fd)
{
	const uint32_t	size = s->rsltn[0] * s->rsltn[1];
	uint32_t		*image;
	int32_t			i;
	uint32_t		j;

	if (!(image = (uint32_t*)malloc(sizeof(uint32_t) * size)))
		return (return_message("could not malloc space for --save !"));
	i = s->rsltn[1] - 1;
	while (i >= 0)
	{
		j = 0;
		while (j < s->rsltn[0])
		{
			image[i * s->rsltn[0] +  j] = cam->addr[((s->rsltn[1] - 1) - i)
			   	* s->rsltn[0] + j];
			++j;
		}
		--i;
	}
	if (!(write(fd, image, size * 4)))
		return (return_message("could not write into bmp file"));
	free(image);
	return (1);
}

uint8_t	sceneToBmp(const struct s_scene *s, const t_cam * cam)
{
	int				fd;
	static uint8_t	num = '0';
	char			name[20];
	
	printf("Converting scene to bmp...\n");
	ft_memcpy(name, "images/output_", 14);
	ft_memcpy(name + 14, &num, 1);
	ft_memcpy(name + 15, ".bmp", 4);
	name[19] = 0;
	++num;
	if ((fd = open(name,
				   	O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR)) < 1)
		return (0);
	if (!writeHeader(s, fd))
		return(return_message("writing the header --bmp_save.c"));
	printf("image was save as:\t%s\n", name);
	return (writeImage(s, cam, fd));
}

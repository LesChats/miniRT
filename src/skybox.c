/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skybox.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/30 22:54:20 by abaudot           #+#    #+#             */
/*   Updated: 2021/04/17 20:41:11 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "tracer.h"
#include "mlx_int.h"

//remove this
/*
void		setSkybox(struct s_skybox *sky,  char *filename, void *mlx)
{
	const uint32_t	n = ft_strlen(filename);
	char relativePath[1024];

	if (n > 1015 || open(filename, O_DIRECTORY) < 0)
		return (return_messageOnly("skybox path is to long"));		
	ft_memcpy(relativePath, filename, n);
	ft_memcpy(relativePath + n, "posx.xpm", 9);
	sky->skyImg[0] = mlx_xpm_file_to_image(mlx, relativePath,
		   	&sky->widthU, &sky->heightV);
	ft_memcpy(relativePath + n, "negx.xpm", 9);
	sky->skyImg[1] = mlx_xpm_file_to_image(mlx, relativePath,
		   	&sky->widthU, &sky->heightV);
	ft_memcpy(relativePath + n, "posy.xpm", 9);
	sky->skyImg[2] = mlx_xpm_file_to_image(mlx, relativePath,
		   	&sky->widthU, &sky->heightV);
	ft_memcpy(relativePath + n, "negy.xpm", 9);
	sky->skyImg[3] = mlx_xpm_file_to_image(mlx, relativePath,
		   	&sky->widthU, &sky->heightV);
	ft_memcpy(relativePath + n, "posz.xpm", 9);
	sky->skyImg[4] = mlx_xpm_file_to_image(mlx, relativePath,
		   	&sky->widthU, &sky->heightV);
	ft_memcpy(relativePath + n, "negz.xpm", 9);
	sky->skyImg[5] = mlx_xpm_file_to_image(mlx, relativePath,
		   	&sky->widthU, &sky->heightV);
}
*/
void	setColorFromInt(int32_t	c, t_vec3f color) 
{
	color[0] = (float)((c & 0x00FF0000) >> 16);
	color[1] = (float)((c & 0x0000FF00) >> 8);
	color[2] = (float)(c & 0x000000FF);
	s_scale(color, COLOR_INV, color);
}

static uint32_t	getMaxIndice(const t_vec3f r, float *u, float *v)
{
	const float	absX = fabs(r[0]);
	const float	absY = fabs(r[1]);
	const float	absZ = fabs(r[2]);

	if (absX >= absY && absX >= absZ)
	{
		*u = 1.f - (r[2] / r[0] + 1.f) * 0.5f;
		*v = (1.f - (r[1] / r[0] + 1.f) * 0.5f) *
			(-1 + 2 * (r[0] > 0)) + (r[0] < 0);;
		return (0 + (r[0] < 0.f));
	}
	if (absY >= absX && absY >= absZ)
	{
		*u = ((r[0] / r[1] + 1.f) * 0.5f) * (-1 + 2 * (r[1] > 0)) + (r[1] < 0);
		*v = ((r[2] / r[1] + 1.f) * 0.5f);
		return (2 + (r[1] < 0.f));
	}
	*u = (r[0] / r[2] + 1.f) * 0.5f;
	*v = (1.f - (r[1] / r[2] + 1.f) * 0.5f) *
		(-1 + 2 * (r[2] > 0)) + (r[2] < 0);
	return (4 + (r[2] < 0.f));
}

static void	getSkyboxTexture(const t_vec3f ray, t_vec3f color,
		const struct s_skybox *sky)
{
	float u;
	float v;
	uint32_t index;
	
	index = getMaxIndice(ray, &u, &v);
	
	u *= (sky->widthU - 1);
	v *= (sky->heightV - 1);
	//wrong * sky->Wi insted
	setColorFromInt(((int *)sky->skybox[index])[(int)v * sky->heightV +
			(int)u], color);
}

void	getBackGround(const struct s_skybox *sky, const t_vec3f ray,
	   	t_vec3f color)
{
	if (*sky->skybox == NULL)
		return (set_vector(color, 0.f, 0.f, 0.f));
	return (getSkyboxTexture(ray, color, sky));
}

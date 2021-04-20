/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loadTexture.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/17 15:14:10 by abaudot           #+#    #+#             */
/*   Updated: 2021/04/19 19:28:59 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "mlx_int.h"

static void		skyImg(struct s_skybox *sky,  char *relativePath, void *mlx,
		const uint32_t n)
{
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

uint8_t		setSkybox(struct s_skybox *sky,  char *filename, void *mlx)
{
	const uint32_t	n = ft_strlen(filename);
	uint32_t		i;
	char			relativePath[1024];

	if (n > 1015 || open(filename, O_DIRECTORY) < 0)
		return (return_message("Wrong path or too long path -fatal"));
	ft_memcpy(relativePath, filename, n);
	skyImg(sky, relativePath, mlx, n);
	i = 0;
	while (i < 6)
	{
		if (sky->skyImg[i])
			sky->skybox[i] = (int*)(((t_img*)sky->skyImg[i])->data);
		else
		{
			return(return_message(
				"Loading skybox, the programe may leak so it stop now\n"));
			*sky->skybox = NULL;
			break ;
		}
		++i;
	}
	return (1);
}

void	grabTexture(void * mlx, struct s_texture *t, const char **s)
{
	char		relativePath[1024];
	uint32_t	i;
	
	while (**s == ' ' || **s == '\t')
		++(*s);
	i = 0;
	while (**s && **s != ' ' && **s != '\t' && i < 1024)
		relativePath[i++] = *((*s)++);
	if (i == 1024)
		return (return_messageOnly("Path to texture is to long"));
	relativePath[i] = 0;
	t->img = mlx_xpm_file_to_image(mlx, relativePath,&t->wdth, &t->hght);
	if (t->img)
		t->addr = (int*)((t_img*)t->img)->data;
	else
		return_messageOnly("Occure while loading texture, color is black now");
}

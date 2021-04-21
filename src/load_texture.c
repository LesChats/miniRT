/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_texture.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/17 15:14:10 by abaudot           #+#    #+#             */
/*   Updated: 2021/04/21 13:45:49 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "mlx_int.h"

static void	skyimg(struct s_skybox *sky, char *relativepath, void *mlx,
		const uint32_t n)
{
	ft_memcpy(relativepath + n, "posx.xpm", 9);
	sky->skyimg[0] = mlx_xpm_file_to_image(mlx, relativepath,
			&sky->widthu, &sky->heightv);
	ft_memcpy(relativepath + n, "negx.xpm", 9);
	sky->skyimg[1] = mlx_xpm_file_to_image(mlx, relativepath,
			&sky->widthu, &sky->heightv);
	ft_memcpy(relativepath + n, "posy.xpm", 9);
	sky->skyimg[2] = mlx_xpm_file_to_image(mlx, relativepath,
			&sky->widthu, &sky->heightv);
	ft_memcpy(relativepath + n, "negy.xpm", 9);
	sky->skyimg[3] = mlx_xpm_file_to_image(mlx, relativepath,
			&sky->widthu, &sky->heightv);
	ft_memcpy(relativepath + n, "posz.xpm", 9);
	sky->skyimg[4] = mlx_xpm_file_to_image(mlx, relativepath,
			&sky->widthu, &sky->heightv);
	ft_memcpy(relativepath + n, "negz.xpm", 9);
	sky->skyimg[5] = mlx_xpm_file_to_image(mlx, relativepath,
			&sky->widthu, &sky->heightv);
}

uint8_t		setskybox(struct s_skybox *sky, char *filename, void *mlx)
{
	const uint32_t	n = ft_strlen(filename);
	uint32_t		i;
	char			relativepath[1024];

	if (n > 1015 || open(filename, O_DIRECTORY) < 0)
		return (return_message("Wrong path or too long path -fatal"));
	ft_memcpy(relativepath, filename, n);
	skyimg(sky, relativepath, mlx, n);
	i = 0;
	while (i < 6)
	{
		if (sky->skyimg[i])
			sky->skybox[i] = (int*)(((t_img*)sky->skyimg[i])->data);
		else
		{
			return (return_message(
					"Loading skybox, the programe may leak so it stop now\n"));
			*sky->skybox = NULL;
			break ;
		}
		++i;
	}
	return (1);
}

void		grabtexture(void *mlx, struct s_text *t, const char **s)
{
	char		relativepath[1024];
	uint32_t	i;

	while (**s == ' ' || **s == '\t')
		++(*s);
	i = 0;
	while (**s && **s != ' ' && **s != '\t' && i < 1024)
		relativepath[i++] = *((*s)++);
	if (i == 1024)
		return (return_messageonly("Path to texture is to long"));
	relativepath[i] = 0;
	t->img = mlx_xpm_file_to_image(mlx, relativepath, &t->wdth, &t->hght);
	if (t->img)
		t->addr = (int*)((t_img*)t->img)->data;
	else
		return_messageonly("Occure while loading texture, color is black now");
}

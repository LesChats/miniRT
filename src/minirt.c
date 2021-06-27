/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <abaudot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 10:42:34 by abaudot           #+#    #+#             */
/*   Updated: 2021/06/27 16:31:33 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "tracer.h"
#include "mlxhandler.h"

static uint8_t	option(int ac, char **av, struct s_scene *s, void *mlx)
{
	uint8_t	sky;
	uint8_t	res;
	int32_t	i;

	sky = 0;
	res = 1;
	i = 0;
	while (i < ac)
	{
		if (!ft_strncmp(av[i], "--save", 6))
			res = 2;
		else if (!sky)
		{
			if (!setskybox(&s->render.sky, av[i], mlx))
				return (return_message("accepted --save or skymap dir"));
			sky = 1;
		}
		else
			return (return_message("To many or invalid argument !"));
		++i;
	}
	return (res);
}

int	main(int ac, char **av)
{
	struct s_scene	scene;
	struct s_mlx	mlx;
	uint32_t		i;
	uint8_t			save;

	save = 1;
	if (ac < 2)
		return (return_message("MiniRT should get at least one argv"));
	if (!parse(av[1], &scene, &mlx))
		return (return_message("Parsing error >>>> EXIT"));
	if (ac > 2)
		save = option(ac - 2, av + 2, &scene, mlx.mlx);
	if (!save)
		return (1);
	setup(&scene, &mlx, save);
	info_message(&scene);
	i = 0;
	while (i < scene.cams.count)
		render(&scene, &scene.cams.cams[i++]);
	if (save == 2)
		if (!scenetobmp(&scene, scene.cams.cams))
			return_messageonly("While saving scene to .bmp !");
	hookloop(&mlx, save);
	return (0);
}

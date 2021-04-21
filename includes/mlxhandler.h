/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlxHandler.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 18:21:49 by abaudot           #+#    #+#             */
/*   Updated: 2021/04/20 17:18:23 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MLXHANDLER_H
# define MLXHANDLER_H

# define ESC	65307
# define A		97
# define S		115
# define D		100
# define Q		113
# define W		119
# define E		101
# define PK		112
# define CLOSE	33
# define SPACE  32

void	cameramove(t_cam *cam, const struct s_mlx *m, uint32_t a);
void	cameraroll(const t_cam *cam, struct s_mlx *m);
int		end_wind(struct s_mlx *m);
#endif

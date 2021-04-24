/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strings_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <abaudot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 12:01:09 by abaudot           #+#    #+#             */
/*   Updated: 2021/04/24 23:49:11 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>

uint32_t	ft_strlen(const char *s)
{
	uint32_t i;

	i = 0;
	while (s[i])
		++i;
	return (i);
}

uint8_t		end_with_rt(const char *s)
{
	uint32_t i;

	i = ft_strlen(s);
	if (i < 4)
		return (0);
	if ((s[i - 1] == 't') && (s[i - 2] == 'r') && (s[i - 3] == '.'))
		return (1);
	return (0);
}

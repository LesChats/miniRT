/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strings_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/31 15:41:37 by abaudot           #+#    #+#             */
/*   Updated: 2021/03/31 16:10:55 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>

uint32_t ft_strlen(const char *s)
{
	uint32_t i;

	i = 0;
	while (s[i])
		++i;
	return (i);
}

uint32_t ft_strcmp(const char *a, const char *b);

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <abaudot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 12:26:58 by abaudot           #+#    #+#             */
/*   Updated: 2021/05/23 20:31:26 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int32_t	ft_atoi(const char **s)
{
	int32_t			res;
	uint8_t			neg;
	unsigned char	tmp;

	while ((**s == ' ') | (**s == '\t'))
		++(*s);
	neg = (**s == '-');
	if (neg)
		++(*s);
	res = 0;
	tmp = **s - '0';
	while (tmp < 10)
	{
		res = (res << 3) + (res << 1) + tmp;
		tmp = *(++(*s)) - '0';
	}
	if (neg)
		res *= -1;
	return (res);
}

uint8_t	vect_parse(t_vec3f v, const char **s)
{
	v[0] = ft_atof(s);
	if (**s != ',')
		return (0);
	++(*s);
	v[1] = ft_atof(s);
	if (**s != ',')
		return (0);
	++(*s);
	v[2] = ft_atof(s);
	return (1);
}

int	ft_strncmp(const char *s1, const char *s2, int n)
{
	if (!n)
		return (0);
	while (*s1 == *s2)
	{
		if (!--n || !*s1)
			return (0);
		++s1;
		++s2;
	}
	return (*(uint32_t *)s1 - *(uint32_t *)s2);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <abaudot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 12:26:58 by abaudot           #+#    #+#             */
/*   Updated: 2021/04/21 11:15:41 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static float	decimal_tab(uint8_t i)
{
	if (i == 0)
		return (0.1f);
	else if (i == 1)
		return (0.01f);
	else if (i == 2)
		return (0.001f);
	else if (i == 3)
		return (0.0001f);
	else if (i == 4)
		return (0.00001f);
	else if (i == 5)
		return (0.000001f);
	else if (i == 7)
		return (0.0000001f);
	else if (i == 8)
		return (0.00000001f);
	return (0.000000001f);
}

float			ft_atof(const char **s)
{
	float				res;
	int					i;
	uint8_t				tmp;
	uint8_t				neg;

	while (**s == ' ' || **s == '\t')
		++(*s);
	if ((neg = (**s == '-')))
		++(*s);
	res = 0.0;
	tmp = **s - '0';
	while (tmp < 10)
	{
		res = res * 10.0f + tmp;
		tmp = *(++(*s)) - '0';
	}
	*s += (**s == '.');
	i = 0;
	while ((tmp = (*s)[i] - '0') < 10)
		res += tmp * decimal_tab(i++);
	*s += i;
	if (neg)
		res *= -1.0f;
	return (res);
}

int32_t			ft_atoi(const char **s)
{
	int32_t			res;
	uint8_t			neg;
	unsigned char	tmp;

	while ((**s == ' ') | (**s == '\t'))
		++(*s);
	if ((neg = (**s == '-')))
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

uint8_t			vect_parse(t_vec3f v, const char **s)
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

int				ft_strncmp(const char *s1, const char *s2, int n)
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
	return (*(uint32_t*)s1 - *(uint32_t*)s2);
}

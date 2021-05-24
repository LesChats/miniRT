/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strings_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <abaudot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 12:01:09 by abaudot           #+#    #+#             */
/*   Updated: 2021/05/24 17:17:42 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>

uint32_t	ft_strlen(const char *s)
{
	uint32_t	i;

	i = 0;
	while (s[i])
		++i;
	return (i);
}

uint8_t	end_with_rt(const char *s)
{
	uint32_t	i;

	i = ft_strlen(s);
	if (i < 4)
		return (0);
	if ((s[i - 1] == 't') && (s[i - 2] == 'r') && (s[i - 3] == '.'))
		return (1);
	return (0);
}

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
	else if (i == 9)
		return (0.000000001f);
	else if (i == 10)
		return (0.0000000001f);
	else if (i == 11)
		return (0.00000000001f);
	return (0.000000000001f);
}

static float	decimal_part(const char **s)
{
	int		i;
	uint8_t	tmp;
	float	res;

	res = 0.f;
	i = 0;
	tmp = (*s)[i] - '0';
	while (tmp < 10)
	{
		res += tmp * decimal_tab(i++);
		tmp = (*s)[i] - '0';
	}
	*s += i;
	return (res);
}

float	ft_atof(const char **s)
{
	float				res;
	uint8_t				tmp;
	uint8_t				neg;

	while (**s == ' ' || **s == '\t')
		++(*s);
	neg = (**s == '-');
	if (neg)
		++(*s);
	res = 0.0;
	tmp = **s - '0';
	while (tmp < 10)
	{
		res = res * 10.0f + tmp;
		tmp = *(++(*s)) - '0';
	}
	*s += (**s == '.');
	res += decimal_part(s);
	if (neg)
		res *= -1.0f;
	return (res);
}

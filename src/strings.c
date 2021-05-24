/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strings.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 12:59:13 by abaudot           #+#    #+#             */
/*   Updated: 2021/05/23 20:11:02 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

uint8_t	update_string(t_string *str, const char *src, int32_t n)
{
	char			*tmp;
	const uint32_t	wanted = n + str->len;

	if (str->size < wanted)
	{
		while (str->size < wanted)
			str->size <<= 1;
		tmp = str->s;
		str->s = (char *)malloc(str->size + 1);
		if (!str->s)
		{
			free(tmp);
			return (0);
		}
		ft_memcpy(str->s, tmp, str->len);
		free(tmp);
	}
	ft_memcpy(str->s + str->len, src, n);
	str->len = wanted;
	str->s[str->len] = 0;
	return (1);
}

uint8_t	initilize_string(t_string *string, uint32_t n)
{
	string->s = (char *)malloc(n);
	if (!string->s)
		return (0);
	*string->s = 0;
	string->len = 0;
	string->size = n - 1;
	return (1);
}

static inline void	wordcopy(uint64_t dstp, uint64_t srcp, uint32_t len)
{
	register uint64_t	a0;
	register uint64_t	a1;

	if (len & 1)
	{
		((uint64_t *)dstp)[0] = ((uint64_t *)srcp)[0];
		if (len == 1)
			return ;
		srcp += 8;
		dstp += 8;
		--len;
	}
	while (len)
	{
		a0 = ((uint64_t *)srcp)[0];
		a1 = ((uint64_t *)srcp)[1];
		((uint64_t *)dstp)[0] = a0;
		((uint64_t *)dstp)[1] = a1;
		srcp += 16;
		dstp += 16;
		len -= 2;
	}
}

void	ft_memcpy(void *dest, const void *src, uint32_t n)
{
	uint64_t	dstp;
	uint64_t	srcp;

	dstp = (uint64_t)dest;
	srcp = (uint64_t)src;
	if (n > 15)
	{
		wordcopy(dstp, srcp, n >> 3);
		srcp += n & -8;
		dstp += n & -8;
		n &= 7;
	}
	while (n--)
		*(uint8_t *)dstp++ = *(uint8_t *)srcp++;
}

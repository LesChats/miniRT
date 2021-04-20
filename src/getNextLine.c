/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getNextLine.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <abaudot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/08 16:37:13 by abaudot           #+#    #+#             */
/*   Updated: 2021/04/19 10:30:10 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static int32_t		get_nl(const char *s, uint32_t n)
{
	uint64_t	long_word;
	uint32_t	i;

	i = 0;
	while (i + 0xf < n)
	{
		long_word = *(uint64_t *)(s + i) ^ NL;
		if ((long_word - LOMAGIC) & ~long_word & HIMAGIC)
			break ;
		i += 8;
	}
	while (i < n)
	{
		if (s[i] == '\n')
			return (i);
		++i;
	}
	return (-1);
}

static int32_t		read_line(int32_t fd, t_string *str, char *sheet, 
		int32_t *sheet_len)
{
	int32_t	a_nl;

	while ((*sheet_len = read(fd, sheet, READ_BUFFSZ)) > 0)
	{
		sheet[*sheet_len] = 0;
		if ((a_nl = get_nl(sheet, *sheet_len)) != -1)
		{
			if (!(update_string(str, sheet, a_nl)))
				return (-1);
			*sheet_len -= a_nl;
			ft_memcpy(sheet, sheet + a_nl + 1, *sheet_len);
			return (1);
		}
		if (!(update_string(str, sheet, *sheet_len)))
			return (-1);
	}
//	str->s = 0;
	*sheet = 0;
	return (-(*sheet_len == -1));
}

int32_t				getNextLine(int32_t fd, t_string *my_line)
{
	static char		sheets[READ_BUFFSZ + 1];
	static int32_t	sheet_len;
	int32_t			a_nl;

	if (sheets[0] == 0)
		return (read_line(fd, my_line, sheets, &sheet_len));
	--sheet_len;
	if ((a_nl = get_nl(sheets, sheet_len)) != -1)
	{
		if (!(update_string(my_line, sheets, a_nl)))
			return (-1);
		ft_memcpy(sheets, sheets + a_nl + 1, sheet_len - a_nl);
		return (1);
	}
	if (!(update_string(my_line, sheets, sheet_len)))
		return (-1);
	return (read_line(fd, my_line, sheets, &sheet_len));
}

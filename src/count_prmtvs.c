/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_prmtvs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <abaudot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 10:02:32 by abaudot           #+#    #+#             */
/*   Updated: 2021/04/21 11:35:00 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static inline uint8_t	update(uint32_t *prmtv_arr, t_string *track,
		uint32_t i)
{
	const char c = '0' + i;

	++prmtv_arr[i];
	return (update_string(track, &c, 1));
}

static uint8_t			check_compound(const char a, const char b,
		uint32_t *prmtv_arr, t_string *track)
{
	if ((a == 'p') & (b == 'y'))
	{
		++prmtv_arr[SQ];
		prmtv_arr[T] += 4;
		return (update_string(track, "8", 1));
	}
	if ((a == 'c') & (b == 'b'))
	{
		prmtv_arr[SQ] += 6;
		return (update_string(track, "9", 1));
	}
	else
		return (return_message("primitive not reconnized !"));
}

static uint8_t			add_prmtv(const char a, const char b,
		uint32_t *prmtv_arr, t_string *track)
{
	if (a == 'A')
		return (update_string(track, ":", 1));
	if (a == 'R')
		return (update_string(track, ";", 1));
	if ((a == 's') & (b == 'p'))
		return (update(prmtv_arr, track, SP));
	if ((a == 'p') & (b == 'l'))
		return (update(prmtv_arr, track, P));
	if ((a == 's') & (b == 'q'))
		return (update(prmtv_arr, track, SQ));
	if ((a == 'c') & (b == 'y'))
		return (update(prmtv_arr, track, CY));
	if ((a == 't') & ((b == 'r')))
		return (update(prmtv_arr, track, T));
	if ((a == 'c') & ((b == ' ') | (b == '\t')))
		return (update(prmtv_arr, track, C));
	if ((a == 'c') & (b == 'p'))
		return (update(prmtv_arr, track, CP));
	if ((a == 'l') & ((b == ' ') | (b == '\t')))
		return (update(prmtv_arr, track, L));
	else
		return (check_compound(a, b, prmtv_arr, track));
	return (1);
}

uint8_t					count_prmtvs(uint32_t *prmtvs_arr, t_string *tracker,
		const int fd)
{
	t_string	s;

	if (!initilize_string(&s, 64))
		return (return_message("string init error: file count_prmtv.c"));
	if (!initilize_string(tracker, 64))
		return (return_message("string init error: file count_prmtv.c"));
	while (getnextline(fd, &s) > 0)
	{
		s.len = 0;
		if ((!*s.s) | (*s.s == '#'))
			continue ;
		if (!add_prmtv(s.s[0], s.s[1], prmtvs_arr, tracker))
		{
			free(s.s);
			free(tracker->s);
			return (return_message("\texpect: A,R,sp,pl,sq,cy,tr,py,c,l"));
		}
	}
	free(s.s);
	return (1);
}

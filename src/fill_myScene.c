/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_myScene.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 12:20:17 by abaudot           #+#    #+#             */
/*   Updated: 2021/04/18 15:34:32 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser.h"

static uint8_t	add_prmtv(struct s_preScene *ps, const char *s,
		const uint32_t index, uint32_t *pos_num)
{
	//change order
	static const t_parser prmtvs_parser[] =
	{
		sphrs_parser, pln_parser,
		sqr_parser, cyl_parser,
	   	trgl_parser, cps_parser,
	   	cams_parser, lght_parser,
		pyr_parser, cub_parser,
		ambnt_parser, rsltn_parser
	};

	return (prmtvs_parser[ps->tracker.s[index] - '0'](ps, s + 2, pos_num));
}

uint8_t fill_myScene(struct s_preScene *ps, const int fd)
{
	t_string	s;
	uint32_t	i;
	uint32_t	pos_num[2];

	i = 0;
	*pos_num = 0;
	pos_num[1] = 0;
	if (!initilize_string(&s, 64))
		return (return_message("string init error: file fill_myScene.c"));
	while (getNextLine(fd, &s) > 0)
	{
		s.len = 0;
		if ((!*s.s) | (*s.s == '#'))
			continue ;
		if (!add_prmtv(ps, s.s, i++, pos_num))
		{
			free(s.s);
			return (return_message("in file fill_myScene.c"));
		}
	}
	free(s.s);
	return (1);
}

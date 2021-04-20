/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <abaudot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/14 14:12:27 by abaudot           #+#    #+#             */
/*   Updated: 2021/04/20 15:30:03 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "struct.h"
# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <mlx.h>

# include <stdio.h>

/*
**	Magic number for fast read
*/
# define HIMAGIC    0x8080808080808080UL
# define LOMAGIC    0x0101010101010101UL
# define NL         0xa0a0a0a0a0a0a0aUL

/*
**	Precomp
*/

# define READ_BUFFSZ	10000
# define COLOR_INV		0.00392156862
# define INVSQ2			0.70710678118

/*
**	Number of primitiv we handel
*/
# define PRMTVNB	6
# define SP	0
# define P	1
# define SQ	2
# define CY	3
# define T	4
# define CP	5
# define C	6
# define L  7
# define PY 8
# define CB 9

/*
**	----- SPECIFIQ Struct, only used in the parsing phase -------
**
**	gathere de data need to make the final scene
**		save primitvs + cameras + lights
*/

typedef struct	s_string
{
	char		*s;
	uint32_t	len;
	uint32_t	size;
}				t_string;

struct					s_preScene
{
	t_string			tracker;
	struct s_cams		cams;
	struct s_lghts		lghts;
	struct s_prmtvs		prmtvs;
	void				*prmtvs_data;	
	void				*mlx;
	t_material			*mtrls_data;
	uint32_t			rsltn[2];
	uint32_t			total_size;
};

/*
**	Utils strings function
*/
uint8_t		initilize_string(t_string *string, uint32_t n);
void		ft_memcpy(void *dest, const void *src, uint32_t n);
uint8_t		update_string(t_string *str, const char *src, int32_t n);
int32_t		ft_atoi(const char **s);
float		ft_atof(const char **s);
uint8_t		vect_parse(t_vec3f v, const char **s);

int32_t		getNextLine(int32_t fd, t_string *my_line);
int32_t		find_nl(const char *s, const uint32_t n);

uint32_t	ft_strlen(const char *s);
int			ft_strncmp(const char *s1, const char *s2, int n);

/*
** main parsing functions	
*/
uint8_t		count_prmtvs(uint32_t *prmtvs_arr, t_string *tracker,
		const int fd);
uint8_t		fill_myScene(struct s_preScene *ps,
		const int fd);
uint8_t		finalScene(struct s_preScene *ps,
		struct s_scene *const scene);
uint8_t		parse(const char *rt_file, struct s_scene *const scene,
		struct s_mlx *mlx);

/*
**	Ad-oc Parsing function
*/
typedef uint8_t	(*t_parser)(struct s_preScene*, const char *,
	   	uint32_t*);

uint8_t		sphrs_parser(struct s_preScene *ps, const char *s,
	  	uint32_t *pos_num);
uint8_t		pln_parser(struct s_preScene *ps, const char *s,
	   	uint32_t *pos_num);
uint8_t		sqr_parser(struct s_preScene *ps, const char *s,
	   	uint32_t *pos_num);
uint8_t		cyl_parser(struct s_preScene *ps, const char *s,
	   	uint32_t *pos_num);
uint8_t		trgl_parser(struct s_preScene *ps, const char *s,
	   	uint32_t *pos_num);
uint8_t		cps_parser(struct s_preScene *ps, const char *s,
		uint32_t *pos_num);
uint8_t		pyr_parser(struct s_preScene *ps, const char *s,
		uint32_t *pos_num);
uint8_t		cub_parser(struct s_preScene *ps, const char *s,
	   	uint32_t *pos_num);
uint8_t		cams_parser(struct s_preScene *ps, const char *s,
	   	uint32_t *pos_num);
uint8_t		lght_parser(struct s_preScene *ps, const char *s,
	   	uint32_t *pos_num);
uint8_t		ambnt_parser(struct s_preScene *ps, const char *s,
	   	uint32_t *pos_num);
uint8_t		rsltn_parser(struct s_preScene *ps, const char *s,
	   	uint32_t *pos_num);

uint8_t		parse_mat(t_material *m, const char **s, void *mlx);

uint8_t		sceneToBmp(const struct s_scene *s, const t_cam * cam);
void		grabTexture(void * mlx, struct s_texture *t, const char **s);
uint8_t		setSkybox(struct s_skybox *sky,  char *filename, void *mlx);

uint8_t		return_message(const char *mess);
void		return_messageOnly(const char *mess);
#endif

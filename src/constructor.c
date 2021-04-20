/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constructor.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aime <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/11 16:56:56 by aime              #+#    #+#             */
/*   Updated: 2021/04/19 11:40:29 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "struct.h"
#include "bvh.h"
#include "stdlib.h"

#include <stdio.h>
static uint8_t	resize_nodes(t_node **node, const uint32_t node_count)
{
	uint32_t		i;
	t_node *const	tmp = *node;

	if (!(*node = (t_node *)malloc(sizeof(t_node) * node_count)))
	{
		free(tmp);
		return (0);
	}
	i = 0;
	while (i < node_count)
	{
//		printf("node cnt/start/off %d/%d/%d\n", tmp[i].prmtv_cnt,tmp[i].start,tmp[i].offset);
		(*node)[i] = tmp[i];
		++i;
	}
	free(tmp);
	return (1);
}

static inline void swapp(t_prmtv *a, t_prmtv *b)
{
//	printf("coucou ici on swap\n");
	const t_prmtv tmp = *a;

	*a = *b;
	*b = tmp;
}

static inline void converter(const t_prmtv *p, t_box *bbox)
{
	static const t_bounder bounder[] = 
	{
		sphr_bounding,
		pln_bounding,
		sqr_bounding,
		cyl_bounding,
		trgl_bounding,
		cps_bounding
	};
	bounder[p->type](p->prmtv, bbox);
};

static inline void set_node(t_node *node, const t_prmtv *p,
		struct s_buildStak *todo, t_box *bc)
{
	t_box			bb;
	t_box			tmp_box;
	t_vec3f			tmp;
	const uint32_t	end = todo->stack[--todo->ptr].end;
	uint32_t		i;

	node->start = todo->stack[todo->ptr].start;
	node->prmtv_cnt = end - node->start;
	node->offset = UNTOUCH;
	converter((p + node->start), &bb);
	getCenter(&bb, tmp);
	equal_(bc->min, tmp);
	equal_(bc->max, tmp);
	i = node->start + 1;
	while (i < end)
	{
		converter((p + i++), &tmp_box);
		expandToInclude(&tmp_box, &bb);
		getCenter(&bb, tmp);
		expandToInclude_vec(tmp, bc);
	}
	node->bbox = bb;
	if (node->prmtv_cnt < LEAF)
		node->offset = 0;
}

static inline void add_totodo(t_prmtv *pv, struct s_buildStak *todo,
		t_box *bc, const uint32_t count)
{
	const uint32_t	split_dim = maxDimension(bc);
	const uint32_t	end = todo->stack[todo->ptr].end;
	const float		split = .5f * (bc->min[split_dim] + bc->max[split_dim]);
	uint32_t		mid;
	uint32_t		start;

	mid = todo->stack[todo->ptr].start;
	start = mid;
	while (start < end)
	{
		converter((pv + start), bc);
		getCenter(bc, bc->min);
		if (bc->min[split_dim] < split)
			swapp(pv + start, pv + mid++);
		++start;
	}
	start = todo->stack[todo->ptr].start;
	if (mid == start || mid == end)
		mid = start + (end - start) / 2;
//	printf("start/mid/end %d/%d/%d\n", start, mid, end);
	todo->stack[todo->ptr++] = (struct s_buildEntry){count - 1, mid, end};
	todo->stack[todo->ptr++] = (struct s_buildEntry){count - 1, start, mid};
}

uint8_t builder(struct s_bvh *bvh)
{
	struct s_buildStak	todo;
	t_box				bc;
	uint32_t			node_count;

	if (!(bvh->nodes = malloc(sizeof(t_node) * (bvh->prmtvs.size * 2))))
		return (0);
	todo.stack[0] = (struct s_buildEntry){0xfffffffc, 0, bvh->prmtvs.size};
	todo.ptr = 1;
	node_count = 0;
	while (todo.ptr > 0)
	{
		set_node(&bvh->nodes[node_count], bvh->prmtvs.prmtvs, &todo, &bc);
		if (todo.stack[todo.ptr].parent != 0xfffffffc)
		{
			if (--bvh->nodes[todo.stack[todo.ptr].parent].offset == TOUCH)
				bvh->nodes[todo.stack[todo.ptr].parent].offset =
					node_count - todo.stack[todo.ptr].parent;
		}
		if (bvh->nodes[node_count++].offset == 0)
			continue ;
		add_totodo(bvh->prmtvs.prmtvs, &todo, &bc, node_count);
	}
	return (resize_nodes(&bvh->nodes, node_count));
}

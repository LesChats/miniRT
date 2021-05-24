/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constructor.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <abaudot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 12:48:06 by abaudot           #+#    #+#             */
/*   Updated: 2021/05/23 20:03:17 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "struct.h"
#include "bvh.h"
#include "stdlib.h"

static uint8_t	resize_nodes(t_node **node, const uint32_t node_count)
{
	uint32_t		i;
	t_node *const	tmp = *node;

	*node = (t_node *)malloc(sizeof(t_node) * node_count);
	if (!*node)
	{
		free(tmp);
		return (0);
	}
	i = 0;
	while (i < node_count)
	{
		(*node)[i] = tmp[i];
		++i;
	}
	free(tmp);
	return (1);
}

static inline void	converter(const t_prmtv *p, t_box *bbox)
{
	static const t_bounder	bounder[] = {
		sphr_bounding,
		pln_bounding,
		sqr_bounding,
		cyl_bounding,
		trgl_bounding,
		cps_bounding
	};

	bounder[p->type](p->prmtv, bbox);
}

static inline void	set_node(t_node *node, const t_prmtv *p,
		struct s_buildstak *todo, t_box *bc)
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
	getcenter(&bb, tmp);
	equal_(bc->min, tmp);
	equal_(bc->max, tmp);
	i = node->start + 1;
	while (i < end)
	{
		converter((p + i++), &tmp_box);
		expandtoinclude(&tmp_box, &bb);
		getcenter(&bb, tmp);
		expandtoinclude_vec(tmp, bc);
	}
	node->bbox = bb;
	if (node->prmtv_cnt < LEAF)
		node->offset = 0;
}

static inline void	add_totodo(t_prmtv *pv, struct s_buildstak *todo,
		t_box *bc, const uint32_t count)
{
	const uint32_t	split_dim = maxdimension(bc);
	const uint32_t	end = todo->stack[todo->ptr].end;
	const float		split = 0.5f * (bc->min[split_dim] + bc->max[split_dim]);
	uint32_t		mid;
	uint32_t		start;

	mid = todo->stack[todo->ptr].start;
	start = mid;
	while (start < end)
	{
		converter((pv + start), bc);
		getcenter(bc, bc->min);
		if (bc->min[split_dim] < split)
			swapp(pv + start, pv + mid++);
		++start;
	}
	start = todo->stack[todo->ptr].start;
	if (mid == start || mid == end)
		mid = start + (end - start) / 2;
	todo->stack[todo->ptr++] = (struct s_buildentry){count - 1, mid, end};
	todo->stack[todo->ptr++] = (struct s_buildentry){count - 1, start, mid};
}

uint8_t	builder(struct s_bvh *bvh)
{
	struct s_buildstak	todo;
	t_box				bc;
	uint32_t			node_count;

	bvh->nodes = malloc(sizeof(t_node) * (bvh->prmtvs.size * 2));
	if (!bvh->nodes)
		return (0);
	todo.stack[0] = (struct s_buildentry){0xfffffffc, 0, bvh->prmtvs.size};
	todo.ptr = 1;
	node_count = 0;
	while (todo.ptr > 0)
	{
		set_node(&bvh->nodes[node_count], bvh->prmtvs.prmtvs, &todo, &bc);
		if (todo.stack[todo.ptr].parent != 0xfffffffc)
		{
			if (--bvh->nodes[todo.stack[todo.ptr].parent].offset == TOUCH)
				bvh->nodes[todo.stack[todo.ptr].parent].offset
					= node_count - todo.stack[todo.ptr].parent;
		}
		if (bvh->nodes[node_count++].offset == 0)
			continue ;
		add_totodo(bvh->prmtvs.prmtvs, &todo, &bc, node_count);
	}
	return (resize_nodes(&bvh->nodes, node_count));
}

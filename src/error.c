/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/01 16:21:55 by abaudot           #+#    #+#             */
/*   Updated: 2021/04/21 12:29:48 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdint.h>

void	return_messageonly(const char *mess)
{
	printf("error:\t%s\n", mess);
	return ;
}

uint8_t	return_message(const char *mess)
{
	printf("error:\t%s\n", mess);
	return (0);
}

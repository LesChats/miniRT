/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/01 16:21:55 by abaudot           #+#    #+#             */
/*   Updated: 2021/04/13 18:05:07 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdint.h>

void return_messageOnly(const char *mess)
{
	printf("error:\t%s\n", mess);
	return ;
}

uint8_t return_message(const char *mess)
{
	printf("error:\t%s\n", mess);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool_printer.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaindro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/01 17:31:43 by amaindro          #+#    #+#             */
/*   Updated: 2017/10/01 17:32:11 by amaindro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "otool.h"

uint64_t	ft_len(uint64_t n, int *width)
{
	uint64_t	i;

	i = 1;
	*width = 0;
	if (n == 0)
		return (16);
	while (n != 0)
	{
		n /= 16;
		i *= 16;
		*width += 1;
	}
	return (i);
}

void		ft_putaddr(uint64_t addr, int offset)
{
	uint64_t	len[2];
	int			width;

	len[0] = ft_len(addr, &width);
	while (offset)
	{
		if (addr)
		{
			if (offset > width)
				ft_putchar('0');
			else
			{
				len[1] = (addr / (len[0] / 16)) % 16;
				ft_putchar((len[1] < 10) ? len[1] + '0' : len[1] - 10 + 'a');
				len[0] /= 16;
			}
		}
		else
		{
			if (offset > width)
				ft_putchar('0');
		}
		offset--;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printer.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaindro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/18 17:56:11 by amaindro          #+#    #+#             */
/*   Updated: 2017/10/04 17:34:15 by amaindro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

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

void		symprinter(uint64_t addr, int offset, char c, char *sym_name)
{
	uint64_t	len[2];
	int			width;

	len[0] = ft_len(addr, &width);
	while (offset)
	{
		if (c != 'U')
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
			ft_putchar(' ');
		offset--;
	}
	ft_putchar(' ');
	ft_putchar(c);
	ft_putchar(' ');
	ft_putendl(sym_name);
}

void		printer(t_sym **sym_array, size_t size)
{
	while (sym_array[size])
	{
		if (sym_array[size]->c != '-')
		{
			if (sym_array[size]->addr)
			{
				symprinter(sym_array[size]->addr, sym_array[size]->padding,
						sym_array[size]->c, sym_array[size]->sym_name);
			}
			else
			{
				symprinter(sym_array[size]->addr, sym_array[size]->padding,
						sym_array[size]->c, sym_array[size]->sym_name);
			}
		}
		size++;
	}
}

int			comparer(const void *s1, const void *s2)
{
	int		ret;
	t_sym	*sym1;
	t_sym	*sym2;

	sym1 = *(t_sym**)s1;
	sym2 = *(t_sym**)s2;
	if ((ret = ft_strcmp(sym1->sym_name, sym2->sym_name)) == 0)
	{
		if (sym1->addr > sym2->addr)
			ret = 1;
		else if (sym1->addr == sym2->addr)
			ret = 0;
		else
			ret = -1;
	}
	return (ret);
}

void		sorter(t_sym *list)
{
	t_sym	**sym_array;
	t_sym	*tmp;
	size_t	size;

	size = 0;
	tmp = list;
	while (list)
	{
		list = list->next;
		size++;
	}
	sym_array = ft_memalloc(sizeof(t_sym*) * (size + 1));
	list = tmp;
	size = 0;
	while (list)
	{
		sym_array[size] = list;
		size++;
		list = list->next;
	}
	ft_qsort(sym_array, size, sizeof(t_sym*), comparer);
	size = 0;
	printer(sym_array, size);
}

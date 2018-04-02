/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool_32.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaindro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/01 17:27:38 by amaindro          #+#    #+#             */
/*   Updated: 2017/10/01 18:16:16 by amaindro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "otool.h"

void	sect_printer_32(char *ptr, struct section *sect)
{
	uint64_t		i;
	uint64_t		j;

	i = -1;
	j = 0;
	while (++i < sect->size)
	{
		if (i % 16 == 0)
		{
			ft_putaddr(i + sect->addr, 8);
			ft_putchar('\t');
		}
		ft_putaddr(*(unsigned char*)(ptr + i + sect->offset), 2);
		ft_putchar(' ');
		if (i % 16 == 15)
			ft_putchar('\n');
	}
	ft_putchar('\n');
}

void	get_sect_32(struct load_command *lc, char *ptr, void *file_end)
{
	int							n;
	int							i;
	char						**sect_array;
	struct segment_command		*seg;
	struct section				*sect;

	seg = (struct segment_command*)lc;
	sect = (void*)seg + sizeof(struct segment_command);
	if ((void*)sect + sizeof(struct section) >= file_end)
		return ;
	if (seg->nsects)
	{
		n = 0;
		while (n < (int)seg->nsects)
		{
			if (ft_strcmp(sect->sectname, SECT_TEXT) == 0)
			{
				sect_printer_32(ptr, sect);
			}
			if ((void*)sect + sizeof(struct section) >= file_end)
				return ;
			sect = (void*)sect + sizeof(struct section);
			n++;
		}
	}
}

void	otool_32(char *ptr, void *file_end)
{
	int							i;
	struct mach_header			*header;
	struct load_command			*lc;

	header = (struct mach_header *)ptr;
	i = -1;
	lc = (void *)(ptr + sizeof(struct mach_header));
	while (++i < (int)header->ncmds)
	{
		if (lc->cmd == LC_SEGMENT)
		{
			get_sect_32(lc, ptr, file_end);
		}
		if ((void*)lc + lc->cmdsize >= file_end)
			return ;
		lc = (void*)lc + lc->cmdsize;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool_64_reverse.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaindro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/01 17:36:16 by amaindro          #+#    #+#             */
/*   Updated: 2017/10/01 22:17:42 by amaindro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "otool.h"

void	sect_printer_64_reverse(char *ptr, struct section_64 *sect)
{
	uint64_t		i;
	uint64_t		j;

	i = -1;
	j = 0;
	while (++i < sect->size)
	{
		if (i % 16 == 0)
		{
			ft_putaddr(i + sect->addr, 16);
			ft_putchar('\t');
		}
		ft_putaddr(*(unsigned char*)(ptr + i + sect->offset), 2);
		ft_putchar(' ');
		if (i % 16 == 15)
			ft_putchar('\n');
	}
	ft_putchar('\n');
}

void	get_sect_64_reverse(struct load_command *lc, char *ptr, void *file_end)
{
	int							n;
	int							i;
	char						**sect_array;
	struct segment_command_64	*seg;
	struct section_64			*sect;

	swap_segment_command_64(seg = (void*)lc, 0);
	swap_section_64(sect = (void*)seg + sizeof(struct segment_command_64),
			seg->nsects, 0);
	if ((void*)sect + sizeof(struct section_64) >= file_end)
		return ;
	if (seg->nsects)
	{
		n = 0;
		while (n < (int)seg->nsects)
		{
			if (ft_strcmp(sect->sectname, SECT_TEXT) == 0)
				sect_printer_64_reverse(ptr, sect);
			if ((void*)sect + sizeof(struct section_64) >= file_end)
				return ;
			sect = (void*)sect + sizeof(struct section_64);
			n++;
		}
	}
}

void	otool_64_reverse(char *ptr, void *file_end)
{
	int							i;
	struct mach_header_64		*header;
	struct load_command			*lc;

	swap_mach_header_64((header = (struct mach_header_64 *)ptr), 0);
	i = -1;
	swap_load_command(lc = (void *)(ptr + sizeof(struct mach_header_64)), 0);
	while (++i < (int)header->ncmds)
	{
		if (lc->cmd == LC_SEGMENT_64)
		{
			get_sect_64_reverse(lc, ptr, file_end);
		}
		if ((void*)lc + lc->cmdsize >= file_end)
			return ;
		swap_load_command(lc = (void*)lc + lc->cmdsize, 0);
	}
}

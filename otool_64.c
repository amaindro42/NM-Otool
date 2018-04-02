/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool_64.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaindro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/29 13:31:47 by amaindro          #+#    #+#             */
/*   Updated: 2017/10/01 18:16:31 by amaindro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "otool.h"

void	sect_printer_64(char *ptr, struct section_64 *sect)
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

void	get_sect_64(struct load_command *lc, char *ptr, void *file_end)
{
	int							n;
	int							i;
	char						**sect_array;
	struct segment_command_64	*seg;
	struct section_64			*sect;

	seg = (struct segment_command_64*)lc;
	sect = (void*)seg + sizeof(struct segment_command_64);
	if ((void*)sect + sizeof(struct section_64) >= file_end)
		return ;
	if (seg->nsects)
	{
		n = 0;
		while (n < (int)seg->nsects)
		{
			if (ft_strcmp(sect->sectname, SECT_TEXT) == 0)
			{
				sect_printer_64(ptr, sect);
			}
			if ((void*)sect + sizeof(struct section_64) >= file_end)
				return ;
			sect = (void*)sect + sizeof(struct section_64);
			n++;
		}
	}
}

void	otool_64(char *ptr, void *file_end)
{
	int							i;
	struct mach_header_64		*header;
	struct load_command			*lc;

	header = (struct mach_header_64 *)ptr;
	i = -1;
	lc = (void *)(ptr + sizeof(struct mach_header_64));
	while (++i < (int)header->ncmds)
	{
		if (lc->cmd == LC_SEGMENT_64)
		{
			get_sect_64(lc, ptr, file_end);
		}
		if ((void*)lc + lc->cmdsize >= file_end)
			return ;
		lc = (void*)lc + lc->cmdsize;
	}
}

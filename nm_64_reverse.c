/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_64_reverse.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaindro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/29 12:44:23 by amaindro          #+#    #+#             */
/*   Updated: 2017/10/01 15:03:26 by amaindro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

void	get_syms_64_reverse(struct load_command *lc, char *ptr,
		char **sect_array, void *file_end)
{
	int						i;
	char					*str;
	struct nlist			*array;
	struct symtab_command	*sym;
	t_sym					*list[2];

	swap_symtab_command(sym = (struct symtab_command *)lc, 0);
	swap_nlist(array = (void*)ptr + sym->symoff, sym->nsyms, 0);
	str = ptr + sym->stroff;
	i = 0;
	if (!(list[0] = get_sym_type_32(str, array[i++], sect_array, file_end)))
		return ;
	list[1] = list[0];
	while (i < (int)sym->nsyms)
	{
		if (!(list[0]->next =
					get_sym_type_32(str, array[i++], sect_array, file_end)))
			return ;
		list[0] = list[0]->next;
	}
	i = 0;
	sorter(list[1]);
	while (sect_array[i])
		free(sect_array[i++]);
	free(sect_array);
}

char	**get_sect_64_reverse(struct load_command *lc, void *file_end)
{
	int							n;
	char						**sect_array;
	struct segment_command_64	*seg;
	struct section_64			*sect;

	sect_array = NULL;
	swap_segment_command_64(seg = (void*)lc, 0);
	swap_section_64(sect = (void*)seg + sizeof(struct segment_command_64),
			seg->nsects, 0);
	if ((void*)sect + sizeof(struct section) >= file_end)
		return (NULL);
	if (seg->nsects)
	{
		sect_array = ft_memalloc(sizeof(char*) * (seg->nsects + 1));
		n = -1;
		while (++n < (int)seg->nsects)
		{
			sect_array[n] = ft_strdup(sect->sectname);
			if ((void*)sect + sizeof(struct section) >= file_end)
				break ;
			sect = (void*)sect + sizeof(struct section);
		}
	}
	swap_segment_command_64(seg, 0);
	return (sect_array);
}

void	nm_64_reverse(char *ptr, void *file_end)
{
	int						i;
	char					**sect_array[3];
	struct mach_header_64	*header;
	struct load_command		*lc;

	swap_mach_header_64((header = (struct mach_header_64 *)ptr), 0);
	i = -1;
	swap_load_command(lc = (void *)(ptr + sizeof(struct mach_header)), 0);
	sect_array[0] = NULL;
	while (++i < (int)header->ncmds)
	{
		if (lc->cmd == LC_SEGMENT)
		{
			sect_array[1] = get_sect_64_reverse(lc, file_end);
			sect_array[2] = merge_array(sect_array[0], sect_array[1]);
			ft_memdel((void*)&sect_array[0]);
			ft_memdel((void*)&sect_array[1]);
			sect_array[0] = sect_array[2];
		}
		if (lc->cmd == LC_SYMTAB)
			get_syms_64_reverse(lc, ptr, sect_array[0], file_end);
		if ((void*)lc + lc->cmdsize >= file_end)
			return ;
		swap_load_command(lc = (void*)lc + lc->cmdsize, 0);
	}
}

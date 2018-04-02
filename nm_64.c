/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_64.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaindro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/11 14:25:33 by amaindro          #+#    #+#             */
/*   Updated: 2017/10/02 17:33:13 by amaindro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

void	free_array(char **sect_array)
{
	int		i;

	i = 0;
	while (sect_array[i])
		free(sect_array[i++]);
	free(sect_array);
}

t_sym	*get_sym_type_64(char *str, struct nlist_64 sym, char **sect_array,
		void *file_end)
{
	char	c;

	c = 0;
	if ((sym.n_type & N_TYPE) == N_ABS)
		c = 'A';
	if ((sym.n_type & N_TYPE) == N_PBUD)
		c = 'C';
	if ((sym.n_type & N_TYPE) == N_INDR)
		c = 'I';
	if ((sym.n_type & N_TYPE) == N_UNDF)
		c = 'U';
	if ((sym.n_type & N_TYPE) == N_SECT)
		c = sect_to_char(sym.n_sect - 1, sect_array);
	if ((sym.n_type & N_EXT) == 0 && c)
		c = ft_tolower(c);
	if (sym.n_type & N_STAB)
		c = '-';
	if ((void*)(str + sym.n_un.n_strx) >= file_end)
		return (NULL);
	return (get_list(c, str + sym.n_un.n_strx, sym.n_value, 16));
}

void	get_syms_64(struct load_command *lc, char *ptr, char **sect_array,
		void *file_end)
{
	int						i;
	char					*str;
	struct nlist_64			*array;
	struct symtab_command	*sym;
	t_sym					*list[2];

	if ((void*)(sym = (struct symtab_command *)lc) >= file_end ||
			(void*)(array = (void*)ptr + sym->symoff) >= file_end ||
			(void*)(str = ptr + sym->stroff) >= file_end)
		return ;
	i = 0;
	if (!(list[0] = get_sym_type_64(str, array[i++], sect_array, file_end)))
		return ;
	list[1] = list[0];
	while (i < (int)sym->nsyms)
	{
		if (!(list[0]->next = get_sym_type_64(str, array[i++], sect_array,
						file_end)))
			return ;
		list[0] = list[0]->next;
	}
	sorter(list[1]);
	free_array(sect_array);
}

char	**get_sect_64(struct load_command *lc, void *file_end)
{
	int							n;
	char						**sect_array;
	struct segment_command_64	*seg;
	struct section_64			*sect;

	seg = (struct segment_command_64*)lc;
	sect = (void*)seg + sizeof(struct segment_command_64);
	if ((void*)sect + sizeof(struct section_64) >= file_end)
		return (NULL);
	if (seg->nsects)
	{
		if (!(sect_array = ft_memalloc(sizeof(char*) * (seg->nsects + 1))))
			return (NULL);
		n = 0;
		while (n < (int)seg->nsects)
		{
			sect_array[n] = ft_strdup(sect->sectname);
			if ((void*)sect + sizeof(struct section_64) >= file_end)
				return (NULL);
			sect = (void*)sect + sizeof(struct section_64);
			n++;
		}
		return (sect_array);
	}
	return (NULL);
}

void	nm_64(char *ptr, void *file_end)
{
	int							i;
	char						**sect_array[3];
	struct mach_header_64		*header;
	struct load_command			*lc;

	header = (struct mach_header_64 *)ptr;
	i = -1;
	lc = (void *)(ptr + sizeof(struct mach_header_64));
	sect_array[0] = NULL;
	while (++i < (int)header->ncmds)
	{
		if (lc->cmd == LC_SEGMENT_64)
		{
			sect_array[1] = get_sect_64(lc, file_end);
			sect_array[2] = merge_array(sect_array[0], sect_array[1]);
			ft_memdel((void*)&sect_array[0]);
			ft_memdel((void*)&sect_array[1]);
			sect_array[0] = sect_array[2];
		}
		if (lc->cmd == LC_SYMTAB)
			get_syms_64(lc, ptr, sect_array[0], file_end);
		if ((void*)lc + lc->cmdsize >= file_end)
			return ;
		lc = (void*)lc + lc->cmdsize;
	}
}

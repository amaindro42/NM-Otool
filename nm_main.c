/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaindro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/25 10:38:06 by amaindro          #+#    #+#             */
/*   Updated: 2017/10/05 15:22:12 by amaindro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

/*
**	A = sym type N_ABS
**	C = sym type N_PBUD
**	I = sym type N_INDR
**	U = sym type N_UNDF
**	B = __bss
**	D = __data
**	T = __text
**	S = other
*/

static t_func	g_func[] =
{
	{MH_MAGIC_64, nm_64},
	{MH_CIGAM_64, nm_64_reverse},
	{MH_MAGIC, nm_32},
	{MH_CIGAM, nm_32_reverse},
	{-1, NULL},
};

static t_fat_func	g_fat_func[] =
{
	{FAT_MAGIC, handle_fat},
	{FAT_CIGAM, handle_fat_reverse},
	{-1, NULL},
};

t_sym	*get_list(char c, char *sym_name, uint64_t n_value, int padding)
{
	t_sym	*sym;

	if (!(sym = ft_memalloc(sizeof(t_sym))))
		return (NULL);
	sym->sym_name = sym_name;
	sym->c = c;
	sym->addr = n_value;
	sym->padding = padding;
	return (sym);
}

char	sect_to_char(int sect, char **sect_array)
{
	if (ft_strcmp(sect_array[sect], SECT_BSS) == 0)
		return ('B');
	if (ft_strcmp(sect_array[sect], SECT_DATA) == 0)
		return ('D');
	if (ft_strcmp(sect_array[sect], SECT_TEXT) == 0)
		return ('T');
	return ('S');
}

char	**merge_array(char **tab1, char **tab2)
{
	size_t	i;
	size_t	size1;
	size_t	size2;
	char	**tab3;

	size1 = 0;
	size2 = 0;
	if (tab1)
		while (tab1[size1])
			size1++;
	if (tab2)
		while (tab2[size2])
			size2++;
	if (!tab1 && !tab2)
		return (NULL);
	if (!(tab3 = ft_memalloc(sizeof(char*) * (size1 + size2 + 1))))
		return (NULL);
	i = -1;
	while (++i < size1)
		tab3[i] = tab1[i];
	i = -1;
	while (++i < size2)
		tab3[i + size1] = tab2[i];
	return (tab3);
}

void	magic_number(char *ptr, void *file_end, char *file_name)
{
	int		magic_number;
	int		i;

	magic_number = *(int *)ptr;
	i = -1;
	while (g_func[++i].magic != -1)
	{
		if (g_func[i].magic == magic_number)
			g_func[i].f(ptr, file_end);
		if (g_fat_func[i].magic == magic_number)
			g_fat_func[i].f(ptr, file_end, file_name);
	}
}

int		main(int ac, char **av)
{
	int				fd;
	int				i;
	char			*ptr;
	struct stat		buf;

	if (ac < 2)
		return (0);
	i = 1;
	while (i < ac)
	{
		if (ac > 2)
			multi_arg(av[i], "\n", ":\n");
		if ((fd = open(av[i], O_RDONLY)) < 0)
			return (0);
		if (fstat(fd, &buf) < 0)
			return (0);
		if ((ptr = mmap(0, buf.st_size, PROT_READ | PROT_WRITE,
						MAP_PRIVATE, fd, 0)) == MAP_FAILED)
			return (0);
		if (close(fd) < 0)
			return (0);
		magic_number(ptr, ptr + buf.st_size, av[i++]);
		if (munmap(ptr, buf.st_size) < 0)
			return (0);
	}
}

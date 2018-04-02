/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaindro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/25 10:37:48 by amaindro          #+#    #+#             */
/*   Updated: 2017/10/05 15:47:10 by amaindro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "otool.h"

static t_func	g_func[] =
{
	{MH_MAGIC_64, otool_64},
	{MH_CIGAM_64, otool_64_reverse},
	{MH_MAGIC, otool_32},
	{MH_CIGAM, otool_32_reverse},
	{-1, NULL},
};

static t_fat_func	g_fat_func[] =
{
	{FAT_MAGIC, handle_fat},
	{FAT_CIGAM, handle_fat_reverse},
	{-1, NULL},
};

void	magic_number(char *ptr, void *file_end, char *file_name)
{
	int		magic_number;
	int		i;

	magic_number = *(int *)ptr;
	i = -1;
	while (g_func[++i].magic != -1)
	{
		if (g_func[i].magic == magic_number)
		{
			ft_putstr("Contents of (__TEXT,__text) section\n");
			g_func[i].f(ptr, file_end);
		}
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
		multi_arg(av[i], "", ":\n");
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

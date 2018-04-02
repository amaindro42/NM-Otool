/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaindro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/09 16:20:30 by amaindro          #+#    #+#             */
/*   Updated: 2017/10/05 14:52:48 by amaindro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

static t_cpu_name	g_cpu_name[] =
{
	{CPU_TYPE_X86, " (for architecture i386):\n"},
	{CPU_TYPE_X86_64, " (for architecture x86_64):\n"},
	{CPU_TYPE_POWERPC, " (for architecture ppc):\n"},
	{CPU_TYPE_POWERPC64, " (for architecture pp64) :\n"},
	{-1, ""}
};

void		multi_arg(char *str, char *str2, char *str3)
{
	ft_putstr(str2);
	ft_putstr(str);
	ft_putstr(str3);
}

void		fat_printer(char *file_name, struct fat_arch *arch)
{
	int		i;

	i = -1;
	while (g_cpu_name[++i].cputype != -1)
	{
		if (g_cpu_name[i].cputype == arch->cputype)
		{
			ft_putchar('\n');
			ft_putstr(file_name);
			ft_putstr(g_cpu_name[i].cpu_name);
		}
	}
}

void		handle_fat(char *ptr, void *file_end, char *file_name)
{
	struct fat_header	*header;
	struct fat_arch		*arch;
	int					num;
	int					i;

	header = (struct fat_header *)ptr;
	num = header->nfat_arch;
	i = -1;
	while (++i < num)
	{
		arch = (void*)(ptr + sizeof(struct fat_header) +
				i * sizeof(struct fat_arch));
		if (arch->cputype == CPU_TYPE &&
				(arch->cpusubtype & CPU_SUBTYPE_MASK) == CPU_SUBTYPE)
			magic_number(ptr + arch->offset, file_end, file_name);
	}
	i = -1;
	while (++i < (int)header->nfat_arch)
	{
		arch = (void*)(ptr + sizeof(struct fat_header) +
				i * sizeof(struct fat_arch));
		fat_printer(file_name, arch);
		magic_number(ptr + arch->offset, file_end, file_name);
	}
}

void		handle_fat_reverse(char *ptr, void *file_end, char *file_name)
{
	struct fat_header	*header;
	struct fat_arch		*arch;
	int					i;

	swap_fat_header((header = (struct fat_header *)ptr), 0);
	i = -1;
	swap_fat_arch(((void*)(ptr + sizeof(struct fat_header))),
			header->nfat_arch, 0);
	while (++i < (int)header->nfat_arch)
	{
		arch = (void*)(ptr + sizeof(struct fat_header) +
				i * sizeof(struct fat_arch));
		if (arch->cputype == CPU_TYPE)
			return (magic_number(ptr + arch->offset, file_end, file_name));
	}
	i = -1;
	while (++i < (int)header->nfat_arch)
	{
		arch = (void*)(ptr + sizeof(struct fat_header) +
				i * sizeof(struct fat_arch));
		fat_printer(file_name, arch);
		magic_number(ptr + arch->offset, file_end, file_name);
	}
}

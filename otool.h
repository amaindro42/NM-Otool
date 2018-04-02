/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaindro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/25 11:35:11 by amaindro          #+#    #+#             */
/*   Updated: 2017/10/05 15:33:28 by amaindro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OTOOL_H
# define OTOOL_H

# include <sys/mman.h>
# include <sys/stat.h>
# include <mach-o/loader.h>
# include <mach-o/fat.h>
# include <mach-o/nlist.h>
# include <mach-o/swap.h>
# include <fcntl.h>
# include <stdlib.h>
# include <unistd.h>
# include "libft/libft.h"

# ifdef __x86_64__
#  define CPU_TYPE CPU_TYPE_X86_64
#  define CPU_SUBTYPE CPU_SUBTYPE_X86_64_ALL
# endif

# ifdef __i386__
#  define CPU_TYPE CPU_TYPE_I386
#  define CPU_SUBTYPE CPU_SUBTYPE_I386_ALL
# endif

typedef struct		s_func
{
	int					magic;
	void				(*f)(char*, void*);
}					t_func;

typedef struct		s_fat_func
{
	int					magic;
	void				(*f)(char*, void*, char*);
}					t_fat_func;

typedef struct		s_cpu_name
{
	cpu_type_t			cputype;
	char				*cpu_name;
}					t_cpu_name;

void				multi_arg(char *str, char *str2, char *str3);
void				ft_putaddr(uint64_t addr, int offset);

void				otool_64(char *ptr, void *file_end);
void				otool_64_reverse(char *ptr, void *file_end);
void				otool_32(char *ptr, void *file_end);
void				otool_32_reverse(char *ptr, void *file_end);
void				handle_fat(char *ptr, void *file_end, char *file_name);
void				handle_fat_reverse(char *ptr, void *file_end,
		char *file_name);

#endif

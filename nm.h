/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaindro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/29 01:55:48 by amaindro          #+#    #+#             */
/*   Updated: 2017/10/05 14:35:27 by amaindro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NM_H
# define NM_H

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

typedef struct		s_sym
{
	char				*sym_name;
	char				c;
	uint64_t			addr;
	int					padding;
	struct s_sym		*next;
}					t_sym;

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

t_sym				*get_list(char c, char *sym_name, uint64_t n_value,
		int padding);
char				sect_to_char(int sect, char **sect_array);
t_sym				*get_sym_type_32(char *str, struct nlist sym,
		char **sect_array, void *file_end);
char				**merge_array(char **tab1, char **tab2);
void				magic_number(char *ptr, void *file_end, char *file_name);
void				sorter(t_sym *list);
void				multi_arg(char *str, char *str2, char *str3);
void				free_array(char **sect_array);

void				nm_64(char *ptr, void *file_end);
void				nm_64_reverse(char *ptr, void *file_end);
void				nm_32(char *ptr, void *file_end);
void				nm_32_reverse(char *ptr, void *file_end);
void				handle_fat(char *ptr, void *file_end, char *file_name);
void				handle_fat_reverse(char *ptr, void *file_end,
		char *file_name);

#endif

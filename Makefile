# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amaindro <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/09/25 10:24:30 by amaindro          #+#    #+#              #
#    Updated: 2017/10/01 17:39:40 by amaindro         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NM = ft_nm

OTOOL = ft_otool

SRC_NM = nm_main.c nm_64.c nm_64_reverse.c nm_32.c nm_32_reverse.c \
		 nm_printer.c fat.c

SRC_OTOOL = otool_main.c fat.c otool_64.c otool_64_reverse.c otool_32.c \
			otool_32_reverse.c otool_printer.c

OBJ_NM = $(SRC_NM:.c=.o)

OBJ_OTOOL = $(SRC_OTOOL:.c=.o)

LIB = -L libft/ -lft

MAKELIB = make -C ./libft

CLEANLIB = make clean -C ./libft

FCLEANLIB = make fclean -C ./libft

all : $(NM)

$(NM) : $(OBJ_NM) $(OBJ_OTOOL)
	@$(MAKELIB)
	@gcc -Wall -Wextra -Werror -c $(SRC_NM)
	@gcc -Wall -Wextra -Werror -o $(NM) $(OBJ_NM) $(LIB)
	@gcc -Wall -Wextra -Werror -o $(OTOOL) $(OBJ_OTOOL) $(LIB)

$(OTOOL) : $(OBJ_OTOOL)
	@$(MAKELIB)
	@gcc -Wall -Wextra -Werror -c $(SRC_OTOOL)
	@gcc -Wall -Wextra -Werror -o $(OTOOL) $(OBJ_OTOOL) $(LIB)

clean :
	@rm -rf $(OBJ_NM) $(OBJ_OTOOL)
	@$(CLEANLIB)

fclean : clean
	@rm -rf $(NM) $(OTOOL)
	@$(FCLEANLIB)

re : fclean all

.PHONY: all clean fclean re

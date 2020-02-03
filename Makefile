# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/02/03 20:26:21 by riblanc           #+#    #+#              #
#    Updated: 2020/02/03 21:25:12 by riblanc          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME				= minishell

###	Libft
LIBFT_LINK	=	-L$(LIBFT_PATH) -lft
LIBFT_PATH	=	./libft
LIBFT_MAKE	=	@$(MAKE) -C $(LIBFT_PATH)
LIBFT_INCL	=	-I $(LIBFT_PATH) -I $(LIBFT_PATH)/headers
LIBS		=	$(LIBFT_LINK)

### Sources
SRCS_DIR	=	./srcs
SRCS_FILES	= 	main.c\
				prompt.c\
				input.c\
				commands.c\
				env.c\
				free_env.c
SRCS		=	$(addprefix $(SRCS_DIR)/, $(SRCS_FILES))

### Headers 
INCLUDES	=	-I./headers $(LIBFT_INCL)

### Compiler
CC			=	clang
CFLAGS		=	-Wall -Wextra -Werror

### Objects
OBJ_DIR		=	./objs
OBJS		=	$(SRCS_FILES:.c=.o)
OBJ      	=	$(addprefix $(OBJ_DIR)/,$(OBJS))
DEP			=	$(OBJ:%.o=%.d)

### run
ARGS		= 

################################################################################

all:		makelib
			@$(MAKE) $(NAME)

$(NAME):	$(OBJ)
			$(CC) $(LIBS)  $(CFLAGS) -o $@ $^

makelib:	
			$(LIBFT_MAKE)

-include $(DEP)
$(OBJ_DIR)/%.o : $(SRCS_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -MMD $(INCLUDES) -o $@ -c $<

bonus:		all

run:		all
			./$(NAME) $(ARGS)		

norme:
			$(LIBFT_MAKE) norme
			norminette $(SRCS)

clean:
			$(RM) $(OBJ)

fclean:
			$(RM) -r $(OBJ_DIR)
			$(RM) $(NAME)
			$(LIBFT_MAKE) fclean

re:			clean all

fre:		fclean all

.PHONY:				all clean fclean re fre norme bonus run makelib

################################################################################

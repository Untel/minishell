# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/02/03 20:26:21 by riblanc           #+#    #+#              #
#    Updated: 2020/03/26 15:52:58 by riblanc          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= minishell

### Sources
SRCS_DIR	= srcs
SRCS_FILES	= \
	main.c \
	prompt.c \
	commands.c \
	input.c \
	input2.c \
	input3.c \
	env.c \
	env_utils.c \
	free_env.c \
	execute.c \
	change_directory.c \
	manage_bin.c \
	export.c \
	process.c \
	echo.c \
	clean.c \
	clean2.c \
	match.c \
	sanitize.c \
	heredoc.c \
	signals.c \
	env_vars.c \
	redirections.c \
	wildcards.c \
	winch.c \
	read_input.c \
	handle_input.c \
	list_utils.c \
	list_utils3.c \
	list_utils4.c \
	list_utils2.c \
	init_termios.c \
	get_next_line.c \
	get_next_line_utils.c \
	history.c \
	history_utils.c

#	word_cursor.c\
//	match_utils.c\
//	match_utils2.c\
//	clipboard.c\

SRCS		=	$(addprefix $(SRCS_DIR)/, $(SRCS_FILES))

###	Libft
LIBFT_LINK	=	-L$(LIBFT_PATH) -lft
LIBFT_PATH	=	./libft
LIBFT_MAKE	=	@$(MAKE) -C $(LIBFT_PATH)
LIBFT_INCL	=	-I $(LIBFT_PATH) -I $(LIBFT_PATH)/headers
LIBS		=	$(LIBFT_LINK) -lncurses

### Headers 
INCLUDES	=	-I ./headers $(LIBFT_INCL)

### Compiler
CC			=	clang
CFLAGS		=	-Wall -Wextra -Werror $(INCLUDES)

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

-include $(DEP)
$(OBJ_DIR)/%.o : $(SRCS_DIR)/%.c
			@mkdir -p $(OBJ_DIR)
			$(CC) $(CFLAGS) -MMD -o $@ -c $<

$(NAME):	$(OBJ)
			$(CC) $(CFLAGS) $(OBJ) $(LIBS) -o $(NAME)
makelib:	
			$(LIBFT_MAKE)

bonus:		all

run:		all
			./$(NAME) $(ARGS)

norme:
			$(LIBFT_MAKE) norme
			norminette $(SRCS)

clean:
			$(RM) -r $(OBJ_DIR)

fclean:
			$(RM) -r $(OBJ_DIR)
			$(RM) $(NAME)
			$(LIBFT_MAKE) fclean

re:			clean all

fre:		fclean all

.PHONY:		all clean fclean re fre norme bonus run makelib

################################################################################

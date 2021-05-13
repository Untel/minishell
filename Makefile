# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: user42 <user42@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/02/03 20:26:21 by riblanc           #+#    #+#              #
#    Updated: 2021/05/13 02:43:18 by riblanc          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= minishell

### Sources
SRCS_DIR	= srcs
SRCS_FILES	= \
	main.c \
	aliases/aliases.c \
	aliases/utils/aliases_utils.c \
	aliases/utils/aliases_utils_2.c \
	builtins/cd.c \
	builtins/echo.c \
	builtins/env.c \
	builtins/exit.c \
	builtins/export.c \
	builtins/pwd.c \
	builtins/unset.c \
	cmd/commands.c \
	cmd/wildcards.c \
	env/env_manager.c \
	env/env_utils.c \
	env/env_vars.c \
	env/free_env.c \
	input_parsing/input.c \
	input_parsing/input2.c \
	input_parsing/input3.c \
	input_parsing/sanitize.c \
	line_edit/append_cmd.c \
	line_edit/auto_complete.c \
	line_edit/handle_input.c \
	line_edit/history.c \
	line_edit/init_termios.c \
	line_edit/read_input.c \
	line_edit/refresh_multi_line.c \
	line_edit/refresh_single_line.c \
	line_edit/select_mode.c \
	line_edit/utils/auto_complete_utils.c \
	line_edit/utils/handle_input_utils.c \
	line_edit/utils/handle_input_utils_2.c \
	line_edit/utils/history_utils.c \
	line_edit/utils/read_input_utils.c \
	line_edit/utils/select_utils.c \
	process/execute.c \
	process/manage_bin.c \
	process/process.c \
	process/utils/manage_bin_utils.c \
	shell_redirect/heredoc.c \
	shell_redirect/redirections.c \
	signals/signals.c \
	signals/winch.c \
	utils/clean.c \
	utils/clean2.c \
	utils/get_next_line.c \
	utils/get_next_line_utils.c \
	utils/list_utils.c \
	utils/list_utils3.c \
	utils/list_utils4.c \
	utils/list_utils2.c \
	utils/match.c

SRCS		=	$(addprefix $(SRCS_DIR)/, $(SRCS_FILES))

###	Libft
LIBFT_LINK	=	-L$(LIBFT_PATH) -lft
LIBFT_PATH	=	./libft
LIBFT_MAKE	=	@$(MAKE) -C $(LIBFT_PATH) --no-print-directory
LIBFT_INCL	=	-I $(LIBFT_PATH) -I $(LIBFT_PATH)/headers
LIBS		=	$(LIBFT_LINK)

### Headers 
INCLUDES	=	-I ./headers $(LIBFT_INCL)

### Compiler
ML			=	1
CC			=	clang
CFLAGS		=	-Wall -Wextra -Werror $(INCLUDES)
DBG			=	0

ifeq ($(DBG), 1)
	CFLAGS += -g3 -fsanitize=address
	DEBUG = $(DBG)
else
	DEBUG = 0
endif

ifneq ($(filter $(ML),0 1),)
	CFLAGS	+= -D MULTI=$(ML)
	MULTI := $(ML)
else
	MULTI := 1
endif

### Objects
OBJ_DIR		=	./objs
OBJS		=	$(SRCS_FILES:.c=.o)
OBJ      	=	$(addprefix $(OBJ_DIR)/,$(OBJS))
DEP			=	$(OBJ:%.o=%.d)

### run
ARGS		= 

################################################################################

all:		makelib
			@$(MAKE) $(NAME) --no-print-directory

-include $(DEP)
$(OBJ_DIR)/%.o : $(SRCS_DIR)/%.c .ML.$(MULTI) .DBG.$(DEBUG)
			@mkdir -p $(OBJ_DIR)
			@mkdir -p $(OBJ_DIR)/aliases/utils
			@mkdir -p $(OBJ_DIR)/builtins
			@mkdir -p $(OBJ_DIR)/cmd
			@mkdir -p $(OBJ_DIR)/env
			@mkdir -p $(OBJ_DIR)/input_parsing
			@mkdir -p $(OBJ_DIR)/line_edit/utils
			@mkdir -p $(OBJ_DIR)/process/utils
			@mkdir -p $(OBJ_DIR)/shell_redirect
			@mkdir -p $(OBJ_DIR)/signals
			@mkdir -p $(OBJ_DIR)/utils
			$(CC) $(CFLAGS) -MMD -o $@ -c $<

$(NAME):	$(OBJ)
			$(CC) $(CFLAGS) $(OBJ) $(LIBS) -o $(NAME)

.ML.$(MULTI): 
	@rm -f .ML.*
	@touch $@

.DBG.$(DEBUG): 
	@rm -f .DBG.*
	@touch $@

makelib:	
			$(LIBFT_MAKE)

single:		clean
			@$(MAKE) ML=0 --no-print-directory

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

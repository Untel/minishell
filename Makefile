LIBFT_LINK			= -L$(LIBFT_PATH) -lft
LIBFT_PATH			= ./libft
LIBFT_MAKE			= @$(MAKE) -C $(LIBFT_PATH)
LIBFT_INCL			= -I $(LIBFT_PATH) -I $(LIBFT_PATH)/headers

LIBS				= $(LIBFT_LINK)

SRCS_DIR			= srcs
SRCS_FILES			= \
	main.c\
	prompt.c\
	input.c

SRCS				= $(addprefix $(SRCS_DIR)/, $(SRCS_FILES))

INCLUDES			= -I./srcs $(LIBFT_INCL)

CC					= clang
CFLAGS				= -Wall -Wextra -Werror $(INCLUDES)
OBJS				= $(SRCS:.c=.o)
NAME				= minishell

ARGS				= 


all:				makelib
					@$(MAKE) $(NAME)

$(NAME):			$(OBJS)
					$(CC) $(OBJS) $(LIBS) -o $(NAME)

bonus:				all

run:				all
					./$(NAME) $(ARGS)		

makelib:			
					$(LIBFT_MAKE)

norme:
					$(LIBFT_MAKE) norme
					norminette $(SRCS)

clean:
					$(RM) $(OBJS)

fclean:
					$(RM) $(OBJS)
					$(RM) $(NAME)
					$(LIBFT_MAKE) fclean

re:					clean all

fre:				fclean all

.PHONY:				all clean fclean re fre norme bonus run makelib

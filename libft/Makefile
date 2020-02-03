STR_DIR			= str
CHAR_DIR		= char
PRINT_DIR		= print
MEM_DIR			= mem
MATH_DIR		= math
LIST_DIR		= list
WCHAR_DIR		= wide
FILES_DIR		= files
PRINTF_DIR		= printf

INCLUDES_DIR	= headers

_MATH			= ft_intlen.c ft_itoa.c ft_uitoa.c ft_atoi.c ft_pow.c
_STR			= ft_strcat.c ft_strcpy_free.c ft_strdup.c ft_split.c ft_split_charset.c ft_split_spaces.c ft_strlen.c ft_strnstr.c ft_strcpy.c ft_substr.c ft_strjoin.c ft_strmjoin.c ft_strmapi.c ft_strrchr.c ft_strlcat.c ft_strstr.c ft_strchr.c ft_strlcpy.c ft_strncmp.c ft_strtrim.c ft_strupcase.c ft_strlowcase.c ft_strcapitalize.c ft_strslice.c
_CHAR			= ft_isalpha.c ft_isascii.c ft_tolower.c ft_toupper.c ft_islower.c ft_isupper.c ft_isdigit.c ft_isalnum.c ft_isprint.c ft_isspace.c
_PRINT			= ft_putchar_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_putstr_fd.c
_MEM			= ft_memdel.c ft_memcmp.c ft_bzero.c ft_memcpy.c ft_calloc.c ft_memccpy.c ft_memmove.c ft_memchr.c ft_memset.c
_LIST			= ft_lstdelone.c ft_lstnew.c ft_lstadd_back.c ft_lstiter.c ft_lstsize.c ft_lstclear.c ft_lstprint.c ft_lstmap.c ft_lstadd_front.c ft_lstlast.c
_WCHAR			= ft_wchar_to_char.c ft_is_wchar.c
_FILES			= get_next_line.c
_PRINTF			= ft_parseint.c ft_parsechar.c ft_parsebase.c ft_parsefloat.c ft_parseexp.c ft_convert_base.c ft_printf_utils.c ft_extract_flags.c ft_convert.c ft_printf.c
_ASM			= list/ft_lstremove_if.s

STR				= $(addprefix $(STR_DIR)/, $(_STR))
CHAR			= $(addprefix $(CHAR_DIR)/, $(_CHAR))
PRINT			= $(addprefix $(PRINT_DIR)/, $(_PRINT))
MEM				= $(addprefix $(MEM_DIR)/, $(_MEM))
MATH			= $(addprefix $(MATH_DIR)/, $(_MATH))
LIST			= $(addprefix $(LIST_DIR)/, $(_LIST))
WCHAR			= $(addprefix $(WCHAR_DIR)/, $(_WCHAR))
FILES			= $(addprefix $(FILES_DIR)/, $(_FILES))
PRINTF			= $(addprefix $(PRINTF_DIR)/, $(_PRINTF))

SRCS			= $(STR) $(CHAR) $(PRINT) $(MEM) $(MATH) $(LIST) $(WCHAR) $(FILES) $(PRINTF)
OBJSC			= $(SRCS:.c=.o)
OBJSS			= $(_ASM:.s=.o)
CFLAGS			= -Wall -Wextra -Werror -I $(INCLUDES_DIR)
CC				= clang
AR				= ar rc
AS				= nasm
ASFLAGS			= -fmacho64
RM				= rm -f
NAME			= libft.a

$(NAME):		$(OBJSS) $(OBJSC) headers/libft.h
				$(AR) $(NAME) $(OBJSS) $(OBJSC)

all:			$(NAME)

clean:
				$(RM) $(OBJSS) $(OBJSC)

fclean:			clean
				$(RM) $(NAME)

norme:				
				norminette $(SRCS) headers/libft.h

re:				fclean all

.PHONY:			all clean fclean re norme

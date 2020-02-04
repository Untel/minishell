/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/07 11:45:14 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/04 09:35:51 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H
# include <string.h>
# include <stdint.h>
# include <ctype.h>
# include <wchar.h>

typedef	struct	s_list
{
	void			*content;
	struct s_list	*next;
	size_t			size;
}				t_list;
int				ft_atoi(const char *str);
int				ft_isalpha(int c);
int				ft_isupper(int c);
int				ft_islower(int c);
int				ft_isupper(int c);
int				ft_isdigit(int c);
int				ft_isalnum(int c);
int				ft_isspace(int c);
int				ft_isascii(int c);
int				ft_isprint(int c);
int				ft_toupper(int c);
int				ft_tolower(int c);
uint64_t		ft_pow(uint64_t nb, int power);
int				ft_strncmp(const char *s1, const char *s2, size_t n);
void			ft_memdel(void **ptr);
int				ft_imemdel(void **ptr);
int				ft_memcmp(const void *s1, const void *s2, size_t n);
int				ft_wchar_to_char(char *buff, wchar_t code);
int				ft_wchars_to_str(char *buff, wchar_t *codes);
int				ft_is_wchar(unsigned char c);
int				ft_is_fwchar(unsigned char c);
size_t			ft_strlen(const char *str);
size_t			ft_strlcat(char *dest, const char *src, size_t dstsize);
size_t			ft_strlcpy(char *dest, const char *src, size_t dstsize);
size_t			ft_strcpy(char *dest, const char *src);
size_t			ft_strcpy_free(char *dest, const char *src, void *to_free);
size_t			ft_strcat(char *dest, char *src);
size_t			ft_intlen(int64_t n);
size_t			ft_uintlen(uint64_t nt);
void			ft_bzero(void *s, size_t n);
void			ft_putchar_fd(char c, int fd);
void			ft_putstr_fd(char *s, int fd);
void			ft_putendl_fd(char *s, int fd);
void			ft_putnbr_fd(int n, int fd);
char			*ft_strncat(char *dest, char *src, size_t nb);
char			*ft_upcase(const char *s);
char			*ft_lowcase(const char *s);
char			*ft_capitalize(const char *s);
char			*ft_strchr(const char *s, int c);
char			*ft_strrchr(const char *s, int c);
char			*ft_strstr(const char *str, const char *to_find);
char			*ft_strnstr(const char *str, const char *to_find, size_t len);
char			*ft_strdup(const char *src);
char			*ft_strndup(const char *src, size_t n);
char			*ft_strdup_until(const char *src, char c);
char			*ft_substr(char const *s, unsigned int start, size_t len);
char			*ft_subwstr(char const *s, unsigned int start, size_t len);
char			*ft_strjoin(char const *s1, char const *s2);
char			*ft_strmjoin(int size, char **strs, char *sep);
char			*ft_strtrim(char const *s1, char const *set);
char			*ft_itoa(int n);
char			*ft_uitoa(unsigned int n);
char			*ft_llitoa(int64_t n);
char			*ft_lluitoa(uint64_t nt);
char			*ft_itoa_wrapper(uint64_t nt, char sep, int is_neg);
char			*ft_strmapi(char const *s, char (*f)(unsigned int, char));
char			*ft_strupcase(const char *str);
char			*ft_strlowcase(const char *str);
char			*ft_strcapitalize(const char *str);
char			*ft_strslice(char *str, size_t start, size_t count);
void			*ft_memset(void *b, int c, size_t len);
void			*ft_memmove(void *dst, const void *src, size_t len);
void			*ft_memcpy(void *dst, const void *src, size_t n);
void			*ft_memccpy(void *dst, const void *src, int c, size_t n);
void			*ft_memchr(const void *s, int c, size_t n);
void			*ft_calloc(size_t count, size_t size);
char			**ft_split(const char *str, char c);
char			**ft_split_charset(const char *str, char *charset);
char			**ft_split_spaces(const char *str);
char			*ft_strreplace(char *buffer, char *target, char *replace);
t_list			*ft_lstnew(void *content, size_t size);
t_list			*ft_lstlast(t_list *lst);
t_list			*ft_lstmap(t_list *lst,
	t_list *(*f)(t_list *), void (*d)(t_list *));
int				ft_lstsize(t_list *lst);
void			ft_lstadd_front(t_list **alst, t_list *n);
void			ft_lstadd_back(t_list **alst, t_list *n);
void			ft_lstdelone(t_list *lst, void (*del)(t_list *));
void			ft_lstremove_if(t_list **begin_list, void *data_ref,
	int (*cmp)(), void (*free_fct)(void *));
void			ft_lstclear(t_list **lst, void (*del)(t_list *));
void			ft_lstprint(t_list *lst, char *cmt);
void			ft_lstiter(t_list *lst, void (*f)(t_list *));
#endif

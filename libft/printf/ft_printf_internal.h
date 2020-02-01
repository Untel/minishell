/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_internal.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/20 20:23:04 by adda-sil          #+#    #+#             */
/*   Updated: 2020/01/19 20:02:56 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_INTERNAL_H
# define FT_PRINTF_INTERNAL_H
# include "ft_printf.h"
# include "libft.h"
# include <stdarg.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <locale.h>
# include <wchar.h>
# define OUTPUT_FD 1
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 4096
# endif
# define B BUFFER_SIZE

typedef enum	e_argsize
{
	H, HH, N, L, LL
}				t_argsize;
typedef	struct	s_modifiers
{
	size_t		padding;
	char		padchar;
	char		sep;
	int			align_left;
	int			precision;
	int			alt;
	t_argsize	size;
	char		sign;
	int			trail;
	char		conv;
}				t_mod;
void			*ft_f(void *ptr, void *res);
int				ft_convert(t_mod m, t_list **s, va_list a);
int				ft_extract_flags(const char *s, va_list a, t_mod *m);
int				ft_is_conv(char *str, char c);
char			*ft_nbrbase(uintptr_t nbr, char *base, unsigned int base_size);
char			*ft_fill(char *str, size_t count, char c, int align_left);
char			*ft_fill_c(char cc, size_t count, char c, int align_left);
char			*ft_add_sign(char sign, char *str);
int				ft_parse_int(char **b, t_mod m, va_list a);
int				ft_parse_char(char **b, t_mod m, va_list a);
int				ft_parse_string(char **b, t_mod m, va_list a);
int				ft_parse_base(char **b, t_mod m, va_list a);
int				ft_parse_float(char **b, t_mod m, va_list a);
int64_t			ft_get_sized_int(va_list args, t_mod mods);
uint64_t		ft_get_sized_uint(va_list args, t_mod mods);
int				ft_printf(const char *str, ...);
int				ft_sprintf(char *buffer, const char *str, ...);
int				ft_split_to_list(const char *str, t_list **lst, va_list args);
char			*ft_stringify_exp(long double a, size_t d, t_mod m);
char			*ft_stringify_float(long double v, size_t d, t_mod m);
#endif

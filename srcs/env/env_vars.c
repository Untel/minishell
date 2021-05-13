/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_vars.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/05 12:26:48 by adda-sil          #+#    #+#             */
/*   Updated: 2020/03/12 16:01:06 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int
	replace_last_ret(t_shell *sh, char **str, char **ptr)
{
	char	*mjoin[3];
	char	*ret;
	int		position;

	mjoin[0] = ft_strndup((*str), (*ptr) - (*str));
	mjoin[1] = ft_itoa(sh->last_ret);
	mjoin[2] = (*ptr) + 2;
	ret = ft_strmjoin(3, mjoin, "");
	position = (*ptr) - (*str) + ft_strlen(mjoin[1]);
	ft_memdel((void **)&mjoin[0]);
	ft_memdel((void **)&mjoin[1]);
	ft_memdel((void **)str);
	*str = ret;
	return (position);
}

int
	replace_var(t_shell *sh, char **str, char **ptr)
{
	char	*mjoin[3];
	char	*key;
	int		key_len;
	int		position;

	key_len = 1;
	while ((*ptr)[key_len] && (ft_isalnum((*ptr)[key_len])
		|| (*ptr)[key_len] == '_'))
		++key_len;
	if (key_len == 1)
		return ((*ptr) - (*str));
	mjoin[0] = ft_strndup(*str, (*ptr) - (*str));
	key = ft_strndup((*ptr) + 1, key_len - 1);
	mjoin[1] = get_value(sh->env, key, "");
	position = (*ptr) - (*str) + ft_strlen(mjoin[1]);
	mjoin[2] = (*ptr) + key_len;
	ft_memdel((void **)&key);
	key = ft_strmjoin(3, mjoin, "");
	ft_memdel((void **)&mjoin[0]);
	ft_memdel((void **)str);
	*str = key;
	return (position);
}

char
	*replace_tilde(t_shell *sh, char *str)
{
	char	*ptr;
	char	*mjoin[3];

	if (!(ptr = ft_strchr_escape(str, '~', '\\')))
		return (str);
	if (!(ptr[1] == ' ' || ptr[1] == '/' || !ptr[1]))
		return (str);
	mjoin[0] = ft_strndup(str, ptr - str);
	mjoin[1] = get_value(sh->env, "HOME", "");
	mjoin[2] = ptr + 1;
	ptr = ft_strmjoin(3, mjoin, "");
	ft_memdel((void **)&str);
	ft_memdel((void **)&mjoin[0]);
	return (ptr);
}

char
	*replace_vars(t_shell *sh, char *str)
{
	char	*ptr;
	int		diff;

	diff = 0;
	while ((ptr = ft_strchr_escape(str + diff, '$', '\\')))
	{
		if (!ptr[1])
			return (str);
		if (!(ft_isalpha(ptr[1]) || ptr[1] == '_' || ptr[1] == '?') && ++diff)
			continue ;
		diff = ptr[1] == '?' ? replace_last_ret(sh, &str, &ptr)
			: replace_var(sh, &str, &ptr);
	}
	return (str);
}

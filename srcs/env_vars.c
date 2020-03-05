/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_vars.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/05 12:26:48 by adda-sil          #+#    #+#             */
/*   Updated: 2020/03/05 13:56:59 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// char
// 	*deep_replace(t_shell *sh, char *str)
// {
	
// }

char
	*replace_vars(t_shell *sh, char *str)
{
	char	*ptr;
	int		key_len;
	char	*tmp;
	char	*key;
	char	*value;

	if (!(ptr = ft_strchr_escape(str, '$', '\\')))
		return (str);
	ft_fprintf(2, "Found %s -> %s\n", str, ptr);
	tmp = ft_strndup(str, ptr - str);
	key_len = 1;
	if (*(ptr + 1) == '?')
	{
		value = ft_itoa(sh->last_ret);
		key = value ? ft_strjoin(tmp, value) : ft_strdup(tmp);
		ft_memdel((void **)&value);
		value = ft_strjoin(key, ptr + 1 + key_len);
	}
	else
	{
		while (ptr[key_len] && !ft_strchr(" /$", ptr[key_len]))
			++key_len;
		key = ft_strndup(ptr + 1, key_len - 1);
		value = get_value(sh->env, key, NULL);
		ft_fprintf(2, "Found %s -> %s\n", key, value);
		ft_memdel((void **)&key);
		key = value ? ft_strjoin(tmp, value) : ft_strdup(tmp);
		value = ft_strjoin(key, ptr + key_len);
	}
	ft_memdel((void **)&tmp);
	ft_memdel((void **)&key);
	ft_memdel((void **)&str);
	return (replace_vars(sh, value));
}

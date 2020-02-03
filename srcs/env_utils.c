/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/03 23:30:06 by riblanc           #+#    #+#             */
/*   Updated: 2020/02/03 23:58:52 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

void	*set_value(t_list **lst_env, char *key, char *value)
{
	t_key	*var;
	int		found;

	found = 0;
	while (*lst_env)
	{
		if ((*lst_env)->next && (var = (t_key *)(*lst_env)->next->content))
			if (!strcmp(var->key, key) && (found = 1))
				break ;
		lst_env = &(*lst_env)->next;
	}
	if (!found)
	{
		if (!(var = malloc(sizeof(*var))))
			return (NULL);
		var->key = ft_strdup(key);
		var->value = ft_strdup(value);
		ft_lstadd_front(lst_env, ft_lstnew(var, sizeof(var)));
		return (*lst_env);
	}
	free(var->value);
	var->value = ft_strdup(value);
	return (*lst_env);
}

void	*get_value(t_list *lst_env, char *key)
{
	int		found;

	found = 0;
	while (lst_env)
	{
		if (!strcmp(((t_key *)(*lst_env).content)->key, key) && (found = 1))
			break ;
		lst_env = lst_env->next;
	}
	if (found == 1)
		return (((t_key *)(*lst_env).content)->value);
	return (NULL);
}

void	*unset_key(t_list **lst_env)
{
}

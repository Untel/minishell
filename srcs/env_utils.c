/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/03 23:30:06 by riblanc           #+#    #+#             */
/*   Updated: 2020/03/06 01:28:23 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

int
	is_key_env_valid(char *key)
{
	int i;

	i = 0;
	if (!(ft_isalpha(*key) || *key == '_'))
		return (0);
	while (key[++i])
		if (!(ft_isalnum(*key) || *key == '_'))
			return (0);
	return (1);
}

void
	*set_value(t_list **lst_env, char *key, char *value)
{
	t_key	*var;
	int		found;

	found = 0;
	if (!is_key_env_valid(key))
		return (0);
	while (*lst_env)
	{
		if ((*lst_env)->next && (var = (t_key *)(*lst_env)->next->content))
			if (!ft_strcmp(var->key, key) && (found = 1))
				break ;
		lst_env = &(*lst_env)->next;
	}
	if (!found)
	{
		if (!(var = malloc(sizeof(*var))))
			return (NULL);
		var->key = ft_strdup(key);
		var->value = value ? ft_strdup(value) : value;
		ft_lstadd_front(lst_env, ft_lstnew(var, sizeof(var)));
		return (*lst_env);
	}
	ft_memdel((void **)&var->value);
	var->value = value ? ft_strdup(value) : value;
	return (*lst_env);
}

void
	*get_value(t_list *lst_env, char *key, char *def)
{
	int		found;

	found = 0;
	while (lst_env)
	{
		if (!ft_strcmp(((t_key *)lst_env->content)->key, key) && (found = 1))
			break ;
		lst_env = lst_env->next;
	}
	if (found == 1)
		return (((t_key *)lst_env->content)->value);
	else
		return (def);
}

int
	ft_cmp_key(t_key *env, char *key)
{
	return (ft_strcmp(env->key, key));
}

void
	unset_key(t_list **lst_env, char *key)
{
	ft_lstremove_if(lst_env, key, ft_cmp_key, free_env_unset);
}

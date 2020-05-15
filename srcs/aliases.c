/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aliases.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:11:55 by riblanc           #+#    #+#             */
/*   Updated: 2020/05/15 21:47:24 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			arg_found(char **strs, char *arg)
{
	int		i;

	i = 0;
	(void)arg;
	while (*(strs + i))
		if (!ft_strcmp(*(strs + i++), arg))
			return (1);
	return (0);
}

static void	check_arg(t_cmd *cmd, char **argv)
{
	int		i;

	if (!ft_strcmp(cmd->argv[0], argv[0]))
		free(argv[0]);
	else
	{
		free(cmd->argv[0]);
		cmd->argv[0] = argv[0];
	}
	i = ft_strslen(argv);
	while (--i > 0)
	{
		if (arg_found(cmd->argv, argv[i]))
		{
			free(argv[i]);
			argv[i] = 0;
		}
		else
			add_argument_index(cmd, argv[i], 1);
	}
	free(argv);
}

void		check_aliases(t_shell *sh, t_cmd *cmd)
{
	t_alias *alias;
	char	**argv;

	alias = sh->alias;
	while (alias)
	{
		if (!ft_strcmp(cmd->argv[0], alias->key))
		{
			if (!(argv = ft_split_chst(alias->value, "\t\v\n\r\f ")))
				return ;
			check_arg(cmd, argv);
		}
		alias = alias->next;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   match.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/08 22:39:41 by riblanc           #+#    #+#             */
/*   Updated: 2020/10/15 16:11:13 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "line_edit.h"

int		match(char *s1, char *s2)
{
	if (*s1 != '\0' && *s2 == '*')
		return (match(s1 + 1, s2) || match(s1, s2 + 1));
	if (*s1 == '\0' && *s2 == '*')
		return (match(s1, s2 + 1));
	if (*s1 == *s2 && *s1 != '\0' && *s2 != '\0')
		return (match(s1 + 1, s2 + 1));
	if (*s1 == *s2 && *s1 == '\0')
		return (1);
	return (0);
}

char	**get_nmatch(t_shell *sh, char **path, char *file)
{
	t_nmatch	utils;

	utils.ret = 0;
	*path = replace_tilde(sh, *path);
	*path = replace_vars(sh, *path);
	utils.dp = opendir(*path);
	if (utils.dp != NULL)
	{
		while ((utils.ep = readdir(utils.dp)))
			if (match(utils.ep->d_name, file))
			{
				if (!ft_strncmp(utils.ep->d_name, ".", 1))
					if (file[0] != '.')
						continue ;
				++utils.ret;
			}
		closedir(utils.dp);
	}
	return (get_matchlist(utils, path, file));
}

char	**get_matchlist(t_nmatch utils, char **path, char *file)
{
	char			**lst;

	if (!(lst = malloc(sizeof(char *) * (utils.ret + 2))))
		return (NULL);
	lst[utils.ret] = ft_strndup(file, ft_strlen(file) - 1);
	lst[utils.ret + 1] = 0;
	utils.dp = opendir(*path);
	utils.i = 0;
	if (utils.dp != NULL)
	{
		while ((utils.ep = readdir(utils.dp)))
			if (match(utils.ep->d_name, file))
			{
				if (!ft_strncmp(utils.ep->d_name, ".", 1))
					if (file[0] != '.')
						continue ;
				if (utils.ep->d_type == DT_DIR)
					lst[utils.i] = ft_strjoin(utils.ep->d_name, "/");
				else
					lst[utils.i] = ft_strdup(utils.ep->d_name);
				++utils.i;
			}
		closedir(utils.dp);
	}
	return (lst);
}

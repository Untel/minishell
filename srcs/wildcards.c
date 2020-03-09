/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/08 17:38:46 by adda-sil          #+#    #+#             */
/*   Updated: 2020/03/09 17:05:21 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

typedef struct		s_wildcards_checker
{
	char			**paths;
	int				depth;
	int				max_depth;
	t_list			*matchs;
}					t_wildcards_checker;

int
	check_matchs(t_wildcards_checker *wc, char *path, int depth, char *deep)
{
	struct dirent	*file;
	DIR				*dir;
	char			*str;

	if (!(dir = opendir(path)))
		return (0);
	depth = depth >= wc->max_depth ? wc->max_depth : depth;
	if (ft_strstr(wc->paths[depth], "**"))
		deep = wc->paths[depth];
	while ((file = readdir(dir)) != NULL)
	{
		if (ft_strcmp(file->d_name, ".") == 0 || ft_strcmp(file->d_name, "..") == 0)
			continue ;
		str = depth > 0 ? ft_strjoin_sep(path, file->d_name, '/') : ft_strdup(file->d_name);
		if (match(file->d_name, wc->paths[depth]))
		{
			if (depth >= wc->max_depth - 1)
				ft_lstadd_back(&wc->matchs, ft_lstnew(ft_strdup(str), ft_strlen(str)));
			else if (depth < wc->max_depth - 1)
				check_matchs(wc, str, depth + 1, deep);
		}
		else if (deep && match(file->d_name, deep))
			check_matchs(wc, str, depth, deep);			
		free(str);
	}
	closedir(dir);
	return (1);
}

void
	clean_wildcards(t_wildcards_checker *wc, char *pattern)
{
	t_list	*el;
	int		i;

	i = 0;
	while(wc->paths[i])
		ft_memdel((void **)&wc->paths[i++]);
	ft_memdel((void **)&wc->paths);
	while ((el = wc->matchs))
	{
		wc->matchs = wc->matchs->next;
		ft_memdel((void **)&el);
	}
	ft_memdel((void **)&pattern);
}

int
	check_wildcards(t_shell *sh, t_cmd *cmd, t_read *rd, char *pattern)
{
	t_wildcards_checker wc;
	t_list				*lst;
	int					ret;

	ret = SUC;
	wc = (t_wildcards_checker) {
		.matchs = NULL, .paths = ft_split(pattern, '/'),
		.depth = -1, .max_depth = 0, };
	while (wc.paths[wc.max_depth])
		wc.max_depth++;
	if (wc.max_depth > 0 && 
		check_matchs(&wc, *pattern == '/' ? "/" : ".", 0, NULL))
	{
		lst = wc.matchs;
		while (lst)
		{
			if (rd->add_to == ARGS)
				add_argument(cmd, lst->content);
			else
				add_redir(sh, cmd, lst->content, rd);
			lst = lst->next;
		}
	}
	if (ft_lstsize(wc.matchs) == 0)
		ret = (ft_fprintf(STDERR, MSG_ERR_REG, pattern) && FALSE);
	clean_wildcards(&wc, pattern);
	return (ret);
}

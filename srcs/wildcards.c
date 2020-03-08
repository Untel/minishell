/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/08 17:38:46 by adda-sil          #+#    #+#             */
/*   Updated: 2020/03/08 20:02:30 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

typedef struct		s_wildcards_checker
{
	char			**paths;
	int				root;
	int				depth;
	int				max_depth;
	int				index;
	t_list			*matchs;
	DIR				*dir;
}					t_wildcards_checker;

char
	*check_matchs(t_wildcards_checker *wc, char *path, int depth)
{
	struct dirent	*file;
	DIR				*dir;

	ft_fprintf(STDERR, "Checking at %s\n", path);
	if (!(dir = opendir(path)))
		return (NULL);
	while ((file = readdir(dir)) != NULL)
	{
		ft_fprintf(STDERR, "%s vs %s: ", file->d_name, wc->paths[depth]);
		if (match(file->d_name, wc->paths[depth]))
		{
			ft_lstadd_front(&wc->matchs, ft_lstnew(
				ft_strjoin(path, file->d_name), ft_strlen(file->d_name)
			));
			ft_fprintf(STDERR, "Matching %s\n", (char *)wc->matchs->content);
		}
		else
			ft_fprintf(STDERR, "Nomatch %s\n", file->d_name);
	}
	ft_fprintf(STDERR, "END?\n");
	return (NULL);
}

t_list
	*check_wildcards(char *pattern)
{
	t_wildcards_checker wc;
	wc = (t_wildcards_checker) {
		.matchs = NULL,
		.paths = NULL,
		.depth = -1,
		.max_depth = 0,
		.index = 0,
		.dir = NULL,
	};
	ft_fprintf(STDERR, "Start checking from pattern %s\n", pattern);
	wc.paths = ft_split(pattern, '/');
	while (wc.paths[wc.max_depth])
		wc.max_depth++;
	if (wc.max_depth > 0)
		check_matchs(&wc, *pattern == '/' ? "/" : ".", 0);
	return (wc.matchs);
}

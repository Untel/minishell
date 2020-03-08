/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sanitize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/18 17:14:01 by adda-sil          #+#    #+#             */
/*   Updated: 2020/03/08 15:14:38 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int
	ask_concat(t_shell *sh, char *ask, char **place, char *stopif)
{
	char	*tmp;
	char	*tmps[2];
	char	*buffer;
	int		offset;

	offset = ft_printf("%s> ", ask);
	sh->term.size_prt = ft_strlen(ask) + 2;
	buffer = read_input(sh);
	write(1, "\n", 1);
	if (buffer == (char *)ERR)
		return (ERR);
	if (stopif && ft_strcmp(stopif, buffer) == 0)
	{
		ft_memdel((void **)&buffer);
		return (0);
	}
	tmps[0] = *place ? *place : "";
	tmps[1] = buffer;
	tmp = ft_strmjoin(2, tmps, *place ? "\n" : "");
	ft_memdel((void **)&buffer);
	ft_memdel((void **)&*place);
	*place = tmp;
	return (1);
}

int
	ask_to_close(t_shell *sh, char *ask)
{
	ft_lstclear(&sh->heredocs, free_heredocs);
	ft_lstclear(&sh->cmds, free_command);
	if (ask_concat(sh, ask, &sh->input, NULL) == ERR)
		return (ERR);
	return (sanitize(sh));
}

int
	check_input(t_shell *sh, t_quoter *q)
{
	int i;

	i = -1;
	while (sh->input[++i])
	{
		if (sh->input[i] == ' ' || sh->input[i] == ';')
			continue;
		q->cc = q->c;
		q->c = sh->input[i];
		if (q->c == '\\')
			q->bslash = i + 1;
		else if (q->c == '\'' && !q->d && q->bslash != i)
			q->s = !q->s;
		else if (q->c == '"' && !q->s && q->bslash != i)
			q->d = !q->d;
		else if (q->c == '<' && sh->input[i + 1] == '<'
			&& !q->s && q->bslash != i++)
			if (!add_heredoc(sh, &i))
				return (FALSE);
	}
	q->i = i;
	return (SUC);
}

int
	sanitize(t_shell *sh)
{
	t_quoter	q;

	q = (t_quoter) { .s = 0, .d = 0, .bslash = -1, .c = 0, .cc = 0, .i = 0 };
	if (!(check_input(sh, &q)))
		return (FALSE);
	if (q.s)
		return (ask_to_close(sh, "quote"));
	else if (q.d)
		return (ask_to_close(sh, "dquote"));
	else if (q.c == '|')
		return (ask_to_close(sh, q.cc == '|' ? "cmdor" : "pipe"));
	else if (q.c == '&' && q.cc == '&')
		return (ask_to_close(sh, "cmdand"));
	else if (q.bslash == q.i)
		return (ask_to_close(sh, ""));
	ask_heredocs(sh);
	return (SUC);
}

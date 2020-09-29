/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sanitize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/18 17:14:01 by adda-sil          #+#    #+#             */
/*   Updated: 2020/09/29 15:40:29 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "line_edit.h"

int
	ask_concat(t_shell *sh, char *ask, char **place, char *stopif)
{
	char	*tmp;
	char	*tmps[2];
	char	*buffer;
	char	*prompt;
	char	seq[64];

	prompt = NULL;
	sh->term.size_prt = ft_sprintf(seq, "%s> ", ask);
	append(&prompt, ft_strdup(seq));
	buffer = read_input(sh, prompt, MULTI, sh->term.size_prt);
	ft_memdel((void **)&prompt);
	if (buffer == (char *)ERR)
		return (ERR);
	if (stopif && ft_strcmp(stopif, buffer) == 0)
		return (!ft_imemdel((void **)&buffer));
	tmps[0] = *place ? *place : "";
	tmps[1] = buffer;
	tmp = ft_strmjoin(2, tmps, *place ? "\n" : "");
	ft_memdel((void **)&buffer);
	ft_memdel((void **)&*place);
	*place = tmp;
	return (1);
}

int
	ask_to_close(t_shell *sh, char *ask, int bypass)
{
	ft_lstclear(&sh->heredocs, free_heredocs);
	ft_lstclear(&sh->cmds, free_command);
	if (sh->inline_fd != ERR)
		return (bypass ? SUC : !ft_fprintf(STDERR, MSG_ERROR, "syntax error"));
	if (ask_concat(sh, ask, &sh->input, NULL) == ERR)
		return (ERR);
	return (sanitize(sh));
}

int
	check_input2(t_shell *sh, t_quoter *q, int i)
{
	q->cc = q->c;
	q->c = sh->input[i];
	if (q->c == '\\' && !q->s && q->bslash != i)
		(q->bslash = i + 1);
	else if (q->c == '\'' && !q->d && q->bslash != i)
		(q->s = !q->s);
	else if (q->c == '"' && !q->s && q->bslash != i)
		(q->d = !q->d);
	else if (q->c == '<' && sh->input[i + 1] == '<'
		&& !q->s && q->bslash != i++)
		if (!add_heredoc(sh, &i))
			return (SUC);
	return (FALSE);
}

int
	check_input(t_shell *sh, t_quoter *q)
{
	int i;

	i = -1;
	while (sh->input[++i])
	{
		if (sh->input[i] == ' ')
			continue;
		else if (sh->input[i] == ';')
		{
			if (i && sh->input[i - 1] == ';')
				return (!ft_fprintf(STDERR, SYN_ERR, sh->input[i], i));
			continue ;
		}
		else if (check_input2(sh, q, i))
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
		return (ask_to_close(sh, "quote", 0));
	else if (q.d)
		return (ask_to_close(sh, "dquote", 0));
	else if (q.c == '|')
		return (ask_to_close(sh, q.cc == '|' ? "cmdor" : "pipe", 0));
	else if (q.c == '&' && q.cc == '&')
		return (ask_to_close(sh, "cmdand", 0));
	else if (q.bslash == q.i)
		return (ask_to_close(sh, "", 1));
	ask_heredocs(sh);
	return (SUC);
}

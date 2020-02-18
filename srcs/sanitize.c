/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sanitize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/18 17:14:01 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/18 19:13:12 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int
	free_heredocs(t_list *lst)
{
	t_heredoc *hd;

	hd = (t_heredoc *)lst->content;
	ft_memdel((void **)&hd->label);
	ft_memdel((void **)&hd->buffer);
	ft_memdel((void **)&hd);
	return (SUC);
}

int
	ask_concat(t_shell *sh, char *ask, char **place)
{
	char	*tmp;
	char	*tmps[2];
	char	*buffer;
	int		offset;
	int		len;

	offset = ft_printf("%s> ", ask);
	sh->term.size_prt = ft_strlen(ask) + 2;
	buffer = read_input(sh);
	write(1, "\n", 1);
	if (buffer == (char *)ERR)
		return (ERR);
	tmps[0] = *place;
	tmps[1] = buffer;
	len = ft_strlen(*place);
	tmp = ft_strmjoin(2, tmps, "\n");
	free(buffer);
	free(*place);
	*place = tmp;
	return (len + 1);
}

int
	ask_to_close(t_shell *sh, char *ask)
{
	ft_lstclear(&sh->heredocs, free_heredocs);
	ft_lstclear(&sh->cmds, free_command);
	ask_concat(sh, ask, &sh->input);
	return (sanitize(sh));
}

int
	add_heredoc(t_shell *sh, int *i)
{
	t_heredoc	*hd;
	size_t		j;

	j = 0;
	if (!(hd = malloc(sizeof(t_heredoc))))
		return (ERR);
	while (sh->input[*i + 1] == ' ')
		*i++;
	*hd = (t_heredoc) { .label = NULL, .buffer = ft_strdup(""), };
	while (ft_isalnum(sh->input[*i + 1 + j]))
		j++;
	hd->label = ft_strndup(&sh->input[*i + 1], j);
	ft_lstadd_back(&sh->heredocs, ft_lstnew(hd, sizeof(t_heredoc *)));
	return (SUC);
}

int
	ask_heredocs(t_shell *sh)
{
	t_heredoc	*hd;
	t_list		*lst;
	int			len;
	char		asker[BUFFER_SIZE];

	lst = sh->heredocs;
	while (lst)
	{
		hd = (t_heredoc *)lst->content;
		ft_sprintf(asker, "heredoc(%s)", hd->label);
		len = 0;
		while (ft_strcmp(hd->buffer + len, hd->label) != 0)
			len = ask_concat(sh, asker, &hd->buffer);
		lst = lst->next;
	}
}

int
	sanitize(t_shell *sh)
{
	t_quoter	q;
	int			i;
	char		c;

	i = -1;
	q = (t_quoter) { .s = 0, .d = 0, .bslash = 0 };
	while (sh->input[++i])
	{
		if (sh->input[i] == ' ')
			continue;
		c = sh->input[i];
		if (c == '\\')
			q.bslash = i + 1;
		else if (c == '\'' && !q.d && q.bslash != i)
			q.s = !q.s;
		else if (c == '"' && !q.s && q.bslash != i)
			q.d = !q.d;
		else if (c == '<' && sh->input[i + 1] == '<'
			&& !q.s && q.bslash != i++)
			add_heredoc(sh, &i);
	}
	if (q.s)
		return (ask_to_close(sh, "quote"));
	else if (q.d)
		return (ask_to_close(sh, "dquote"));
	else if (c == '|')
		return (ask_to_close(sh, "pipe"));
	else if (q.bslash == i)
		return (ask_to_close(sh, ""));
	ask_heredocs(sh);
	return (SUC);
}

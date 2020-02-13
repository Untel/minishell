/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   match_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/13 00:11:58 by riblanc           #+#    #+#             */
/*   Updated: 2020/02/13 06:00:38 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		get_size_current_word(t_shell *sh, t_lst_in **tmp)
{
	int			old_i;
	int			i;

	i = sh->term.input->size;
	while (*tmp != NULL && --i > sh->term.pos_str)
		*tmp = (*tmp)->prev;
	old_i = i;
	while (*tmp && (*tmp)->next && (*tmp)->c != ' ')
	{
		*tmp = (*tmp)->next;
		++i;
	}
	if ((*tmp)->c == ' ' && --i)
		*tmp = (*tmp)->prev;
	return (i - old_i);
}


int		get_nmatch(t_shell *sh, char *str)
{
	DIR				*rep;
	struct dirent	*file;
	int				nb_elem;

	file = NULL;
	rep = NULL;
	nb_elem = 0;
	if ((rep = opendir(sh->dir)) == NULL)
		return (-1);
	while ((file = readdir(rep)) != NULL)
		if (match(file->d_name, str))
			++nb_elem;
	if (closedir(rep) == -1)
		return (-1);
	return (nb_elem);
}

void	print_list(t_shell *sh)
{
	int		pid;
	char	*av[2];
	char	**env;
	int		ret;

	if ((pid = fork()) == -1)
		return ;
	else if (pid > 0)
	{
		wait(NULL);
		ft_printf(MSG_PROMPT, sh->printed_dir);
		return ;
	}
	else
	{
		av[0] = "/bin/ls";
		av[1] = 0;
		if (!(env = convert_env_list(sh->env)))
			return ;
		write(1, "\n", 1);
		ret = execve("/bin/ls", av, env);
		free(env);
		exit(ret);
	}
}

void	handle_empty_line(t_shell *sh, t_lst_in **tmp)
{
	free_all(sh->term.input);
	sh->term.input = malloc(sizeof(t_data));
	init_lst(sh->term.input);
	add_empty(sh->term.input, '\0');
	sh->term.pos_str = 1;
	*tmp = sh->term.input->end;
}

void	add_str_to_lst(t_shell *sh, char *str, char *filename)
{
	t_lst_in	*tmp;
	int			offset;
	int			size;
	int			i;

	i = -1;
	while (++i < sh->term.old_s_in + sh->term.pos_str)
		write(1, "\e[D", 3);
	tmp = sh->term.input->end;
	i = -1;
	while (++i < sh->term.old_s_in && sh->term.input->size > 1)
		delone(sh->term.input, sh->term.pos_str + (sh->term.input->size > 2));
	ft_printf("%*c", i + 2, 127);
	if (sh->term.input->size == 1)
		handle_empty_line(sh, &tmp);
	size = get_size_current_word(sh, &tmp);
	offset = 0;
	while (str[offset] && offset < size)
		++offset;
	sh->term.old_s_in = ft_strlen(filename) - offset;
	offset -= (size < 0 || (size == 0 && (tmp->c == 32 || tmp->c == 0)));
	while (++offset < ft_strlen(filename))
		add_after(sh->term.input, filename[offset], sh->term.pos_str);
}

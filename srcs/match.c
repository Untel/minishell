/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   match.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/08 22:39:41 by riblanc           #+#    #+#             */
/*   Updated: 2020/02/12 07:44:28 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <ncurses.h>
#include <termios.h>

extern int	g_termx;

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

char	*get_current_word(t_shell *sh)
{
	t_lst_in	*tmp;
	char		*str;
	int			i_str;
	int			size;

	tmp = sh->term.input->end;
	size = get_size_current_word(sh, &tmp);
	if (!(str = malloc(sizeof(char) * (size + 3))))
		return (NULL);
	i_str = -1;
	while (++i_str <= size && tmp->prev)
	{
		str[i_str] = tmp->c;
		tmp = tmp->prev;
	}
	str[i_str] = '*';
	str[i_str + 1] = 0;
	return (str);
}

int		read_char(char buff[3])
{
	read(0, buff, 1);
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

void	add_str_to_lst(t_shell *sh, char *str, char *filename)
{
	t_lst_in	*tmp;
	int			offset;
	int			size;
	int			i;

	tmp = sh->term.input->end;
	i = -1;
	while (++i < sh->term.old_s_in && sh->term.input->size > 2)
		delone(sh->term.input, sh->term.pos_str + 1);
	size = get_size_current_word(sh, &tmp);
	offset = 0;
	while (str[offset] && offset < size)
		++offset;
	sh->term.old_s_in = ft_strlen(filename) - offset;
	offset -= (size < 0 || (size == 0 && (tmp->c == 32 || tmp->c == 0)));
	while (++offset < ft_strlen(filename))
		add_after(sh->term.input, filename[offset], sh->term.pos_str);
}

#define P1_READ		0
#define P2_WRITE	1
#define P2_READ		2
#define P1_WRITE	3

int		ft_count_char(char *str, char c)
{
	int		i;
	int		count;

	count = 0;
	i = -1;
	while (str[++i])
		if (str[i] == c)
			++count;
	return (count);
}

int		print_highlight(t_shell *sh, char *str, int nb_elem, int i)
{
	DIR				*rep;
	struct dirent	*file;
	int				j;
	int				size;

	int				pid;
	int				p[4];
	char			buff[4097];
	char			*tmp;
	int				ret;
	char			*in;
	int				pute[2];

	file = NULL;
	rep = NULL;
	j = 0;
	size = 0;
	pipe(p);
	pipe(p + 2);
	if ((pid = fork()) == -1)
		return (0);
	else if (pid == 0)
	{
		dup2(p[P2_WRITE], 1);
		close(p[P1_READ]);
		close(p[P1_WRITE]);
		in = NULL;
		if ((ret = read(p[P2_READ], buff, 4096)) > 0)
		{
			buff[ret] = 0;
			tmp = ft_strjoin(in, (const char *)buff);
			free(in);
			in = tmp;
		}
		pipe(pute);
		dup2(pute[0], 0);
		ft_putstr_fd(tmp, pute[1]);
		close(pute[1]);
		char	*av[] = {"/usr/bin/column", "-x", ft_strjoin("-c ", ft_itoa(g_termx)), NULL};
		execve("/usr/bin/column", av, convert_env_list(sh->env));
		exit(0);
	}
	else
	{
		close(p[P2_READ]);
		close(p[P2_WRITE]);
		if ((rep = opendir(sh->dir)) == NULL)
			return (-1);
		while ((file = readdir(rep)) != NULL)
		{
			if (match(file->d_name, str))
			{
				if (j % nb_elem == i % nb_elem)
				{
					add_str_to_lst(sh, str, file->d_name);
					ft_putstr_fd("\e[104m", p[P1_WRITE]);
					ft_putstr_fd(file->d_name, p[P1_WRITE]);
					ft_putstr_fd("\e[0m\n", p[P1_WRITE]);
				}
				else
				{
					ft_putstr_fd("\e[0;0m", p[P1_WRITE]);
					ft_putstr_fd(file->d_name, p[P1_WRITE]);
					ft_putstr_fd("\e[0m\n", p[P1_WRITE]);
				}
				++j;
			}
		}
		ft_putstr_fd("\n", p[P1_WRITE]);
		if (closedir(rep) == -1)
			return (-1);
		ret = read(p[P1_READ], buff, 4096);
		buff[ret] = 0;
		close(p[P1_READ]);
		close(p[P1_WRITE]);
		ft_printf("%s", buff);
		wait(NULL);
	}
	size = ft_count_char(buff, '\n');
	return (size);
}

int		print_match(t_shell *sh, char buff[3])
{
	int				size;
	char			*str;
	int				i;
	int				j;
	int				nb_elem;

	size = 0;
	str = get_current_word(sh);
	if ((nb_elem = get_nmatch(sh, str)) == -1)
	{
		free(str);
		return (0);
	}
	i = 0;
	sh->term.old_s_in = 0;
	while ((buff[0] == 9 || buff[0] == 10) && !sh->ctrl_c)
	{
		ft_printf("\n");
		size = print_highlight(sh, str, nb_elem, i - (buff[0] == 10));
		i += (buff[0] == 9);
		j = -1;
		while (++j <= size)
			ft_printf("\e[A");
		print_line(sh, &size);
		if (buff[0] == 10)
			break ;
		read_char(buff);
	}
	sh->ctrl_c = 0;
	sh->term.pos_str *= -1;
	print_line(sh, &size);
	free(str);
	return (size);
}

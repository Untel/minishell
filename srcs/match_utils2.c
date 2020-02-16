/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   match_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/13 22:27:42 by riblanc           #+#    #+#             */
/*   Updated: 2020/02/14 01:17:05 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

int		is_first_word(t_shell *sh)
{
	t_lst_in	*tmp;
	int			i;

	tmp = get_elem_by_pos(sh->term.input, sh->term.pos_str);
	i = sh->term.pos_str - 1;
	while (++i < sh->term.input->size && tmp->c != 32)
		tmp = tmp->next;
	if (i == sh->term.input->size)
		return (1);
	return (0);
}

int		is_match_bin(t_shell *sh, char *path, char *str, t_list **occur)
{
	DIR				*rep;
	struct dirent	*file;
	int				nmatch;

	nmatch = 0;
	if ((rep = opendir(path)) == NULL)
		return (-1);
	while ((file = readdir(rep)) != NULL)
		if (match(file->d_name, str))
		{
			ft_lstadd_back(occur,
					ft_lstnew(ft_strdup(file->d_name), sizeof(file->d_name)));
			++nmatch;
		}
	if (closedir(rep) == -1)
		return (-1);
	return (nmatch);
}

t_list	*get_nmatch_bin(t_shell *sh, char **paths, char *str)
{
	int		i;
	int		nmatch;
	int		ret;
	t_list	*occur;

	occur = NULL;
	nmatch = 0;
	i = -1;
	while (paths[++i])
		is_match_bin(sh, paths[i], str, &occur);
	return (occur);
}

void	free_occur(t_list *occur)
{
	free(occur->content);
}

int		match_bin(t_shell *sh, int i, t_list *occur, int nb_elem)
{
	t_list	*tmp;
	char	*str;
	int		j;
	int		size;

	str = get_current_word(sh);
	tmp = occur;
	j = 0;
	while (tmp)
	{
		if (j % nb_elem == i % nb_elem)
			add_str_to_lst(sh, str, tmp->content);
		size += ft_strlen(tmp->content);
		tmp = tmp->next;
		++j;
	}
	free(str);
	return (size);
}

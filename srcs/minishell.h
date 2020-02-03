/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/01 20:32:31 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/03 19:25:25 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "libft.h"
# include "get_next_line.h"
# include "ft_printf.h"
# include <sys/wait.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# define SUC 1
# define ERR -1


typedef struct	s_env
{
	char		*dir;
	int			stop;
}				t_env;

typedef struct	s_key
{
	char	*key;
	char	*value;
}				t_key;

int		prompt_line(t_env *env);
t_list	*create_env_list(char **envp);
void	print_lst_env(t_list *lst_env);
char	**convert_env_list(t_list *lst_env);
void	*set_value(t_list **lst_env, char *key, char *value);
void	free_env_array(char **envp);
void	free_env_var(t_list *lst_env);
void	free_env_list(t_list **env);
void	*get_value(t_list *env, char *key);

#endif

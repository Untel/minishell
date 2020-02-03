/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/01 20:32:31 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/03 22:58:23 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "libft.h"
# include "get_next_line.h"
# include "ft_printf.h"
# include <sys/wait.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/errno.h>
# include <dirent.h>
# define FALSE 0
# define SUC 1
# define ERR -1
# define MSG_ERROR		"💩  \033[0;31mError\033[0m %s\n"
# define MSG_404_CMD	"🤔  \033[0;33m%s\033[0m: command not found\n"
# define MSG_PROMPT		"🔥  \033[0;32m%s\033[0m$ "
# define MSG_EXIT		"🖐  \033[0;31mGood bye!\033[0m\n"
typedef struct	s_reader
{
	int 		simple_q;
	int			double_q;
	int			idx;
}				t_reader;

typedef struct	s_cmd
{
	char		*input;
	int			argc;
	char		**argv;
}				t_cmd;

typedef struct	s_shell
{
	char		*input;				//malloced
	int			stop;
	t_list		*cmds;
	char		dir[BUFFER_SIZE];
	t_list		*env;
}				t_shell;

typedef struct	s_key
{
	char	*key;
	char	*value;
}				t_key;


t_list	*create_env_list(char **envp);
int		exec_lines(t_shell *sh);
void	print_lst_env(t_list *lst_env);
char	**convert_env_list(t_list *lst_env);
void	*set_value(t_list **lst_env, char *key, char *value);
void	free_env_array(char **envp);
void	free_env_var(t_list *lst_env);
void	free_env_list(t_list **env);
void	*get_value(t_list *env, char *key);
void	clear_last_prompt(t_shell *sh);

void	err_shutdown(t_shell *sh, char *str);
int		change_directory(t_shell *sh, t_cmd *cmd);
/* Promt */
int		prompt_line(t_shell *sh);
int		sanitize_input(t_shell *sh);
int		ask_closing_quote(t_shell *sh);

/* Command handling */
t_cmd	*new_command(t_shell *sh);
char	*add_argument(t_cmd *cmd, char *str);
char	*add_arg_to_last_cmd(t_shell *sh, char *str);
void	free_command(t_list *lst);
#endif

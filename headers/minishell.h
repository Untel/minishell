/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/01 20:32:31 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/13 06:58:16 by riblanc          ###   ########.fr       */
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
# include <signal.h>
# include <dirent.h>
# include <termios.h>
# include <curses.h>
# include <term.h>
# include "list.h"
# define FALSE 0
# define SUC 1
# define MSG_ERROR		"💩  \033[1;31mError\033[0m %s\n"
# define MSG_404_CMD	"🤔  \033[1;33m%s\033[0m: command not found\n"
# define MSG_PROMPT		"🔥  \033[1;32m%s\033[0m » "
# define MSG_EXIT		"🖐  \033[1;31mGood bye!\033[0m\n"
# define PIPE_OUT		0
# define PIPE_IN		1
# define STDIN			0
# define STDOUT			1
# define STDERR			2

typedef struct	s_reader
{
	int			simple_q;
	int			double_q;
	int			idx;
}				t_reader;

typedef struct	s_read
{
	char		*buffer;
	int			index;
}				t_read;

typedef enum	e_operator
{
	NONE,
	OR,
	AND,
	PIPE,
	JOB,
	REDIR_IN,
	REDIR_OUT,
	REDIR_IN_END,
	REDIR_OUT_END,
}				t_operator;

typedef struct	s_cmd
{
	char		*input;
	int			argc;
	char		**argv;
	t_operator	op;
	int			pipe[2];
	struct s_cmd	*left;
	struct s_cmd	*right;
}				t_cmd;

typedef struct termios	t_termios;

typedef struct	s_term
{
	t_termios	term;
	t_termios	old_term;
	t_data		*input;
	int			pos_str;
	int			size_prt;
	int			old_s_in;
	int			tmp;
}				t_term;

typedef struct	s_shell
{
	char	*input;				//malloced
	int		stop;
	t_list	*cmds;
	char	dir[BUFFER_SIZE];
	char	printed_dir[BUFFER_SIZE];
	int		last_ret;
	t_list	*env;
	t_term	term;
	int		ctrl_c;
}				t_shell;

typedef struct	s_key
{
	char	*key;
	char	*value;
}				t_key;

t_list	*create_env_list(char **envp);
int		exec_lines(t_shell *sh);
int		ft_env(t_shell *sh, t_cmd *cmd);
char	**convert_env_list(t_list *lst_env);
void	*set_value(t_list **lst_env, char *key, char *value);
void	free_env_array(char **envp);
void	free_env_var(t_list *lst_env);
void	free_env_list(t_list **env);
void	*get_value(t_list *env, char *key, char *def);
void	clear_last_prompt(t_shell *sh);
void	unset_key(t_list **lst_env, char *key);
void	free_env_unset(void *content);
int		is_key_env_valid(char *key);
/* PATH management */
int		exec_bin(t_shell *sh, t_cmd *cmd);
int		unset_env(t_shell *sh, t_cmd *cmd);
void	err_shutdown(t_shell *sh, char *str);
int		change_directory(t_shell *sh, t_cmd *cmd);
int		export_env(t_shell *sh, t_cmd *cmd);
/* Promt */
int		prompt_line(t_shell *sh);
int		sanitize_input(t_shell *sh);
int		sanitize_input2(t_shell *sh);
int		format_directory(t_shell *sh);
// int		ask_closing_quote(t_shell *sh);
int		builtin_subprocess(t_shell *sh, t_cmd *cmd,
	int (*fn)(t_shell *sh, t_cmd *cmd));
/* Command handling */
t_cmd	*new_command(t_shell *sh, t_operator op);
char	*add_argument(t_cmd *cmd, char *str);
char	*add_arg_to_last_cmd(t_shell *sh, char *str);
void	free_command(t_list *lst);

/* input handling */
void	handle_arrows(char buff[3], t_term *term);
void	handle_backspace(char buff[3], t_term *term);
int		handle_ctrl_d(char buff[3], t_term *term);
void	handle_ctrl_u(t_term term);
void	handle_ctrl_c(t_term *term);

/* autocomplete utils */
void	print_line(t_shell *sh);
void	print_list(t_shell *sh);
int		get_nmatch(t_shell *sh, char *str);
char	*get_current_word(t_shell *sh);
int		get_size_current_word(t_shell *sh, t_lst_in **tmp);
void	add_str_to_lst(t_shell *sh, char *str, char *filename);

int		init_term(struct termios *s_termios, struct termios *s_termios_backup);
char	*read_input(t_shell *sh);
void	sigint_quit (int sig);
int		match(char *s1, char *s2);
int		print_match(t_shell *sh, char buff[3]);

#endif

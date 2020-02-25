/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/01 20:32:31 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/25 18:45:14 by adda-sil         ###   ########.fr       */
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
# include <fcntl.h>
# include <sys/errno.h>
# include <signal.h>
# include <dirent.h>
# include <termios.h>
# include <curses.h>
# include <term.h>
# include <ncurses.h>
# include "list.h"
# define FALSE 0
# define SUC 1
# define MSG_ERROR		"💩  \033[1;31mError\033[0m %s\n"
# define MSG_ERRORN		"💩  \033[1;31mError\033[0m %s"
# define MSG_404_CMD	"🤔  \033[1;33m%s\033[0m: command not found\n"
# define MSG_PROMPT		"🔥  \033[1;32m%s\033[0m » "
# define MSG_PROMPT_ERR	"🧨  \033[1;31m%s\033[0m » "
# define MSG_EXIT		"🖐  \033[1;31mGood bye!\033[0m\n"
# define PIPE_OUT		0
# define PIPE_IN		1
# define STDERR			2

typedef enum	e_mode
{
	ARGS,
	HEREDOC,
	IN_REDIR,
	OUT_REDIR,
	OUT_END_REDIR,
}				t_mode;

typedef struct	s_read
{
	char		*buffer;
	char		*input;
	int			index;
	t_mode		add_to;
}				t_read;

typedef struct	s_history
{
	char		*path;
	t_bilist	*elements;
	t_bilist	*index;
	char		*input;
}				t_hist;

typedef struct	s_quoter
{
	int			s;
	int			d;
	int			bslash;
	char		c;
	char		cc;
	int			i;
}				t_quoter;

typedef enum	e_operator
{
	PIPE,
	JOB,
	NONE,
	OR,
	AND,
}				t_operator;

typedef	struct	s_heredoc
{
	char		*label;
	char		*buffer;
}				t_heredoc;

typedef struct	s_cmd
{
	char			*input;
	int				argc;
	char			**argv;
	char			**env;
	t_operator		op;
	int				pipe[2];
	t_list			*redir_in;
	t_list			*redir_out;
	struct s_cmd	*left;
	struct s_cmd	*right;
}				t_cmd;

typedef struct termios	t_termios;

typedef struct	s_point
{
	int			x;
	int			y;
}				t_point;

typedef struct	s_term
{
	t_termios	term;
	t_termios	old_term;
	t_data		*input;
	int			pos_str;
	int			size_prt;
	int			old_s_in;
	int			tmp;
	t_point		pos;
}				t_term;

typedef struct	s_shell
{
	char	*input;
	int		stop;
	char	dir[BUFFER_SIZE];
	char	printed_dir[BUFFER_SIZE];
	int		last_ret;
	t_list	*cmds;
	t_list	*env;
	t_list	*heredocs;
	t_hist	history;
	int		hd_index;
	t_term	term;
	int		ctrl_c;
}				t_shell;

typedef struct	s_key
{
	char	*key;
	char	*value;
}				t_key;

typedef struct	s_redirect
{
	char	*filename;
	char	*value;
	int		pipe[2];
	t_mode	type;
}				t_redirect;

extern t_shell	g_sh;

/*
** Built ins
*/
int				ft_echo(t_shell *sh, t_cmd *cmd);
int				ft_pwd(t_shell *sh, t_cmd *cmd);
int				change_directory(t_shell *sh, t_cmd *cmd);
int				ft_env(t_shell *sh, t_cmd *cmd);

/*
** Environnement
*/
t_list			*create_env_list(char **envp);
char			**convert_env_list(t_list *lst_env);
void			*set_value(t_list **lst_env, char *key, char *value);
void			*get_value(t_list *env, char *key, char *def);
void			unset_key(t_list **lst_env, char *key);
int				is_key_env_valid(char *key);
int				unset_env(t_shell *sh, t_cmd *cmd);
int				export_env(t_shell *sh, t_cmd *cmd);

/*
** Memory utils/cleaning
*/
void			clear_last_prompt(t_shell *sh);
void			free_env_unset(void *content);
void			free_command(t_list *lst);
void			free_heredocs(t_list *lst);
void			free_history(t_list *element);
void			free_occur(t_list *occur);
void			free_env_array(char **envp);
void			free_env_var(t_list *lst_env);
void			free_env_list(t_list **env);
void			err_shutdown(t_shell *sh, char *str);

/*
**	Promt
*/
int				prompt_line(t_shell *sh);
int				format_directory(t_shell *sh);

/*
**	Command handling
*/
t_cmd			*new_command(t_shell *sh, t_operator op);
char			*add_argument(t_cmd *cmd, char *str);
void			add_arg_to_last_cmd(t_shell *sh, char *str, t_read *rd);

/*
**	Heredocs
*/
int				add_heredoc(t_shell *sh, int *i);
int				ask_heredocs(t_shell *sh);

/*
**	Input handling
*/
void			handle_arrows(char buff[3], t_term *term);
void			handle_backspace(char buff[3], t_term *term);
int				handle_ctrl_d(char buff[3], t_term *term);
void			handle_ctrl_u(t_term term);
void			handle_ctrl_c(t_term *term);

/*
**	Input sanitizing
*/
int				sanitize(t_shell *sh);
int				ask_concat(t_shell *sh, char *ask,
					char **place, char *stopif);

/*
**	Input parsing
*/
int				handle_double_quote(t_shell *sh, t_read *rd, int *i);
int				handle_simple_quote(t_shell *sh, t_read *rd, int *i);
int				handle_space(t_shell *sh, t_read *rd, int *i);
int				handle_separator(t_shell *sh, t_read *rd, int *i);
int				handle_redirections(t_shell *sh, t_read *rd, int *i);
int				is_cmd_separator(char c);
char			*replace_vars(t_shell *sh, char *str);
int				copy_from_idx(t_shell *sh, t_read *rd, int idx);
t_operator		get_operator(t_shell *sh, int *i);
int				parse_input(t_shell *sh);
int				exec_lines(t_shell *sh);

/*
**	Autocomplete utils
*/
int				match(char *s1, char *s2);
void			print_line(t_shell *sh);
void			print_list(t_shell *sh);
char			*get_current_word(t_shell *sh);
int				get_size_current_word(t_shell *sh, t_lst_in **tmp);
void			add_str_to_lst(t_shell *sh, char *str, char *filename);
int				is_first_word(t_shell *sh);
t_list			*get_nmatch_bin(t_shell *sh, char **paths, char *str);
int				match_bin(t_shell *sh, int i, t_list *occur, int nb_elem);
int				get_nmatch(t_shell *sh, char *str);
int				init_term(struct termios *s_termios,
					struct termios *s_termios_backup);
char			*read_input(t_shell *sh);

/*
**	Processus
*/
int				after_child_exec(t_shell *sh, t_cmd *cmd);
int				builtin_subprocess(t_shell *sh, t_cmd *cmd,
					int (*fn)(t_shell *sh, t_cmd *cmd));
int				exec_cmd(t_shell *sh, t_cmd *cmd,
					int (*fn)(t_shell *sh, t_cmd *cmd));
int				exec_bin(t_shell *sh, t_cmd *cmd);

/*
**	History
*/
int				init_history(t_shell *sh);
int				add_to_history(t_shell *sh);
void			print_history(t_shell *sh, t_term *term, int next);
int				persist_history(t_shell *sh);

/*
**	Redirections / Pipelines
*/
void			redirect_buffer(int from, int to);
void			run_redirect_in(t_shell *sh, t_cmd *cmd);
int				run_redirect_out(t_shell *sh, t_cmd *cmd);
int				after_redirect_out(t_shell *sh, t_cmd *cmd);
int				mount_pipes(t_shell *sh);

/*
**	signal utils
*/
void			sigint_quit (int sig);
void			sigint_void(int sig);
void			handle_winch(int sig);
int				get_termx(t_shell *sh, char **av, char **env);
int				print_match(t_shell *sh, char buff[3]);
#endif

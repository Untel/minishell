/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/01 20:32:31 by adda-sil          #+#    #+#             */
/*   Updated: 2020/05/15 21:23:04 by riblanc          ###   ########.fr       */
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
# include "list.h"
# define FALSE 0
# define SUC 1
# define ERR -1
# define MSG_ERROR		"\033[1;31mError\033[0m %s\n"
# define MSG_SYNTAX_ERR	"\033[1;31mError\033[0m syntax error near token '%c' at position %d\n"
# define MSG_ERRORN		"\033[1;31mError\033[0m %s"
# define MSG_CMDARG_ERR	"\033[1;31mError\033[0m command previous token '%c' at position %d has no arguments"
# define MSG_404_REDIR	"\033[1;31mError\033[0m redirection argument missing\n"
# define MSG_404_CMD	"\033[1;33m%s\033[0m: command not found\n"
# define MSG_ERR_REG	"\033[1;33m%s\033[0m: no match found for this pattern\n"
# define MSG_GEN_ERR	"\033[1;33m%s\033[0m: %s\n"
# define MSG_PROMPT		"\033[1;32m%s\033[0m » "
# define MSG_PROMPT_ERR	"\033[1;31m%s\033[0m » "
# define MSG_EXIT		"\033[1;31mGood bye!\033[0m\n"
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
	char		c;
	int			i;
	int			fd;
	int			ret;
}				t_read;

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
	t_data		*input;
	int			pos_str;
	int			pos_aff;
	int			size_prt;
	t_termios	term;
}				t_term;

typedef struct	s_alias
{
	 struct s_alias	*next;
	 char			*key;
	 char			*value;
}				t_alias;

typedef struct	s_shell
{
	char	*input;
	int		stop;
	int		exit_val;
	char	dir[BUFFER_SIZE];
	char	printed_dir[BUFFER_SIZE];
	int		last_ret;
	int		inline_fd;
	t_list	*cmds;
	t_list	*env;
	t_list	*heredocs;
	t_alias	*alias;
	int		hd_index;
	t_term	term;
	int		sub;
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
	int		fd;
	t_mode	type;
}				t_redirect;

typedef struct	s_wildcards_checker
{
	char		**paths;
	int			depth;
	int			max_depth;
	int			ret;
	t_list		*matchs;
}				t_wildcards_checker;

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
void			free_content(t_list *el);
void			free_env_array(char **envp);
void			free_env_var(t_list *lst_env);
void			free_env_list(t_list **env);
void			free_redirection(t_list *lst);
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
char			*add_argument_index(t_cmd *cmd, char *str, int index);
int				add_arg_to_last_cmd(t_shell *sh, char *str, t_read *rd);
void			add_redir(t_shell *sh, t_cmd *cmd, char *str, t_read *rd);
int				check_wildcards(t_shell *sh, t_cmd *cmd,
					t_read *rd, char *pattern);
int				check_matchs(t_wildcards_checker *wc, char *path,
					int depth, char *deep);

/*
**	Heredocs
*/
int				add_heredoc(t_shell *sh, int *i);
int				ask_heredocs(t_shell *sh);

/*
**	Input handling
*/
void			handle_arrows(char buff[6], t_term *term);
void			handle_right_arrow(char buff[6], t_term *term);
void			handle_left_arrow(char buff[6], t_term *term);
void			handle_backspace(char buff[6], t_term *term);
int				handle_ctrl_d(char buff[6], t_term *term);
void			handle_ctrl_u(t_term term);
void			handle_ctrl_c(t_term *term);
void			handle_home(t_shell *sh, char buff[6], t_term *term);
void			handle_end(t_shell *sh, char buff[6], t_term *term);
void			handle_ctrl_keys(char buff[6], t_term *term);
int				del_right(t_term *term);
void			handle_option_cases(t_shell *sh, char buff[6]);

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
char			*replace_tilde(t_shell *sh, char *str);
int				copy_from_idx(t_shell *sh, t_read *rd, int idx);
int				copy_to_cmd(t_shell *sh, t_read *rd, int *i);
t_operator		get_operator(t_shell *sh, int *i);
int				parse_input(t_shell *sh);
int				exec_lines(t_shell *sh);

/*
**	Autocomplete utils
*/

int				match(char *s1, char *s2);
int				ft_strslen(char **strs);

/*
**	Processus
*/
int				after_child_exec(t_shell *sh, t_cmd *cmd);
int				builtin_subprocess(t_shell *sh, t_cmd *cmd,
					int (*fn)(t_shell *sh, t_cmd *cmd));
int				exec_cmd(t_shell *sh, t_cmd *cmd,
					int (*fn)(t_shell *sh, t_cmd *cmd));
int				fork_exec(t_shell *sh, t_cmd *cmd, char *tmp[2], int nb);
int				test_dir(char *path, char *cmd);
int				exec_bin(t_shell *sh, t_cmd *cmd);
int				fe_init(t_shell *sh, t_cmd *cmd, char **paths);

/*
**	Redirections / Pipelines
*/
void			run_redirect_in(t_shell *sh, t_cmd *cmd);
int				redirect_buffer(int from, int to);
int				run_redirect_out(t_shell *sh, t_cmd *cmd);
int				after_redirect_out(t_shell *sh, t_cmd *cmd);
int				mount_pipes(t_shell *sh);

/*
**	signal utils
*/
void			sigint_quit (int sig);
void			sigint_void(int sig);
void			handle_winch(int sig);
void			init_child_signals(pid_t child);

/*
**	Global vars
*/

extern t_shell	g_sh;
extern int		g_termx;
extern int		g_termy;
extern int		g_resize;

/*
** Alias utils
*/
void			check_aliases(t_shell *sh, t_cmd *cmd);
t_alias			*load_alias(void);
char			**ft_split_chst(char *str, char *charset);

#endif

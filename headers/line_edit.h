/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_edit.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/20 00:27:00 by riblanc           #+#    #+#             */
/*   Updated: 2020/10/13 01:31:39 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LINE_EDIT_H
# define LINE_EDIT_H
# include <termios.h>
# include "list.h"
# include "libft.h"
# include "minishell.h"

/*
** History defines
*/

# define HPREV		0
# define HNEXT		1
# define H_NONE		0
# define H_SAVE		1
# define H_EMPTYL	2

/*
** read_input attributes
*/

# ifndef MULTI
#  define MULTI	1
# endif

struct	s_history
{
	t_list	*lst;
	t_list	*cursor_pos;
	int		len;
	int		index;
	char	*filename;
};

typedef struct s_history	t_history;

struct	s_select
{
	int		ret;
	int		sel;
	char	*str;
	t_list	*lst;
};

typedef struct s_select	t_select;

struct	s_complete
{
	char	*str;
	int		pos;
};

typedef struct s_complete	t_complete;

struct	s_nmatch
{
	struct dirent	*ep;
	DIR				*dp;
	int				ret;
	int				i;

};

typedef struct s_nmatch	t_nmatch;

struct	s_cplutils
{
	t_lst_in	*lstmp;
	char		**paths;
	char		**lst;
	char		*current_word;
	char		*tmp;
	int			nb_elem;
	int			i;
	int			ret;
};

typedef struct s_cplutils	t_cplutils;

struct	s_line
{
	char			*prompt;
	int				old_size;
	int				pos;
	int				old_pos;
	int				size_prompt;
	int				offset;
	int				edit;
	int				sel[2];
	int				maxrows;
	int				multi;
	char			buff[6];
	char			*copy_buff;
	t_data			*lst_input;
	struct termios	s_term;
	struct termios	s_term_backup;
	t_history		edit_history;
	char			*buf;
	char			seq[64];
	t_complete		complete;
	int				ret;
	char			*str;
	int				nb_res;

};

typedef struct s_line	t_line;

struct	s_ml
{
	int	offset;
	int	len;
	int	rows;
	int	pos;
	int	rpos;
	int	rpos2;
	int	col;
	int	old_rows;
	int	i;
};

typedef struct s_ml	t_ml;

extern t_history	g_history;
extern int			g_termx;
extern int			g_termy;
extern int			g_resize;

char	*read_input(t_shell *sh, char *prompt, int multi, int size_prompt);
int		init_term(struct termios *s_term, struct termios *s_term_b);
void	append_single_cmd(t_line *line, t_data *lst, int offset, int max);
void	append_multi_cmd(t_line *line, int max);
void	add_char(t_line *line);
int		get_term_size(char **av, char **env);
void	refresh_line(t_line *line, char *prompt, int edit);
void	refresh_single_line(t_line *line, int edit);
void	refresh_multi_line(t_line *line, char *prompt, int edit);
void	load_history(const char *filename, t_history *history);
void	history_pn(t_line *line, int sens, t_history *history);
int		history_save(const char *filename, char *cmd);
void	print_history(t_history *history);
void	add_history(t_history *history, char *cmd, int save, size_t pos);
void	free_history(t_history *history, int index);
void	save_cursor_pos(void);

void	append(char **s1, char *s2);
char	*free_input(t_line *line, int free_yank);
int		init_sline(t_line *line);
char	*linedit_notty(void);

void	select_mode(t_line *line, char *prompt);
int		handle_escape(t_line *line, char *prompt, int edit);
void	go_right(t_line *line);
void	handle_ctrlu(t_line *line);

/*
** handle functions
*/

void	handle_winch(int sig);
int		handle_input(t_shell *sh, t_line *line, char *prompt);
void	handle_escape_sp(t_line *line, int edit, int *ret);
void	handle_ctrll(t_line *line, char *prompt);
int		handle_ctrld(t_line *line);
void	handle_uarrow(t_line *line, int edit);
void	handle_darrow(t_line *line, int edit);
void	handle_ctrl_uarrow(t_line *line);
void	handle_ctrl_darrow(t_line *line, int edit);

/*
** select_utils
*/

char	*get_str_by_pos(t_line *line, int del);
void	past(t_line *line, int before);
void	handle_select_x(t_select *s_sel, t_line *line, char **yank, int maj);

/*
** auto_complete utils
*/

void	auto_complete(t_shell *sh, t_line *line);
char	**get_matchlist(t_nmatch utils, char **path, char *file);
char	**get_nmatch(t_shell *sh, char **path, char *file);
void	get_rest_word(t_line *line, char *match, char *elem);
int		ft_strslen(char **strs);
char	**split_path(char *str);
char	*get_current_word(t_line *line);
int		get_size_cword(t_lst_in *tmp);

#endif

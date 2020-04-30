/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_edit.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/20 00:27:00 by riblanc           #+#    #+#             */
/*   Updated: 2020/04/29 19:52:49 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LINE_EDIT_H
# define LINE_EDIT_H
# include <termios.h>
# include "list.h"
# include "libft.h"

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

struct	s_alias
{
	struct s_alias	*next;
	char			*key;
	char			*value;
};

typedef struct s_alias	t_alias;

struct	s_line
{
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

char	*read_input(char *prompt, int multi, int size_prompt);
int		init_term(struct termios *s_term, struct termios *s_term_b);
void	append_single_cmd(t_line *line, t_data *lst, int offset, int max);
void	append_multi_cmd(t_line *line, int max);
void	handle_winch(int sig);
int		handle_input(t_line *line, char *prompt);
void	handle_escape_sp(t_line *line, int edit, int *ret);
void	handle_ctrll(t_line *line, char *prompt);
int		handle_ctrld(t_line *line);
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
** select_utils
*/

char	*get_str_by_pos(t_line *line, int del);
void	past(t_line *line, int before);
void	handle_select_x(t_select *s_sel, t_line *line, char **yank, int maj);
char	*check_aliases(char *str);

/*
** alias_utils
*/

void	join_alias(char *alias, char **str, int i[3]);
void	free_alias_lst(t_alias **alias, char *except, char **str);
char	*ft_strsjoin(int size, char **strs, char *sep);
char	**ft_split_chst(char *str, char *charset);

#endif
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 11:09:50 by marboccu          #+#    #+#             */
/*   Updated: 2024/06/23 12:32:18 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <libft.h>
# include <ms_parsing.h>

# ifndef PROGNAME
#  define PROGNAME "minishell"
# endif

# ifndef PROG_PROMPT
#  define PROG_PROMPT "minishell>"
# endif

# ifndef HEREDOC_PROMPT
#  define HEREDOC_PROMPT "heredoc>"
# endif

# ifndef PROMPT
#  define PROMPT "> "
# endif

# ifndef CWD_INITIAL_SIZE
#  define CWD_INITIAL_SIZE 4096
# endif

extern t_uchar	g_status;

typedef struct s_main
{
	int		argc;
	char	**envp;
	char	**argv;
}			t_main;

typedef struct s_command
{
	t_list		*args;

	t_list		*in_redirects;
	t_list		*out_redirects;

	int			in_fd;
	int			out_fd;

	char		*last_heredoc_file;
}	t_command;

/**
 * @brief "GLOBAL" VARIABLE FOR THE CURRENT PROJECT.
 * USEFUL FOR WRAPPER FUNCTIONS: IF ANY ERROR HAPPENS,
 * THIS STRUCTURE WILL BE FREED (with `cleanup`).
 * ANY FUNCTION THAT HAS THIS *t_var will probably call cleanup on errors.
 */
typedef struct s_var
{
	t_list				*env;
	char				*home_path;

	// -------------PWD-------------
	char				*curr_path;
	uint64_t			curpath_len;
	// -------------INPUT-----------
	t_list				*history;
	// -------------EXECUTION-------
	t_list				*all_cmds;
	int					pipes[2][2];
	int					orig_stdin;
	int					orig_stdout;
	//------------------------------
	char				**script_content;

	t_main				_main;
}			t_var;

int		ms_init(t_var *mshell);

void	ms_prompt(t_var *mshell);
void	ms_exec_script(t_var *mshell);
void	handle_sig(int signal);

//-------------------------BUILTIN----------------------------------------------
t_state	ms_unset(t_var *mshell, t_list *args);
t_state	ms_exit(t_var *mshell, t_list *args);
t_state	ms_pwd(t_var *mshell, t_list *args);
t_state	ms_export(t_var *mshell, t_list *args);
t_state	ms_env(t_var *mshell, t_list *args);
t_state	ms_cd(t_var *mshell, t_list *args);
t_state	ms_history(t_var *mshell, t_list *args);
t_state	ms_echo(t_var *mshell, t_list *args);
//-------------EXTRA-------------------
void	print_history(t_list *history);
//-----------------------------------------------------------------------------

t_state	ms_in_redir(t_command *cmd, int *fd);
t_state	ms_rediout(t_command *cmds);
t_state	ms_inredir_handle(t_var *mshell, t_command *cmds);
char	*heredoc_expand(t_var *mshell, char **arg);
char	*gen_heredocs(t_var *mshell, int count);

t_state	ms_wrap_commands(t_var *mshell);
t_state	ms_exec_commands(t_var *mshell, t_list *all);
t_state	ms_exec_cmd(t_var *mshell, t_list *args);
t_state	ms_exec_update_stds(t_var *mshell, t_command *command, int idx);

//-------------------------UTILS------------------------------------------------
t_uchar	g_set_status(t_uchar status);

int		cleanup(t_var *g, bool shouldexit, int status);

int		reset_stds(t_var *mshell);
void	*clean_cmds(t_list *cmds, bool free_content);
//-------------CHECKS------------------
bool	ms_is_builtin(char *cmd);
//-----------CONVERTERS----------------
char	**lst_env_to_mtx(t_var *mshell);
t_list	*ms_split_pipelines(t_list *all);
//-------------------------------------
t_state	add_history_line(t_var *mshell, char *cmd);
t_state	ms_update_cwd(t_var *mshell);
//------------------------------------------------------------------------------

//-------------------------EXPERIMENTAL-----------------------------------------
char	*pre_parse_expand(t_var *mshell, char **line);
//------------------------------------------------------------------------------

#endif

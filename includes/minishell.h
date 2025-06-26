/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 11:09:50 by marboccu          #+#    #+#             */
/*   Updated: 2024/08/10 09:18:50 by odudniak         ###   ########.fr       */
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

extern int	g_lastsig;

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
	int					**pipes;
	pid_t				last_cmd_pid;
	int					orig_stdin;
	int					orig_stdout;
	//------------------------------
	char				**script_file;

	t_uchar				statuscode;

	t_main				_main;
}			t_var;

int		ms_init(t_var *mshell);
void	close_pipes(t_var *mshell);
void	*kill_pipes(t_var *mshell);
t_state	ms_prompt(t_var *mshell);
t_state	ms_exec_script(t_var *mshell);

//---------------------SIGNAL-HANDLERS------------------------------------------
void	handle_sig(int signal);
void	handle_child_ret_sig(int signal);
void	handle_exec_sig(int signal);
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

t_state	ms_in_redir(t_var *mshell, t_command *cmd, int *fd);
t_state	ms_rediout(t_var *mshell, t_command *cmds);
t_state	ms_inredir_handle(t_var *mshell, t_command *cmds);
char	*heredoc_expand(t_var *mshell, char **arg);
char	*gen_heredocs(t_var *mshell, int count);

t_state	ms_wrap_commands(t_var *mshell);
t_state	ms_exec_commands(t_var *mshell, t_list *all);
t_state	ms_exec_cmd(t_var *mshell, t_list *args);
t_state	ms_exec_update_stds(t_var *mshell, t_command *command, int idx);

//-------------------------UTILS------------------------------------------------
int		g_setlastsig(int sig);
t_state	track_lastsig(t_var *mshell);
t_uchar	setstatus(t_var *mshell, t_uchar code);

int		cleanup(t_var *g, bool shouldexit, int status);

int		reset_stds(t_var *mshell);
void	*clean_cmds(t_list *cmds, bool free_content);
//-------------CHECKS------------------
bool	is_builtin(char *cmd);
bool	is_known_token(char *value);
bool	is_node_token(t_list *node, char *expected);
//-----------CONVERTERS----------------
char	**lst_env_to_mtx(t_var *mshell);
t_list	*ms_split_pipelines(t_list *all);
//-------------------------------------
t_state	add_history_line(t_var *mshell, char *cmd);
t_state	ms_update_cwd(t_var *mshell);
//-----------FILE-UTILS----------------
/**
 * @brief Find the absolute path of a `cmd` command.
 * @param paths array of path where to lookup the given `cmd`
 * @param cmd the requested command
 * @return absolute path of the `cmd` or if it doesn't exist a `NULL`
 * @attention Uses: `malloc`
 *  `free`
 */
char	*sys_findcmdpath(t_var *mshell, char **paths, char *cmd);
//------------------------------------------------------------------------------

//-------------------------EXPERIMENTAL-----------------------------------------
char	*pre_parse_expand(t_var *mshell, char **line);
//------------------------------------------------------------------------------

#endif

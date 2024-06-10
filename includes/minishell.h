/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marboccu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 11:09:50 by marboccu          #+#    #+#             */
/*   Updated: 2024/06/10 18:12:57 by marboccu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <libft.h>
# include <ms_parsing.h>
# include <ms_exec.h>

extern t_uchar	g_status;

# ifndef PROGNAME
#  define PROGNAME "minishell"
# endif

# ifndef PROMPT
#  define PROMPT "minishell>"
# endif

# ifndef CWD_INITIAL_SIZE
#  define CWD_INITIAL_SIZE 4096
# endif


typedef struct s_main
{
	char	**envp;
}			t_main;

typedef struct s_command
{
	t_list		*args;

	t_list		*in_redirects;
	t_list		*out_redirects;

	char		*in_file;
	char		*out_file;

}	t_command;

/**
 * @brief "GLOBAL" VARIABLE FOR THE CURRENT PROJECT.
 * USEFUL FOR WRAPPER FUNCTIONS: IF ANY ERROR HAPPENS,
 * THIS STRUCTURE WILL BE FREED (with `cleanup`).
 * ANY FUNCTION THAT HAS THIS *t_var will probably call cleanup on errors.
 */
typedef struct s_var
{
	t_uchar				*status_code;

	t_list				*env;
	char				*home_path;

	// ----START----PWD-------------
	char				*curr_path;
	uint64_t			curpath_len;
	// -----END-----PWD-------------


	// ----START----INPUT-----------
	char				*last_input;
	t_list				*history;
	// ----END------INPUT-----------

	// ----START----EXECUTION-------
	t_list				*all_cmds;
	// ----END------EXECUTION-------

	t_main				_main;
}			t_var;

int		ms_loadenv(t_var *mshell);
void	ms_init(t_var *mshell);
void	ms_prompt(t_var *mshell);

int		cleanup(t_var *g, bool shouldexit, int status);

int		ms_unset(t_var *mshell, t_list *args);

//-------------------------BUILTIN----------------------------------------------
int		ms_exit(t_var *mshell, t_list *args);
void	*ms_pwd(t_var *mshell, t_list *args);
int		ms_export(t_var *mshell, t_list *args);
void	ms_env(t_var *mshell, t_list *args);
int		ms_cd(t_var *mshell, t_list *args);
int		ms_history(t_var *mshell, t_list *args);
int		ms_echo(t_var *mshell, t_list *args);

void	add_cmd_history(t_var *mshell, char *cmd);

void	print_history(t_list *history);
//-----------------------------------------------------------------------------
void	*ms_update_cwd(t_var *mshell);
int		ms_exec_cmd(t_var *mshell, t_list *args);

void	*freeallcmds(t_list *cmds, bool free_content);

bool	ms_is_builtin(char *cmd);

int		ms_heredoc(t_var *mshell, t_command *cmds);
char	*heredoc_expand(t_var *mshell, char **arg);
char	*gen_heredocs(int count);

#endif

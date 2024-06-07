/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marboccu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 11:09:50 by marboccu          #+#    #+#             */
/*   Updated: 2024/06/07 10:52:10 by marboccu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <libft.h>
# include <ms_parsing.h>

# define PROGNAME "minishell"
# define PROMPT "minishell>"
# define CWD_INITIAL_SIZE 4096

extern t_uchar	g_status;

typedef struct s_main
{
	char	**envp;
}			t_main;

typedef struct s_command
{
	t_list		*args;

	t_list		*in_redirects;
	t_list		*out_redirects;
	t_list		*heredocs;
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
	t_list				*history;
	t_main				_main;
	char				**cmds_paths;
	char				*curr_path;
	char				*home_path;
	uint64_t			curpath_len;
	t_uchar				*status_code;

	char				*last_input;
	t_list				*all_cmds;
	t_list 				*heredoc;
	char *heredoc_file;
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


void	*ms_exec_commands(t_var *mshell, t_list *all);

void	add_cmd_history(t_var *mshell, char *cmd);

void	print_history(t_list *history);
//-----------------------------------------------------------------------------
void	*sys_update_cwd(t_var *mshell);
int		ms_exec_cmd(t_var *mshell, t_list *args);

void	*freeallcmds(t_list *cmds, bool free_content);

bool	ms_is_builtin(char *cmd);

int ms_heredoc(t_var *mshell, t_command *cmds);
//char	**ft_lst_to_array(t_list *lst);
bool	has_heredoc(t_list *cmd_list);
void cleanup_heredocs(t_command *cmd);
void unlink_heredocs(t_command *cmd);
char	*heredoc_expand(t_var *mshell, char **arg);
char *gen_heredocs(int count);
#endif

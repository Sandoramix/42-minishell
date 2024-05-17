/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marboccu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 11:09:50 by marboccu          #+#    #+#             */
/*   Updated: 2024/05/17 16:51:03 by marboccu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <libft.h>
# include <ms_parsing.h>

# define PROGNAME "minishell"
# define PROMPT "minishell>"
# define PATH_MAX 4096

typedef struct s_main
{
	char	**envp;
}			t_main;

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
}			t_var;

int		ms_loadenv(t_var *mshell);
void	ms_init(t_var *mshell);
void	ms_prompt(t_var *mshell);

int		cleanup(t_var *g, bool shouldexit, int status);

int		ms_unset(t_var *mshell, t_list *args);

int	ms_exit(t_var *mshell, t_list *args);

void	*ms_pwd(t_var *mshell, t_list *args);
int		ms_export(t_var *mshell, t_list *args);
void	ms_env(t_var *mshell, t_list *args);
int		ms_cd(t_var *mshell, t_list *args);
int		ms_history(t_var *mshell, t_list *args);
int ms_echo(t_var *mshell, t_list *args);

void	parse_and_exec(t_var *mshell, char *input);
void	add_cmd_history(t_var *mshell, char *cmd);
void	print_history(t_list *history);
//-----------------------------------------------------------------------------
void	*sys_update_cwd(t_var *mshell);

#endif

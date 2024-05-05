/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 11:09:50 by marboccu          #+#    #+#             */
/*   Updated: 2024/05/05 14:29:15 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <libft.h>
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
	t_main				_main;
	char				**cmds_paths;
	char				*curr_path;
	uint64_t			curpath_len;
	t_uchar				*status_code;
}			t_var;

int		ms_loadenv(t_var *mshell);
void	ms_init(t_var *mshell);
void	ms_prompt(t_var *mshell);

int		cleanup(t_var *g, bool shouldexit, int status);

void	ms_unset(t_var *mshell, char **args);

void	ms_exit(t_var *mshell, char **args);

void	*ms_pwd(t_var *mshell);
int		ms_export(t_var *mshell, t_list *args);

//-----------------------------------------------------------------------------
//---------------------------PARSING-------------------------------------------

// TODO REMOVE
/**
# DEPRECATED
 * @brief Parse the string of a command.
 * It even handles internal quotes.
 * @param command command's string to parse
 * @return Array of args parsed.
 */
char	**cmd_parse(char *command);
char	**expand_and_clear_mtx(char **args);

t_list	*cmd_parse_new(char *raw);

char	*str_clearquotes(char **str);

t_list	*expand_and_clear(t_list *args);
#endif

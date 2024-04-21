/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 11:09:50 by marboccu          #+#    #+#             */
/*   Updated: 2024/04/21 14:53:23 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <libft.h>
#include <readline/readline.h>
# define PROMPT "minishell>"

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
	t_list	*env;
	t_main	_main;
	char	**cmds_paths;
	int		*status_code;
}			t_var;

int	ms_loadenv(t_var *mshell);
void ms_init(t_var *mshell);
void ms_prompt(t_var *mshell);

void	ms_unset(t_var *mshell, char **args);

#endif

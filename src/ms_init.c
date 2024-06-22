/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 12:33:15 by marboccu          #+#    #+#             */
/*   Updated: 2024/06/22 11:11:27 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int	ms_loadenv(t_var *mshell)
{
	t_list	*home_node;

	mshell->env = env_load(mshell->_main.envp);
	if (!mshell->env)
		return (pf_errcode(E_ENV_LOAD), cleanup(mshell, true, 1), KO);
	home_node = lst_findbykey_str(mshell->env, "HOME");
	if (home_node)
	{
		mshell->home_path = str_dup(home_node->val);
		if (!mshell->home_path && home_node->val)
			return (pf_errcode(E_ENV_LOAD), cleanup(mshell, true, 1));
	}
	return (OK);
}

int	ms_init(t_var *mshell)
{
	ms_loadenv(mshell);
	mshell->orig_stdout = dup(STDOUT_FILENO);
	mshell->orig_stdin = dup(STDIN_FILENO);
	if (mshell->orig_stdin == -1 || mshell->orig_stdin == -1)
		return (pf_errcode(E_DUP), cleanup(mshell, true, 1));
	mshell->curpath_len = CWD_INITIAL_SIZE;
	mshell->curr_path = ft_calloc(CWD_INITIAL_SIZE, sizeof(char));
	if (!mshell->curr_path)
		return (pf_errcode(E_MALLOC), cleanup(mshell, true, 1));
	ms_update_cwd(mshell);
	return (OK);
}

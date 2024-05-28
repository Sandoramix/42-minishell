/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marboccu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 12:33:15 by marboccu          #+#    #+#             */
/*   Updated: 2024/05/28 12:24:02 by marboccu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ms_loadenv(t_var *mshell)
{
	t_list	*home_node;

	mshell->env = env_load(mshell->_main.envp);
	if (!mshell->env)
		return (pf_errcode(ERR_ENV_LOAD), cleanup(mshell, true, 1), KO);
	home_node = lst_findbykey_str(mshell->env, "HOME");
	if (home_node)
	{
		mshell->home_path = str_dup(home_node->val);
		if (!mshell->home_path && home_node->val)
			return (pf_errcode(ERR_ENV_LOAD), cleanup(mshell, true, 1));
	}
	mshell->cmds_paths = env_load_paths(mshell->env);
	return (OK);
}

void	ms_init(t_var *mshell)
{
	ms_loadenv(mshell);
	mshell->status_code = ft_calloc(1, sizeof(unsigned char));
	if (!mshell->status_code)
		cleanup(mshell, true, 1);
	//dbg_printf("HOME PATH: %s\n", mshell->home_path);
	mshell->curpath_len = CWD_INITIAL_SIZE;
	mshell->curr_path = ft_calloc(CWD_INITIAL_SIZE, sizeof(char));
	if (!mshell->curr_path)
	{
		pf_errcode(ERR_MALLOC);
		cleanup(mshell, true, 1);
	}
	sys_update_cwd(mshell);
}

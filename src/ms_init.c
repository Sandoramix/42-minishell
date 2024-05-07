/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marboccu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 12:33:15 by marboccu          #+#    #+#             */
/*   Updated: 2024/04/30 17:42:34 by marboccu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ms_loadenv(t_var *mshell)
{
	mshell->env = env_load(mshell->_main.envp);
	if (!mshell->env)
		return (pf_errcode(ERR_ENV_LOAD), cleanup(mshell, true, 1));
	while (mshell->env)
	{
		if (str_ncmp((char *)mshell->env->key, "HOME", 5) == 0)
		{
			if (mshell->home_path)
				free(mshell->home_path);
			mshell->home_path = str_dup((char *)mshell->env->val);
			if (!mshell->home_path)
				return (pf_errcode(ERR_MALLOC), cleanup(mshell, true, 1));
		}
		mshell->env = mshell->env->next;
	}
	ft_printf("HOME: %s\n", mshell->home_path);
	mshell->cmds_paths = env_load_paths(mshell->env);
	return (0);
}

void	ms_init(t_var *mshell)
{
	ms_loadenv(mshell);
	mshell->status_code = ft_calloc(1, sizeof(unsigned char));
	if (!mshell->status_code)
		cleanup(mshell, true, 1);
	mshell->curpath_len = PATH_MAX;
	mshell->curr_path = ft_calloc(PATH_MAX, sizeof(char));
	if (!mshell->curr_path)
	{
		pf_errcode(ERR_MALLOC);
		cleanup(mshell, true, 1);
	}
}

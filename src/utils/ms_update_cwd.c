/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_update_cwd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marboccu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 09:15:20 by odudniak          #+#    #+#             */
/*   Updated: 2024/06/13 23:41:57 by marboccu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void ms_update_env(t_list *env, char *key, char *new_val)
{
	t_list *current;

	current = env;
	while (current)
	{
		if (str_equals(current->key, key))
	{
		free(current->val);
		current->val = str_dup(new_val);
		return ;
	}
		current = current->next;
	}
}

void	*ms_update_cwd(t_var *mshell)
{
	char	*pwd;

	pwd = getcwd(mshell->curr_path, mshell->curpath_len);
	if (!pwd && errno == ERANGE)
	{
		mshell->curpath_len *= 2;
		free(mshell->curr_path);
		mshell->curr_path = ft_calloc(mshell->curpath_len, sizeof(char));
		if (!mshell->curr_path)
			return (pf_errcode(E_MALLOC), cleanup(mshell, true, 1), NULL);
		return (ms_update_cwd(mshell), NULL);
	}
	
	ms_update_env(mshell->env, "PWD", mshell->curr_path);
	return (NULL);
}

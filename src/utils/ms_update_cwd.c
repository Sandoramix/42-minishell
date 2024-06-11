/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_update_cwd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marboccu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 09:15:20 by odudniak          #+#    #+#             */
/*   Updated: 2024/06/11 23:47:13 by marboccu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

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
			return (pf_errcode(ERR_MALLOC), cleanup(mshell, true, 1), NULL);
		return (ms_update_cwd(mshell), NULL);
	}
	return (NULL);
}

// void *ms_update_oldpwd(t_var *mshell)
// {
// 	t_list *oldpwd;

// 	oldpwd = lst_findbykey_str(mshell->env, "PWD");
// 	return NULL;
// }

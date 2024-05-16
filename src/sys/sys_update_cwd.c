/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_update_cwd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 09:15:20 by odudniak          #+#    #+#             */
/*   Updated: 2024/05/16 09:17:26 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	*sys_update_cwd(t_var *mshell)
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
		return (sys_update_cwd(mshell), NULL);
	}
	return (NULL);
}

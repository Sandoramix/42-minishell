/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 14:36:35 by marboccu          #+#    #+#             */
/*   Updated: 2024/04/24 11:26:38 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	*ms_pwd(t_var *mshell)
{
	char		*pwd;

	pwd = getcwd(mshell->curr_path, mshell->curpath_len);
	if (!pwd && errno == ERANGE)
	{
		mshell->curpath_len *= 2;
		free(mshell->curr_path);
		mshell->curr_path = ft_calloc(mshell->curpath_len, sizeof(char));
		if (!mshell->curr_path)
			return (pf_errcode(ERR_MALLOC), cleanup(mshell, true, 1), NULL);
		return (ms_pwd(mshell), NULL);
	}
	printf("%s\n", mshell->curr_path);
	return (NULL);
}
/*
int	main(int ac, char **av, char **envp)
{
	t_var			mshell;

	(void)av;
	(void)ac;
	mshell = (t_var){0};
	if (ac != 1)
		return (pf_errcode(ERR_INVALID_ARGC), cleanup(&mshell, true, 1), 1);
	mshell._main.envp = envp;
	ms_init(&mshell);
	ms_pwd(&mshell);
	return (cleanup(&mshell, true, 0));
}
*/
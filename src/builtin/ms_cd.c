/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marboccu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 17:37:44 by marboccu          #+#    #+#             */
/*   Updated: 2024/04/30 18:41:09 by marboccu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void ms_cd(t_var *mshell, char **args)
{
	int len;
	struct stat buf;

	len = str_mtxlen(args);
	if (len > 2)
	{
		ft_fprintf(2, "cd: too many arguments\n");
		*mshell->status_code = 1;
		return ;
	}
	else if (len == 1)
		chdir(mshell->home_path);
	else if (len == 2)
	{
		if (stat(args[1], &buf) != 0)
		{
			ft_fprintf(2, "cd: %s: No such file or directory\n", args[1]);
			*mshell->status_code = 1;
			return ;
		}
		chdir(args[1]);
		mshell->home_path = args[1];
	}
	ft_printf("cd: %s\n", mshell->home_path);
	*mshell->status_code = 0;
}

int main(int ac, char **av, char **envp)
{
	t_var mshell;

	av = (char *[]){"cd",  NULL};
	mshell = (t_var){0};
	if (ac != 1)
		return (pf_errcode(ERR_INVALID_ARGC), cleanup(&mshell, true, 1), 1);
	mshell._main.envp = envp;
	ms_init(&mshell);
	ms_pwd(&mshell);
	ms_cd(&mshell, av);
	ms_pwd(&mshell);
	return (cleanup(&mshell, true, 0));
}

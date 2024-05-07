/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marboccu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 17:37:44 by marboccu          #+#    #+#             */
/*   Updated: 2024/05/07 13:02:51 by marboccu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void ms_cd(t_var *mshell, t_list *args)
{
	int len;
	struct stat buf;

	len = lst_size(args);
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
		if (stat(args->val, &buf) != 0)
		{
			ft_fprintf(2, "%s: No such file or directory\n", args->val);
			*mshell->status_code = 1;
			return ;
		}
		chdir(args->val);
		mshell->home_path = args->val;
	}
	ft_printf("cd: %s\n", mshell->home_path);
	*mshell->status_code = 0;
}

// int main(int ac, char **av, char **envp)
// {
// 	t_var mshell;
// 	t_list *args;
// 	int len;

// 	av = (char *[]){"cd", "test", NULL};
// 	mshell = (t_var){0};
// 	if (ac != 1)
// 		return (pf_errcode(ERR_INVALID_ARGC), cleanup(&mshell, true, 1), 1);
// 	mshell._main.envp = envp;
// 	ms_init(&mshell);
// 	args = lst_new(av, NULL);
// 	len = lst_size(args);
// 	ms_pwd(&mshell, args);
// 	ms_cd(&mshell, args);
// 	ms_pwd(&mshell, args);
// 	return (cleanup(&mshell, true, 0));
// }

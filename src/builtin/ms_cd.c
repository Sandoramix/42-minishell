/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marboccu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 17:37:44 by marboccu          #+#    #+#             */
/*   Updated: 2024/05/12 11:22:01 by marboccu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

// TODO: fix pwd quando lancio il comando cd con un solo argomento
int ms_cd(t_var *mshell, t_list *args)
{
	int len;
	struct stat buf;
	t_list *current;

	len = lst_size(args);
	current = lst_findbykey_str(mshell->env, "HOME");
	ft_printf("current: %s\n", current->val);
	if (len > 2)
	{
		ft_fprintf(2, "cd: too many arguments\n");
		*mshell->status_code = 1;
		return (KO);
	}
	else if (len == 1)
		chdir(mshell->home_path);
	else if (len == 2)
	{
		if (stat(args->next->val, &buf) != 0)
		{
			ft_fprintf(2, "%s: No such file or directory\n", args->val);
			*mshell->status_code = 1;
			return (KO);
		}
		chdir(args->next->val);
	}
	if (len == 1)
		chdir(mshell->home_path);
	else
		chdir(args->next->val);
	
	ft_printf("cd: %s\n", mshell->home_path);
	*mshell->status_code = 0;
	return (OK);
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

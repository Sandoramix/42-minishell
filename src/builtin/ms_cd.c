/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marboccu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 17:37:44 by marboccu          #+#    #+#             */
/*   Updated: 2024/05/17 16:58:47 by marboccu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>


int	ms_cd(t_var *mshell, t_list *args)
{
	int			len;
	struct stat	buf;
	t_list		*current_home;

	len = lst_size(args);
	if (len > 2)
	{
		*mshell->status_code = 1;
		return (ft_perror("cd: too many arguments\n"), KO);
	}
	else if (len == 1)
	{
		current_home = lst_findbykey_str(mshell->env, "HOME");
		if (current_home)
		{
			free(mshell->home_path);
			mshell->home_path = str_dup(current_home->val);
			if (!mshell->home_path)
				return (pf_errcode(ERR_MALLOC), cleanup(mshell, true, 1));
		}
		if (!mshell->home_path)
			printf("%s\n", mshell->curr_path);
		else
			chdir(mshell->home_path);
		sys_update_cwd(mshell);
		return (OK);
	}
	if (stat(args->next->val, &buf) != 0)
	{
		ft_fprintf(2, "%s: No such file or directory\n", args->val);
		*mshell->status_code = 1;
		return (KO);
	}
	chdir(args->next->val);
	sys_update_cwd(mshell);
	dbg_printf("cd: %s -> %s\n", args->next->val, mshell->curr_path);
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

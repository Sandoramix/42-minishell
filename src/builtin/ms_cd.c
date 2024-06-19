/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 17:37:44 by marboccu          #+#    #+#             */
/*   Updated: 2024/06/19 22:17:13 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	cd_update_oldpwd(t_var *mshell)
{
	t_list		*pwd;

	pwd = lst_findbykey_str(mshell->env, "PWD");
	if (pwd)
		lst_upsert_str(&mshell->env, "OLDPWD", pwd->val);
}

static int	ms_cd_home(t_var *mshell)
{
	t_list		*current_home;

	current_home = lst_findbykey_str(mshell->env, "HOME");
	if (current_home)
	{
		free(mshell->home_path);
		mshell->home_path = str_dup(current_home->val);
		if (!mshell->home_path)
			return (pf_errcode(E_MALLOC), cleanup(mshell, true, 1));
	}
	cd_update_oldpwd(mshell);
	if (!mshell->home_path)
	{
		*mshell->status_code = 1;
		ft_perror("cd: HOME is not set\n", mshell->curr_path);
	}
	else
		chdir(mshell->home_path);
	ms_update_cwd(mshell);
	dbg_printf(CCYAN"[cd]:\t[]->%s\n"CR, mshell->curr_path);
	return (OK);
}

int	ms_cd(t_var *mshell, t_list *args)
{
	int			len;
	struct stat	buf;

	len = lst_size(args);
	if (len > 2)
	{
		*mshell->status_code = 1;
		return (ft_perror("cd: too many arguments\n"), KO);
	}
	else if (len == 1)
		return (ms_cd_home(mshell));
	if (stat(args->next->val, &buf) != 0)
	{
		ft_perror("cd: %s: no such file or directory\n", args->next->val);
		*mshell->status_code = 1;
		return (KO);
	}
	cd_update_oldpwd(mshell);
	chdir(args->next->val);
	ms_update_cwd(mshell);
	dbg_printf(CCYAN"[cd]:\t[%s]->%s\n"CR, args->next->val, mshell->curr_path);
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
// 		return (pf_errcode(E_INVALID_ARGC), cleanup(&mshell, true, 1), 1);
// 	mshell._main.envp = envp;
// 	ms_init(&mshell);
// 	args = lst_new(av, NULL);
// 	len = lst_size(args);
// 	ms_pwd(&mshell, args);
// 	ms_cd(&mshell, args);
// 	ms_pwd(&mshell, args);
// 	return (cleanup(&mshell, true, 0));
// }

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 17:37:44 by marboccu          #+#    #+#             */
/*   Updated: 2024/08/10 08:54:30 by odudniak         ###   ########.fr       */
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
	if (!current_home)
	{
		setstatus(mshell, 1);
		ft_perror("cd: HOME is not set\n", mshell->curr_path);
	}
	else
		chdir(mshell->home_path);
	ms_update_cwd(mshell);
	dbg_printf(CCYAN"[cd]:\t[]->%s\n"CR, mshell->curr_path);
	return (OK);
}

t_state	ms_cd(t_var *mshell, t_list *args)
{
	int			len;
	struct stat	buf;

	len = lst_size(args);
	if (len > 2)
		return (setstatus(mshell, 1),
			ft_perror("cd: too many arguments\n"), KO);
	else if (len == 1)
		return (ms_cd_home(mshell));
	if (stat(args->next->val, &buf) != 0)
		return (setstatus(mshell, 1),
			ft_perror("cd: %s: no such file or directory\n", args->next->val),
			KO);
	cd_update_oldpwd(mshell);
	chdir(args->next->val);
	ms_update_cwd(mshell);
	dbg_printf(CCYAN"[cd]:\t[%s]->%s\n"CR, args->next->val, mshell->curr_path);
	return (setstatus(mshell, 0), OK);
}

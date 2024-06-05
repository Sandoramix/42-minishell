/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_exec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marboccu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 09:52:54 by marboccu          #+#    #+#             */
/*   Updated: 2024/06/05 00:16:37 by marboccu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ms_run_builtin(t_var *mshell, t_list *args)
{
	if (str_equals(args->val, "export"))
		return (ms_export(mshell, args));
	else if (str_equals(args->val, "unset"))
		return (ms_unset(mshell, args));
	else if (str_equals(args->val, "echo"))
		return (ms_echo(mshell, args));
	else if (str_equals(args->val, "cd"))
		return (ms_cd(mshell, args));
	else if (str_equals(args->val, "pwd"))
		return (ms_pwd(mshell, args), OK);
	else if (str_equals(args->val, "env"))
		return (ms_env(mshell, args), OK);
	else if (str_equals(args->val, "history"))
		return (ms_history(mshell, args), OK);
	else if (str_equals(args->val, "exit"))
		return (ms_exit(mshell, args));
	return (KO);
}

static int	ms_exec_command(t_var *mshell, t_list *args, bool tofork)
{
	pid_t	pid;

	if (ms_is_builtin(args->val) && !tofork)
	{
		ms_run_builtin(mshell, args);
		return (OK);
	}
	else if (has_heredoc(args))
	{

		if (!ms_heredoc(mshell, args))
			return (KO);
	}
	//ms_heredoc(mshell, args);
	else if (ms_is_builtin(args->val) && tofork)
	{
		pid = fork();
		if (pid < 0)
			return (pf_errcode(ERR_FORK), KO);
		else if (!pid)
		{
			ms_run_builtin(mshell, args);
			cleanup(mshell, true, *mshell->status_code);
		}
	}
	else
		ms_exec_cmd(mshell, args);
	return (OK);
}

// Ritorna una lista di liste in `val`
t_list	*lst_split_bystrval(t_list *all, char *val)
{
	t_list	*split;
	t_list	*tmp;
	t_list	*next;

	split = NULL;
	tmp = all;
	while (all)
	{
		next = all->next;
		if (next && str_equals(next->val, val))
		{
			next = all->next->next;
			lst_delnode(&all->next, free);
			all->next = NULL;
			if (!lst_addnew_tail(&split, tmp, NULL))
				return (lst_free(&split, free), lst_free(&next, free));
			tmp = next;
		}
		all = next;
	}
	if (!split || tmp)
		lst_addnew_tail(&split, tmp, NULL);
	return (split);
}

void	*ms_exec_commands(t_var *mshell, t_list *all)
{
	t_list	*cmds_list;
	int		size;
	t_list	*command;

	mshell->all_cmds = lst_split_bystrval(all, "|");
	if (!mshell->all_cmds)
		return (NULL);
	cmds_list = mshell->all_cmds;
	size = lst_size(cmds_list);
	dbg_printf(CCYAN"TOTAL COMMANDS TO EXECUTE: %d\n"CR, size);
	while (cmds_list)
	{
		command = cmds_list->val;
		ms_exec_command(mshell, command, size > 1);
		cmds_list = cmds_list->next;
	}
	return (freeallcmds(mshell->all_cmds, true), NULL);
}

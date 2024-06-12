/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_exec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 09:52:54 by marboccu          #+#    #+#             */
/*   Updated: 2024/06/12 10:42:43 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ms_exec_builtin(t_var *mshell, t_command *command)
{
	t_list	*args;

	args = command->args;
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

static int	ms_pre_exec(t_var *mshell, t_command *command, bool tofork)
{
	if (ms_heredoc(mshell, command) == KO)
	{
		g_status = 1;
		if (tofork && !command->args)
			g_status = 0;
		return (KO);
	}
	if (!command->args)
	{
		g_status = 0;
		return (OK_EXIT);
	}
	ms_rediout(command);
	return (OK);
}

int	ms_exec_cmd(t_var *mshell, t_list *cmd)
{
	char	**paths;
	char	**args;
	char	**env;
	char	*abs_path;

	paths = env_load_paths(mshell->env);
	if (!paths && lst_findbykey_str(mshell->env, "PATH"))
		return (pf_errcode(E_MALLOC), cleanup(mshell, true, 1));
	args = lst_to_strmtx(cmd);
	env = lst_to_strmtx(mshell->env);
	if (!args || (!env && mshell->env))
		return (pf_errcode(E_MALLOC), str_freemtx(env), str_freemtx(args), KO);
	abs_path = sys_findcmdpath(paths, cmd->val);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	execve(abs_path, args, env);
	*mshell->status_code = 131;
	ft_perror("%s: command not found\n", cmd->val);
	return (str_freemtx(paths), str_freemtx(args), free(abs_path),
		cleanup(mshell, true, *mshell->status_code), KO);
}

static int	ms_exec_command(t_var *mshell, t_command *command, bool tofork)
{
	int		status;
	pid_t	pid;

	status = ms_pre_exec(mshell, command, tofork);
	if (status != OK)
		return ((int [2]){OK, KO}[status == KO]);
	if (tofork || !ms_is_builtin(command->args->val))
	{
		pid = fork();
		if (pid < 0)
			return (pf_errcode(E_FORK), KO);
		else if (!pid)
		{
			if (ms_is_builtin(command->args->val))
				ms_exec_builtin(mshell, command);
			else
				ms_exec_cmd(mshell, command->args);
			cleanup(mshell, true, *mshell->status_code);
		}
	}
	else
		ms_exec_builtin(mshell, command);
	return (OK);
}

void	*ms_exec_commands(t_var *mshell, t_list *all)
{
	t_list		*cmds_list;
	int			size;
	t_command	*command;

	mshell->all_cmds = lst_split_bystrval(all, "|");
	if (!ms_wrap_commands(mshell) || !mshell->all_cmds)
		return (NULL);
	cmds_list = mshell->all_cmds;
	size = lst_size(cmds_list);
	dbg_printf(CBLUE"N. of commands to execute: [%d]\n"CR, size);
	while (cmds_list)
	{
		command = cmds_list->val;
		ms_exec_command(mshell, command, size > 1);
		cmds_list = cmds_list->next;
	}
	return (freeallcmds(mshell->all_cmds, true), NULL);
}

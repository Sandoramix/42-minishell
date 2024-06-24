/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_exec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 09:52:54 by marboccu          #+#    #+#             */
/*   Updated: 2024/06/24 19:12:58 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	execute_builtin(t_var *mshell, t_command *command)
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

t_state	ms_exec_cmd(t_var *mshell, t_list *cmd)
{
	char	**paths;
	char	**args;
	char	**env;
	char	*abs_path;

	paths = env_load_paths(mshell->env);
	if (!paths && lst_findbykey_str(mshell->env, "PATH"))
		return (pf_errcode(E_MALLOC), cleanup(mshell, true, 1), KO);
	args = lst_to_strmtx(cmd);
	env = lst_env_to_mtx(mshell);
	if (!args || (!env && mshell->env))
		return (pf_errcode(E_MALLOC), str_freemtx(env), str_freemtx(args), KO);
	abs_path = sys_findcmdpath(paths, cmd->val);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (abs_path && g_status != 1 && execve(abs_path, args, env) == -1)
		pf_errcode(E_EXECVE);
	return (str_freemtx(paths), str_freemtx(args), str_freemtx(env),
		free(abs_path), cleanup(mshell, true, g_status), KO);
}

static int	ms_pre_exec(t_var *mshell, t_command *command, bool to_fork)
{
	if (ms_inredir_handle(mshell, command) == KO)
	{
		if (g_status != 130)
			g_set_status(1);
		if (g_status != 130 && to_fork && !command->args)
			g_set_status(0);
		return (KO);
	}
	if (ms_rediout(command) == KO)
		return (KO);
	if (!command->args)
		return (g_set_status(0), OK_EXIT);
	return (OK);
}

static t_state	ms_exec_command(t_var *mshell, t_command *command,
		int tot_cmds, int idx)
{
	const t_state	state = ms_pre_exec(mshell, command, tot_cmds > 1);
	pid_t			pid;

	if (tot_cmds > 1 || (command->args && !is_builtin(command->args->val)))
	{
		pid = fork();
		if (pid < 0)
			return (pf_errcode(E_FORK), KO);
		else if (!pid)
		{
			if (state != OK)
				return (reset_stds(mshell), cleanup(mshell, true, g_status));
			ms_exec_update_stds(mshell, command, idx);
			if (is_builtin(command->args->val))
				execute_builtin(mshell, command);
			else
				ms_exec_cmd(mshell, command->args);
			return (reset_stds(mshell), cleanup(mshell, true, g_status), OK);
		}
		return (OK);
	}
	if (state != OK)
		return ((int [2]){OK, KO}[state == KO]);
	return (ms_exec_update_stds(mshell, command, idx),
		execute_builtin(mshell, command), reset_stds(mshell), OK);
}

t_state	ms_exec_commands(t_var *mshell, t_list *all)
{
	t_command	*command;
	t_list		*cmds_list;
	int			tot_cmds;
	int			i;

	mshell->all_cmds = ms_split_pipelines(all);
	if (!mshell->all_cmds || ms_wrap_commands(mshell) == KO)
		return (KO);
	cmds_list = mshell->all_cmds;
	tot_cmds = lst_size(cmds_list);
	i = -1;
	mshell->pipes = ft_calloc(tot_cmds + 1, sizeof(int *));
	if (!mshell->pipes)
		return (pf_errcode(E_MALLOC), clean_cmds(mshell->all_cmds, true), KO);
	while (++i < tot_cmds)
	{
		mshell->pipes[i] = ft_calloc(2, sizeof(int));
		if (!mshell->pipes[i] || pipe(mshell->pipes[i]) == -1)
			return (kill_pipes(mshell), clean_cmds(mshell->all_cmds, true), KO);
		command = cmds_list->val;
		signal(SIGINT, handle_exec_sig);
		ms_exec_command(mshell, command, tot_cmds, i);
		cmds_list = cmds_list->next;
	}
	return (kill_pipes(mshell), clean_cmds(mshell->all_cmds, true), OK);
}

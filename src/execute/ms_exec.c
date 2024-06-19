/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_exec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 09:52:54 by marboccu          #+#    #+#             */
/*   Updated: 2024/06/19 22:06:31 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	ms_update_std(t_command *command)
{
	if (command->in_fd > 2)
	{
		dup2(command->in_fd, STDIN_FILENO);
		close(command->in_fd);
	}
	if (command->out_fd > 2)
	{
		dup2(command->out_fd, STDOUT_FILENO);
		close(command->out_fd);
	}
}

static int	ms_std_update(t_var *mshell, t_command *command, int idx)
{
	const int	tot_cmds = lst_size(mshell->all_cmds);
	const bool	should_pipe = tot_cmds > 1 && idx < tot_cmds - 1;
	int			*curr_fds;
	int			*prev_fds;

	curr_fds = mshell->pipes[idx % 2];
	prev_fds = mshell->pipes[1 - (idx % 2)];
	if (idx > 0 && command->in_fd <= 2 && tot_cmds > 1 && idx > 0)
		command->in_fd = prev_fds[0];
	if (command->out_fd <= 2 && should_pipe)
		command->out_fd = curr_fds[1];
	close(curr_fds[0]);
	close(prev_fds[1]);
	dbg_printf(CGREEN"cmd[%s]\tidx[%d]\ttot_cmds[%d]\tin_fd[%d]\tout_fd[%d]\n\
	\tcurr_fds_idx[%d]\tprev_fds_idx[%d]\n"CR, command->args->val,
		idx, tot_cmds, command->in_fd, command->out_fd, idx % 2, 1 - (idx % 2));
	ms_update_std(command);
	return (OK);
}

static int	ms_reset_std(t_var *mshell)
{
	dup2(mshell->orig_stdin, STDIN_FILENO);
	dup2(mshell->orig_stdout, STDOUT_FILENO);
	return (OK);
}

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
	env = lst_env_to_mtx(mshell);
	if (!args || (!env && mshell->env))
		return (pf_errcode(E_MALLOC), str_freemtx(env), str_freemtx(args), KO);
	abs_path = sys_findcmdpath(paths, cmd->val);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	execve(abs_path, args, env);
	*mshell->status_code = 131;
	ft_perror("%s: command not found\n", cmd->val);
	return (str_freemtx(paths), str_freemtx(args), str_freemtx(env),
		free(abs_path), cleanup(mshell, true, *mshell->status_code), KO);
}

static int	ms_pre_exec(t_var *mshell, t_command *command, bool tofork)
{
	if (ms_inredir_handle(mshell, command) == KO)
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
	if (ms_rediout(command) == KO)
	{
		g_status = 1;
		return (KO);
	}
	dbg_printf(CCYAN"Command's redirects:\tin[%d]\tout[%d]\n"CR,
		command->in_fd, command->out_fd);
	return (OK);
}

static int	ms_exec_command(t_var *mshell, t_command *command,
		bool tofork, int idx)
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
			ms_std_update(mshell, command, idx);
			if (ms_is_builtin(command->args->val))
				ms_exec_builtin(mshell, command);
			else
				ms_exec_cmd(mshell, command->args);
			ms_reset_std(mshell);
			cleanup(mshell, true, *mshell->status_code);
		}
	}
	else
	{
		ms_std_update(mshell, command, idx);
		ms_exec_builtin(mshell, command);
		ms_reset_std(mshell);
	}
	return (OK);
}

void	*ms_exec_commands(t_var *mshell, t_list *all)
{
	t_list		*cmds_list;
	t_command	*command;
	int			size;
	int			i;

	mshell->all_cmds = lst_split_bystrval(all, "|");
	if (!ms_wrap_commands(mshell) || !mshell->all_cmds)
		return (NULL);
	cmds_list = mshell->all_cmds;
	size = lst_size(cmds_list);
	i = -1;
	if (pipe(mshell->pipes[0]) == -1)
		return (pf_errcode(E_PIPE), freeallcmds(mshell->all_cmds, true), NULL);
	if (pipe(mshell->pipes[1]) == -1)
		return (pf_errcode(E_PIPE), freeallcmds(mshell->all_cmds, true), NULL);
	while (cmds_list && ++i > -1)
	{
		command = cmds_list->val;
		ms_exec_command(mshell, command, size > 1, i);
		cmds_list = cmds_list->next;
	}
	files_close(mshell->pipes[0], 2);
	files_close(mshell->pipes[1], 2);
	return (freeallcmds(mshell->all_cmds, true), NULL);
}

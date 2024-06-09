/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_exec_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 22:02:15 by marboccu          #+#    #+#             */
/*   Updated: 2024/06/09 14:19:55 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>


int	ms_exec_cmd(t_var *mshell, t_list *cmd)
{
	char	*cmd_path;
	char	**args;
	pid_t	pid;

	str_freemtx(mshell->cmds_paths);
	mshell->cmds_paths = env_load_paths(mshell->env);
	if (!mshell->cmds_paths && lst_findbykey_str(mshell->env, "PATH"))
		return (pf_errcode(ERR_MALLOC), KO);
	cmd_path = sys_findcmdpath(mshell->cmds_paths, cmd->val);
	args = lst_to_strmtx(cmd);
	if (!args)
	{
		pf_errcode(ERR_MALLOC);
		free(cmd_path);
		str_freemtx(args);
		return (KO);
	}
	pid = fork();
	if (pid < 0)
	{
		pf_errcode(ERR_FORK);
		str_freemtx(args);
		free(cmd_path);
		lst_free(&cmd, free);
		return (KO);
	}
	else if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		execve(cmd_path, args, mshell->_main.envp);
		*mshell->status_code = 131;
		ft_perror("Command not found: %s\n", cmd->val);
		str_freemtx(args);
		free(cmd_path);
		cleanup(mshell, true, *mshell->status_code);
	}
	str_freemtx(args);
	free(cmd_path);
	return (OK);
}

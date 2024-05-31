/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_execute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 22:02:15 by marboccu          #+#    #+#             */
/*   Updated: 2024/05/31 15:54:15 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	**ft_lst_to_array(t_list *lst)
{
	char	**arr;
	int		i;

	arr = ft_calloc(lst_size(lst) + 1, sizeof(char *));
	if (!arr)
		return (NULL);
	i = 0;
	while (lst)
	{
		arr[i] = str_dup(lst->val);
		if (!arr[i])
		{
			pf_errcode(ERR_MALLOC);
			str_freemtx(arr);
			return (NULL);
		}
		lst = lst->next;
		i++;
	}
	return (arr);
}

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
	args = ft_lst_to_array(cmd);
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
		lst_free(&cmd, free);
		freeallcmds(mshell->all_cmds, false);
		cleanup(mshell, true, *mshell->status_code);
	}
	str_freemtx(args);
	free(cmd_path);
	return (OK);
}

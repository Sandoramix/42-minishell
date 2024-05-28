/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_execute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marboccu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 22:02:15 by marboccu          #+#    #+#             */
/*   Updated: 2024/05/28 23:29:05 by marboccu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char *ms_get_cmd_path(t_var *mshell, char *cmd)
{
	char	*cmd_path;
	char	*tmp;
	char	**paths;
	int		i;

	cmd_path = NULL;
	paths = mshell->cmds_paths;
	i = 0;
	while (paths && paths[i])
	{
		tmp = str_join(paths[i], "/");
		if (!tmp)
			return (NULL);
		cmd_path = str_join(tmp, cmd);
		free(tmp);
		if (!cmd_path)
			return (NULL);
		if (access(cmd_path, F_OK) == 0)
			break ;
		free(cmd_path);
		cmd_path = NULL;
		i++;
	}
	return (cmd_path);
}

char **ft_lst_to_array(t_list *lst)
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

int ms_exec_cmd(t_var *mshell, t_list *cmd)
{
	char	*cmd_path;
	char	**args;
	pid_t	pid;
	int status;

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
		if (execve(cmd_path, args, mshell->_main.envp) == -1)
		{
			ft_perror("Command not found: %s\n", cmd->val);
			str_freemtx(args);
			free(cmd_path);
			lst_free(&cmd, free);
			cleanup(mshell, true, KO);
		}
	}
	else
	{
		waitpid(pid, &status, 0);
		*mshell->status_code = (t_uchar) (status);
	}
	str_freemtx(args);
	free(cmd_path);
	return (OK);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 18:16:29 by odudniak          #+#    #+#             */
/*   Updated: 2024/08/10 08:58:43 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static char	*handle_relative_path(t_var *mshell, char *path)
{
	if (file_isdir(path))
		return (setstatus(mshell, 126),
			ft_perror("%s: permission denied\n", path), NULL);
	if (!file_exists(path))
		return (setstatus(mshell, 127),
			ft_perror("%s: no such file or directory\n", path), NULL);
	if (file_hasperm(path, X_OK))
		return (str_dup(path));
	return (setstatus(mshell, 126),
		ft_perror("%s: permission denied\n", path), NULL);
}

char	*sys_findcmdpath(t_var *mshell, char **paths, char *cmd)
{
	int			i;
	char		*res;

	i = -1;
	if (str_startswith(cmd, "./") || str_startswith(cmd, "../")
		|| str_startswith(cmd, "/"))
		return (handle_relative_path(mshell, cmd));
	while (paths && paths[++i])
	{
		if (!str_endswith(paths[i], "/"))
			res = str_join(paths[i], "/");
		else
			res = str_dup(paths[i]);
		if (!res)
			return (NULL);
		res = str_freejoin(res, cmd);
		if (!res)
			return (NULL);
		if (file_hasperm(res, X_OK))
			return (res);
		free(res);
	}
	return (setstatus(mshell, 127),
		ft_perror("%s: command not found\n", cmd), NULL);
}

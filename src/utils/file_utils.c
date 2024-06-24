/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 18:16:29 by odudniak          #+#    #+#             */
/*   Updated: 2024/06/24 19:10:27 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static char	*handle_relative_path(char *path)
{
	if (file_isdir(path))
		return (g_set_status(126),
			ft_perror("%s: permission denied\n", path), NULL);
	if (!file_exists(path))
		return (g_set_status(127),
			ft_perror("%s: no such file or directory\n", path), NULL);
	if (file_hasperm(path, X_OK))
		return (str_dup(path));
	return (g_set_status(126),
		ft_perror("%s: permission denied\n", path), NULL);
}

char	*sys_findcmdpath(char **paths, char *cmd)
{
	int			i;
	char		*res;

	i = -1;
	if (str_startswith(cmd, "./") || str_startswith(cmd, "../")
		|| str_startswith(cmd, "/"))
		return (handle_relative_path(cmd));
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
	return (g_set_status(127), ft_perror("%s: command not found\n", cmd), NULL);
}

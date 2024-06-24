/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 10:38:09 by odudniak          #+#    #+#             */
/*   Updated: 2024/06/24 16:05:32 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

static char	*handle_relative_path(char *path)
{
	if (file_hasperm(path, X_OK))
		return (str_dup(path));
	return (NULL);
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
	return (NULL);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 17:39:55 by odudniak          #+#    #+#             */
/*   Updated: 2024/06/22 10:32:37 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

bool	file_isdir(char *path)
{
	int		fd;

	fd = open(path, O_DIRECTORY);
	if (fd == -1)
		return (false);
	close(fd);
	return (true);
}

bool	file_exists(char *path)
{
	return (!file_isdir(path) && access(path, F_OK) == 0);
}

void	file_close(int fd)
{
	struct stat	buffer;
	int			state;

	state = fstat(fd, &buffer);
	if (state == 0)
		close(fd);
}

int	files_close(int fds[], int n)
{
	int			res;
	int			i;

	i = -1;
	res = 0;
	while (++i < n)
	{
		if (fds[i] == -1)
			continue ;
		res++;
		close(fds[i]);
	}
	return (res);
}

bool	file_hasperm(char *path, mode_t perms)
{
	return (!file_isdir(path) && !access(path, perms));
}

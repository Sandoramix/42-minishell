/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 10:36:40 by marboccu          #+#    #+#             */
/*   Updated: 2024/06/09 14:24:41 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*heredoc_expand(t_var *mshell, char **arg)
{
	int		dollar_idx;
	char	*res;

	dollar_idx = str_idxofchar(*arg, '$');
	while (dollar_idx != -1)
	{
		res = arg_update(mshell, arg, &dollar_idx, true);
		if (!res)
			return (NULL);
	}
	return (*arg);
}

char	*gen_heredocs(int count)
{
	char	*suffix;
	char	*prefixed_name;
	char	*heredoc_file;

	suffix = ft_itoa(count);
	heredoc_file = file_gen_name(".heredoc_", R_OK | W_OK);
	prefixed_name = str_join(heredoc_file, suffix);
	if (!prefixed_name)
		return (ft_perror("file_gen_name"), NULL);
	free(suffix);
	free(heredoc_file);
	return (prefixed_name);
}

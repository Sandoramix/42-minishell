/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 10:36:40 by odudniak          #+#    #+#             */
/*   Updated: 2024/06/22 10:36:41 by odudniak         ###   ########.fr       */
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

static char	*gen_heredoc_prefix(t_var *mshell)
{
	char	*home_path;
	char	*res;

	home_path = NULL;
	if (mshell->home_path)
		home_path = str_dup(mshell->home_path);
	if (home_path && !str_endswith(home_path, "/"))
		home_path = str_freejoin(home_path, "/");
	if (!home_path && mshell->home_path)
		return (pf_errcode(E_MALLOC), NULL);
	res = str_freejoin(home_path, FILE_HEREDOC_PREFIX);
	return (res);
}

char	*gen_heredocs(t_var *mshell, int count)
{
	char	*suffix;
	char	*prefixed_name;
	char	*heredoc_file;

	prefixed_name = gen_heredoc_prefix(mshell);
	if (!prefixed_name)
		return (NULL);
	suffix = ft_itoa(count);
	if (!suffix)
		return (free(prefixed_name), NULL);
	prefixed_name = str_freejoin(prefixed_name, suffix);
	free(suffix);
	if (!prefixed_name)
		return (NULL);
	heredoc_file = file_gen_name(prefixed_name, R_OK | W_OK);
	free(prefixed_name);
	if (!heredoc_file)
		return (pf_errcode(E_MALLOC), NULL);
	return (heredoc_file);
}

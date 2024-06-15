/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/06/15 09:08:34 by odudniak         ###   ########.fr       */
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

char	*gen_heredocs(t_var *mshell, int count)
{
	char	*suffix;
	char	*prefixed_name;
	char	*heredoc_file;

	if (mshell->home_path)
		prefixed_name = str_join(mshell->home_path, FILE_HEREDOC_PREFIX);
	else
		prefixed_name = str_dup(FILE_HEREDOC_PREFIX);
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

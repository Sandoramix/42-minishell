/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marboccu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/06/10 18:30:55 by marboccu         ###   ########.fr       */
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
	prefixed_name = str_join(".heredoc_", suffix);
	free(suffix);
	heredoc_file = file_gen_name(prefixed_name, R_OK | W_OK);
	free(prefixed_name);
	if (!heredoc_file)
		return (pf_errcode(ERR_MALLOC), NULL);
	return (heredoc_file);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 14:56:28 by odudniak          #+#    #+#             */
/*   Updated: 2024/05/31 15:16:33 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

bool	ms_closing_quotes_check(char *s)
{
	int		edge;
	int		i;

	i = -1;
	while (s[++i])
	{
		if (chr_isquote(s[i]))
		{
			edge = chr_quoteclose_idx(s, i);
			if (edge == -1)
				return (false);
			i = edge;
		}
	}
	return (true);
}

bool	ms_token_syntax_check(t_list *args)
{
	const char	*known_tokens[] = {"<", "<<", ">", ">>", "|", NULL};
	int			i;

	while (args)
	{
		if (args->type == A_TOKEN)
		{
			i = 0;
			while (known_tokens[i] && str_cmp(args->val, known_tokens[i]) != 0)
				i++;
			if (i == 5)
				return (false);
			if ((args->prev && args->prev->type == A_TOKEN)
				|| (args->next && args->next->type == A_TOKEN))
				return (false);
			if (!str_cmp("<<", args->val) && !args->next)
				return (false);
			if (str_equals(args->val, "|") && (!args->prev || !args->next))
				return (false);
		}
		args = args->next;
	}
	return (true);
}

bool	ms_is_builtin(char *cmd)
{
	const char	*valid[] = {"export", "unset", "echo", "cd", "pwd",
		"env", "history", "exit", NULL};
	int			i;

	i = -1;
	while (valid[++i])
		if (str_equals(cmd, (char *)valid[i]))
			return (true);
	return (false);
}

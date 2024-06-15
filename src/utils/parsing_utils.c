/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 14:56:28 by odudniak          #+#    #+#             */
/*   Updated: 2024/06/15 09:36:03 by odudniak         ###   ########.fr       */
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

bool	cmdp_append_last(t_list **res, char *append, char dbg_char)
{
	t_list	*last;

	dbg_printf(CBGRAY"\tIt's near the char %c\n", dbg_char);
	last = lst_gettail(*res);
	if (!last && !lst_addnew_tail(res, append, NULL))
		return (lst_free(res, free), free(append), false);
	else
	{
		last->val = str_freejoin(last->val, append);
		free(append);
		if (!last->val)
			return (lst_free(res, free), false);
	}
	return (true);
}

void	cmd_dbgparse(t_cmdp_switch type, char *s, int i, int edge)
{
	const bool	common_part = chr_isspace(s[i + 1])
		|| !s[i + 1] || chr_isquote(s[i + 1]);

	if (!ft_isdebug())
		return ;
	dbg_printf(CGRAY);
	if (type == CMDP_QUOTE)
		dbg_printf("Found a quote {%c} at:\t[%3d] - [%3d]:\t", s[i], i, edge);
	else if (type == CMDP_TOKEN && chr_istoken(s[i])
		&& (common_part || !chr_istoken(s[i + 1])))
		dbg_printf("Found unquoted TOKEN at:\t[%3d] - [%3d]:\t", edge, i);
	else if (type == CMDP_WORD && (common_part || chr_istoken(s[i + 1])))
		dbg_printf("Found unquoted WORD at:\t[%3d] - [%3d]:\t", edge, i);
	else
	{
		dbg_printf(CR);
		return ;
	}
	write(1, "[", 1);
	if (type == CMDP_QUOTE)
		write(1, s + i, edge - i + 1);
	else
		write(1, s + edge, i - edge + 1);
	write(1, "]\n", 2);
	dbg_printf(CR);
}

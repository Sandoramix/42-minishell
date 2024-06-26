/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 14:56:28 by odudniak          #+#    #+#             */
/*   Updated: 2024/06/27 00:14:55 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

bool	ms_closing_quotes_check(char *s)
{
	int		edge;
	int		i;

	i = -1;
	while (s && s[++i])
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
	while (args)
	{
		if (args->type == A_TOKEN)
		{
			if (!is_known_token(args->val))
				return (false);
			if ((!args->next))
				return (dbg_printf("syntax error no neib.\n"), false);
			if (is_node_token(args, "|"))
			{
				if ((!args->next || is_node_token(args->next, "|"))
					|| (!args->prev || is_node_token(args->prev, NULL)))
					return (false);
			}
			else if (!is_node_token(args, "|")
				&& (is_node_token(args->next, "|")
					|| is_node_token(args->prev, "|")))
				;
			else if (is_node_token(args->prev, NULL)
				|| is_node_token(args->next, NULL))
				return (dbg_printf("syntax error n&p are token\n"), false);
		}
		args = args->next;
	}
	return (true);
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

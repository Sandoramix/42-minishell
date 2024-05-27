/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_parse.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 13:16:03 by odudniak          #+#    #+#             */
/*   Updated: 2024/05/15 22:23:48 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

// TODO: document this sh*t

static void	cmd_dbgparse(t_cmdp_switch type, char *s, int i, int edge)
{
	const bool	common_part = chr_isspace(s[i + 1])
		|| !s[i + 1] || chr_isquote(s[i + 1]);

	if (!DEBUG)
		return ;
	dbg_printf(COLOR_GRAY);
	if (type == CMDP_QUOTE)
		dbg_printf("Found a quote {%c} at:\t[%3d] - [%3d]:\t", s[i], i, edge);
	else if (type == CMDP_TOKEN && chr_istoken(s[i]) && common_part)
		dbg_printf("Found unquoted TOKEN at:\t[%3d] - [%3d]:\t", edge, i);
	else if (type == CMDP_WORD && common_part)
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

static bool	lst_strappend_last(t_list **res, char *append, char dbg_char)
{
	t_list	*last;

	dbg_printf(COLOR_BGRAY"\tIt's near the char %c\n", dbg_char);
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

static bool	cmdp_switch(t_cmdp_switch type, char *raw, t_cmdp_arg *split)
{
	const int		i = split->i;

	if (type == CMDP_QUOTE)
	{
		if (!chr_isquote(raw[i]))
			return (false);
		split->edge = chr_quoteclose_idx(raw, raw[i], i);
		if (split->edge == -1)
			split->edge = str_ilen(raw);
	}
	cmd_dbgparse(type, raw, i, split->edge);
	if (type == CMDP_QUOTE)
		return (true);
	if (type == CMDP_SPACE)
		return (chr_isspace(raw[i]) && !chr_isspace(raw[i + 1]));
	if (type == CMDP_TOKEN)
		return (chr_istoken(raw[i]) && (chr_isspace(raw[i + 1])
				|| !raw[i + 1] || chr_isquote(raw[i + 1])));
	if (type == CMDP_WORD)
		return ((!chr_isspace(raw[i]) && !chr_istoken(raw[i]))
			&& (chr_isspace(raw[i + 1]) || !raw[i + 1]
				|| chr_isquote(raw[i + 1]) || chr_istoken(raw[i + 1])));
	dbg_printf("[cmd_switch]: Unknown TYPE [%d]!\n", type);
	return (false);
}

static t_list	*cmdp_push_handle(char *raw, t_cmdp_arg *split)
{
	int		from;

	from = (int [2]){split->edge, split->i}[chr_isquote(raw[split->i])];
	if (chr_isquote(raw[split->i]))
		split->tmp = str_substr(raw, split->i, split->edge);
	else
		split->tmp = str_substr(raw, split->edge, split->i);
	if (!split->tmp)
		return (lst_free(&split->res, free), NULL);
	if (from != 0 && !chr_isspace(raw[from - 1]) && !chr_istoken(raw[from - 1]))
	{
		if (!lst_strappend_last(&split->res, split->tmp, raw[from - 1]))
			return (NULL);
	}
	else if (!lst_addnew_tail(&split->res, split->tmp, NULL))
		return (lst_free(&split->res, free), free(split->tmp), NULL);
	if (split->edge == from)
		split->edge = split->i + 1;
	else if (split->edge - 1 == split->i && ++split->i)
		split->edge++;
	else
		split->i = split->edge;
	return (split->res);
}

t_list	*cmd_parse(t_var *mshell, char *raw)
{
	t_cmdp_arg		split;

	split = (t_cmdp_arg){0};
	split.i = -1;
	dbg_printf(COLOR_BRED"[cmd_parse] input: `%s`\n"CR, raw);
	while (raw && raw[++split.i])
	{
		if (cmdp_switch(CMDP_SPACE, raw, &split))
			split.edge = split.i + 1;
		else if (cmdp_switch(CMDP_TOKEN, raw, &split))
		{
			split.tmp = str_substr(raw, split.edge, split.i);
			if (!split.tmp || !lst_addnew_tail(&split.res, split.tmp, NULL))
				return (lst_free(&split.res, free), free(split.tmp), NULL);
			lst_gettail(split.res)->type = A_TOKEN;
			split.edge = split.i + 1;
		}
		else if (cmdp_switch(CMDP_QUOTE, raw, &split)
			|| cmdp_switch(CMDP_WORD, raw, &split))
		{
			if (!cmdp_push_handle(raw, &split))
				return (NULL);
		}
	}
	return (expand_and_clear(mshell, split.res));
}

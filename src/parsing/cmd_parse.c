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

static bool	cmdp_switch(t_cmdp_switch type, t_cmdp_arg *var)
{
	const int	i = var->i;
	const bool	curr_token = chr_istoken(var->str[i]);
	const bool	next_space_or_end = chr_isspace(var->str[i + 1])
		|| !var->str[i + 1];
	const bool	next_quote = chr_isquote(var->str[i + 1]);
	const bool	next_token = chr_istoken(var->str[i + 1]);

	if (type == CMDP_QUOTE)
		return (chr_isquote(var->str[i]));
	if (type == CMDP_TOKEN)
		return (curr_token && (next_space_or_end || !next_token || next_quote
				|| (var->str[i] != var->str[i + 1])));
	if (type == CMDP_WORD)
		return ((!chr_isspace(var->str[i]) && !curr_token)
			&& (next_space_or_end || next_quote || next_token));
	dbg_printf(CGRAY"[cmd_switch]: Unknown TYPE [%d]!\n", type);
	return (false);
}

static t_list	*cmdp_handleword(t_cmdp_arg *var)
{
	cmd_dbgparse(CMDP_WORD, var->str, var->i, var->edge);
	var->tmp = str_substr(var->str, var->edge, var->i);
	if (!var->tmp)
		return (lst_free(&var->res, free), NULL);
	if (var->edge != 0 && !chr_isspace(var->str[var->edge - 1])
		&& !chr_istoken(var->str[var->edge - 1]))
	{
		if (!cmdp_append_last(&var->res, var->tmp, var->str[var->edge - 1]))
			return (NULL);
	}
	else if (!lst_addnew_tail(&var->res, var->tmp, NULL))
		return (lst_free(&var->res, free), free(var->tmp), NULL);
	var->edge = var->i + 1;
	return (var->res);
}

static t_list	*cmdp_handlequote(t_cmdp_arg *var)
{
	var->edge = chr_quoteclose_idx(var->str, var->i);
	if (var->edge == -1)
		var->edge = str_ilen(var->str);
	cmd_dbgparse(CMDP_QUOTE, var->str, var->i, var->edge);
	var->tmp = str_substr(var->str, var->i, var->edge);
	if (!var->tmp)
		return (lst_free(&var->res, free), NULL);
	if (var->i != 0 && !chr_isspace(var->str[var->i - 1])
		&& !chr_istoken(var->str[var->i - 1]))
	{
		if (!cmdp_append_last(&var->res, var->tmp, var->str[var->i - 1]))
			return (NULL);
	}
	else if (!lst_addnew_tail(&var->res, var->tmp, NULL))
		return (lst_free(&var->res, free), free(var->tmp), NULL);
	if (var->edge > var->i)
		var->i = var->edge;
	var->edge++;
	return (var->res);
}

static t_list	*cmdp_handletoken(t_cmdp_arg *var)
{
	cmd_dbgparse(CMDP_TOKEN, var->str, var->i, var->edge);
	var->tmp = str_substr(var->str, var->edge, var->i);
	if (!var->tmp || !lst_addnew_tail(&var->res, var->tmp, NULL))
		return (lst_free(&var->res, free), free(var->tmp), NULL);
	lst_gettail(var->res)->type = A_TOKEN;
	var->edge = var->i + 1;
	return (var->res);
}

t_list	*cmd_parse(t_var *mshell, char **s)
{
	t_cmdp_arg		var;

	if (!pre_parse_expand(mshell, s))
		return (NULL);
	var = (t_cmdp_arg){0};
	var.i = -1;
	var.str = *s;
	dbg_printf(CBYELLOW"[cmd_parse] input: `%s`\n"CR, *s);
	while (s && *s && (*s)[++var.i])
	{
		if (chr_isspace((*s)[var.i]) && !chr_isspace((*s)[var.i + 1]))
			var.edge = var.i + 1;
		if (!cmdp_switch(CMDP_TOKEN, &var)
			&& !cmdp_switch(CMDP_QUOTE, &var) && !cmdp_switch(CMDP_WORD, &var))
			continue ;
		if (cmdp_switch(CMDP_TOKEN, &var))
			var.res = cmdp_handletoken(&var);
		else if (cmdp_switch(CMDP_QUOTE, &var))
			var.res = cmdp_handlequote(&var);
		else if (cmdp_switch(CMDP_WORD, &var))
			var.res = cmdp_handleword(&var);
		if (!var.res)
			return (NULL);
	}
	return (args_clearquotes(mshell, var.res));
}

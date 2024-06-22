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
		return (curr_token && (next_space_or_end || !next_token || next_quote));
	if (type == CMDP_WORD)
		return ((!chr_isspace(var->str[i]) && !curr_token)
			&& (next_space_or_end || next_quote || next_token));
	dbg_printf("[cmd_switch]: Unknown TYPE [%d]!\n", type);
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

char	*pre_parse_escape_newval(char *value)
{
	char	*tmp;
	int		i;

	i = -1;
	while (value && value[++i])
	{
		if (chr_istoken(value[i]) && !str_ischar_inquotes(value, i))
		{
			tmp = ft_calloc(str_ilen(value) + 3, sizeof(char));
			if (!tmp)
				return (free(value), NULL);
			str_lcpy(tmp, value, i + 1);
			tmp[i] = '\'';
			tmp[i + 1] = value[i];
			tmp[i + 2] = '\'';
			str_lcpy(tmp + i + 3, value + i + 1, str_ilen(value) + 3);
			value = tmp;
			i = -1;
		}
	}
	return (value);
}

char	*pre_parse_replace_variable(t_var *mshell,
	char **arg_p, int *dollar_idx, int *end_idx)
{
	char	*variable;
	t_list	*env;
	char	*val;

	variable = str_substr(*arg_p, *dollar_idx + 1, *end_idx);
	if (!variable)
		return (pf_errcode(E_MALLOC), NULL);
	if (str_equals(variable, "?"))
		return (free(variable), expand_statuscode(arg_p, dollar_idx, end_idx));
	env = lst_findbykey_str(mshell->env, variable);
	free(variable);
	if (!env)
		return (expand_empty(arg_p, dollar_idx, end_idx));
	val = pre_parse_escape_newval(env->val);
	if (!val)
		return (pf_errcode(E_MALLOC), NULL);
	*arg_p = str_replace_from_to(*arg_p, *dollar_idx, *end_idx, val);
	if (!(*arg_p))
		return (pf_errcode(E_MALLOC), NULL);
	*dollar_idx = str_idxofchar(*arg_p, '$');
	return (*arg_p);
}

static char	*pre_parse_arg_update(t_var *mshell, char **arg, int *d_idx, bool in_heredoc)
{
	const int		len = str_ilen((*arg));
	int				end;

	if (*d_idx == -1)
		return (*arg);
	end = str_var_ending_idx((*arg), (*d_idx));
	dbg_printf("\tFound a $ at [%3d] - [%3d]\n", (*d_idx), end, len);
	if (str_ischar_inquotes((*arg), (*d_idx)) == '\'')
	{
		(*d_idx) = str_idxofchar_from((*arg), (*d_idx) + 1, '$');
		return (dbg_printf(CMAGENTA"\t\tIt's inside single quote.\n"), *arg);
	}
	if (end == *d_idx && chr_isquote(((*arg))[*d_idx + 1]))
	{
		if (!in_heredoc)
			(*arg) = str_replace_from_to((*arg), (*d_idx), (end), "");
		(*d_idx) = str_idxofchar_from((*arg), (*d_idx) + 1, '$');
		return (*arg);
	}
	else if (end == *d_idx)
	{
		(*d_idx) = str_idxofchar_from((*arg), (*d_idx) + 1, '$');
		return (*arg);
	}
	return (pre_parse_replace_variable(mshell, arg, d_idx, &end));
}

static char	*pre_parse(t_var *mshell, char **line)
{
	int		dollar_idx;
	char	*res;

	res = *line;
	if (!line || !*line)
		return (dbg_printf(CRED"WTF??\n"CR), NULL);
	dollar_idx = str_idxofchar(*line, '$');
	dbg_printf(CYELLOW"[pre_parse] of [%s]\n"CR, *line);
	while (dollar_idx != -1)
	{
		res = pre_parse_arg_update(mshell, line, &dollar_idx, false);
		if (!res)
			return (NULL);
	}
	return (res);
}
t_list	*cmd_parse(t_var *mshell, char **s)
{
	t_cmdp_arg		var;

	(void)pre_parse;
	//if (!pre_parse(mshell, s))
	//	return (NULL);
	var = (t_cmdp_arg){0};
	var.i = -1;
	var.str = *s;
	dbg_printf(CBRED"[cmd_parse] input: `%s`\n"CR, *s);
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
	return (cmd_expand_clear(mshell, var.res));
}

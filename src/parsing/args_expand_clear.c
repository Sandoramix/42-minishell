/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_expand_clear.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/05 14:24:26 by odudniak          #+#    #+#             */
/*   Updated: 2024/06/06 21:37:49 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*replace_empty(t_var *mshell, char **arg, int *d_idx, int *end)
{
	(void)mshell;
	dbg_printf(CGREEN"\t\tVariable not found, replacing with \"\"\n");
	(*arg) = str_replace_from_to((*arg), (*d_idx), (*end), "");
	(*end) = (*d_idx);
	if (!(*arg))
		return (pf_errcode(ERR_MALLOC), NULL);
	(*d_idx) = str_idxofchar((*arg), '$');
	return (*arg);
}

char	*replace_laststatus(t_var *mshell, char **arg, int *d_idx, int *end)
{
	char	*status;

	status = ft_itoa(*mshell->status_code);
	if (!status)
		return (pf_errcode(ERR_MALLOC), NULL);
	(*arg) = str_replace_from_to((*arg), (*d_idx), (*end), status);
	free(status);
	if (!*arg)
		return (pf_errcode(ERR_MALLOC), NULL);
	(*d_idx) = str_idxofchar((*arg), '$');
	return (*arg);
}

char	*replace_variable(t_var *mshell, char **arg, int *d_idx, int *end)
{
	char	*variable;
	t_list	*env;

	variable = str_substr((*arg), (*d_idx) + 1, (*end));
	if (!variable)
		return (pf_errcode(ERR_MALLOC), NULL);
	dbg_printf(CGRAY"\t\tVariable to search: [%s]\n", variable);
	if (str_equals(variable, "?"))
		return (free(variable), replace_laststatus(mshell, arg, d_idx, end));
	env = lst_findbykey_str(mshell->env, variable);
	free(variable);
	if (!env)
		return (replace_empty(mshell, arg, d_idx, end));
	dbg_printf(CGREEN"\tVariable found with value: [%s]\n", env->val);
	(*arg) = str_replace_from_to((*arg), (*d_idx), (*end), env->val);
	if (!(*arg))
		return (pf_errcode(ERR_MALLOC), NULL);
	(*d_idx) = str_idxofchar((*arg), '$');
	return (*arg);
}

char	*arg_update(t_var *mshell, char **arg, int *d_idx, bool in_heredoc)
{
	const int		len = str_ilen((*arg));
	int				end;

	if (*d_idx == -1) // EXTRA 2 lines; but need to be careful of it's usage if removed
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
		dbg_printf(CMAGENTA"\t\tIt has a bad ending idx\n");
		return (*arg);
	}
	return (replace_variable(mshell, arg, d_idx, &end));
}

t_list	*arg_expand(t_var *mshell, t_list *arg)
{
	int		dollar_idx;
	char	*argument;
	char	*res;

	dollar_idx = str_idxofchar((char *)arg->val, '$');
	argument = (char *)(arg->val);
	dbg_printf(CYELLOW"[arg_expand] of [%s]\n", argument);
	if (arg->prev && arg->prev->type == A_TOKEN)
	{
		if (argument && str_includesset(arg->val, "'\""))
			arg->prev->_prevent_expansion = true;
		if (str_equals(arg->val, "<<"))
			return (arg);
	}
	while (dollar_idx != -1)
	{
		res = arg_update(mshell, &argument, &dollar_idx, false);
		arg->val = argument;
		if (!res)
			return (NULL);
	}
	return (arg);
}

t_list	*cmd_expand_clear(t_var *mshell, t_list *args)
{
	t_list	*arg;

	arg = args;
	while (arg)
	{
		dbg_printf(CCYAN"[expand_clear]\t|%s|:\n"CR, arg->val);
		if (arg->val)
		{
			if (!arg_expand(mshell, arg))
				return (lst_free(&args, free), cleanup(mshell, true, KO), NULL);
			if (!str_clearquotes((char **)&arg->val))
				return (lst_free(&args, free), cleanup(mshell, true, KO), NULL);
		}
		dbg_printf(CCYAN"\tresult|%s|\n"CR, arg->val);
		arg = arg->next;
	}
	return (args);
}

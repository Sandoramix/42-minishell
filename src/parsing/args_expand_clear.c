/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_expand_clear.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marboccu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/05 14:24:26 by odudniak          #+#    #+#             */
/*   Updated: 2024/06/13 09:38:57 by marboccu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

// TODO: REFACTOR TIME

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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_expand_clear.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/05 14:24:26 by odudniak          #+#    #+#             */
/*   Updated: 2024/05/22 15:38:12 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_list	*replace_variable(t_var *mshell, t_list *arg, int *d_idx, int *end)
{
	char	*variable;
	t_list	*env;

	variable = my_substr(arg->val, (*d_idx) + 1, (*end));
	if (!variable)
		return (pf_errcode(ERR_MALLOC), NULL);
	dbg_printf(COLOR_GRAY"\t\tVariable to search: [%s]\n", variable);
	env = lst_findbykey_str(mshell->env, variable);
	free(variable);
	if (env)
	{
		dbg_printf(COLOR_GREEN"\tVariable found with value: [%s]\n", env->val);
		arg->val = str_replace_from_to(arg->val, (*d_idx), (*end), env->val);
		if (!arg->val)
			return (pf_errcode(ERR_MALLOC), NULL);
		(*d_idx) = str_idxofchar(arg->val, '$');
		return (arg);
	}
	dbg_printf(COLOR_GREEN"\t\tVariable not found, replacing with \"\"\n");
	arg->val = str_replace_from_to((char *)arg->val, (*d_idx), (*end), "");
	(*end) = (*d_idx);
	if (!arg->val)
		return (pf_errcode(ERR_MALLOC), NULL);
	(*d_idx) = str_idxofchar(arg->val, '$');
	return (arg);
}

t_list	*arg_update(t_var *mshell, t_list *arg, int *d_idx)
{
	int		end;
	int		len;

	len = str_ilen((char *)arg->val);
	end = str_var_ending_idx((char *)arg->val, (*d_idx));
	dbg_printf("\tFound a $ at [%3d] - [%3d]\n", (*d_idx), end, len);
	if (str_ischar_inquotes((char *)arg->val, (*d_idx)) == '\'')
	{
		dbg_printf(COLOR_MAGENTA"\t\tIt's inside single quote ('). Skipping\n");
		(*d_idx) = str_idxofchar_from(arg->val, (*d_idx) + 1, '$');
		return (arg);
	}
	if (end == len || end == (*d_idx))
	{
		dbg_printf(COLOR_MAGENTA"\t\tIt's does not have a good ending idx\n");
		(*d_idx) = str_idxofchar_from(arg->val, (*d_idx) + 1, '$');
		return (arg);
	}
	return (replace_variable(mshell, arg, d_idx, &end));
}

t_list	*arg_expand(t_var *mshell, t_list *arg)
{
	int		dollar_idx;

	dollar_idx = str_idxofchar((char *)arg->val, '$');
	dbg_printf(COLOR_YELLOW"[arg_expand] of [%s]\n", arg->val);
	if (arg->val)
		arg->_first_char = ((char *)arg->val)[0];
	while (dollar_idx != -1)
	{
		if (!arg_update(mshell, arg, &dollar_idx))
			return (NULL);
	}
	return (arg);
}

t_list	*expand_and_clear(t_var *mshell, t_list *args)
{
	t_list	*arg;

	arg = args;
	while (arg)
	{
		dbg_printf(COLOR_CYAN"[expand_clear]\t|%s|:\n"CR, arg->val);
		if (arg->val)
		{
			if (!arg_expand(mshell, arg))
				return (lst_free(&args, free), cleanup(mshell, true, KO), NULL);
			if (!str_clearquotes((char **)&arg->val))
				return (lst_free(&args, free), cleanup(mshell, true, KO), NULL);
		}
		dbg_printf(COLOR_CYAN"\tresult|%s|\n"CR, arg->val);
		arg = arg->next;
	}
	return (args);
}

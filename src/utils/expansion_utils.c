/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 09:38:41 by marboccu          #+#    #+#             */
/*   Updated: 2024/06/22 10:42:13 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*expand_empty(char **arg, int *dollar_idx, int *end_idx)
{
	dbg_printf(CGREEN"\t\tVariable not found, replacing with \"\"\n"CR);
	*arg = str_replace_from_to((*arg), *dollar_idx, *end_idx, "");
	*end_idx = *dollar_idx;
	if (!(*arg))
		return (pf_errcode(E_MALLOC), NULL);
	*dollar_idx = str_idxofchar((*arg), '$');
	return (*arg);
}

char	*expand_statuscode(t_var *mshell,
	char **arg, int *dollar_idx, int *end_idx)
{
	char	*status_code;

	status_code = ft_itoa(*mshell->status_code);
	if (!status_code)
		return (pf_errcode(E_MALLOC), NULL);
	*arg = str_replace_from_to(*arg, *dollar_idx, *end_idx, status_code);
	free(status_code);
	if (!*arg)
		return (pf_errcode(E_MALLOC), NULL);
	*dollar_idx = str_idxofchar(*arg, '$');
	return (*arg);
}

char	*replace_variable(t_var *mshell,
	char **arg_p, int *dollar_idx, int *end_idx)
{
	char	*variable;
	t_list	*env;

	variable = str_substr(*arg_p, *dollar_idx + 1, *end_idx);
	if (!variable)
		return (pf_errcode(E_MALLOC), NULL);
	dbg_printf(CGRAY"\t\tVariable to search: [%s]\n"CR, variable);
	if (str_equals(variable, "?"))
		return (free(variable),
			expand_statuscode(mshell, arg_p, dollar_idx, end_idx));
	env = lst_findbykey_str(mshell->env, variable);
	free(variable);
	if (!env)
		return (expand_empty(arg_p, dollar_idx, end_idx));
	dbg_printf(CGREEN"\tVariable found with value: [%s]\n"CR, env->val);
	*arg_p = str_replace_from_to(*arg_p, *dollar_idx, *end_idx, env->val);
	if (!(*arg_p))
		return (pf_errcode(E_MALLOC), NULL);
	*dollar_idx = str_idxofchar(*arg_p, '$');
	return (*arg_p);
}

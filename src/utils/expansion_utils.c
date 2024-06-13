/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marboccu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 09:38:41 by marboccu          #+#    #+#             */
/*   Updated: 2024/06/13 09:39:00 by marboccu         ###   ########.fr       */
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
		return (pf_errcode(E_MALLOC), NULL);
	(*d_idx) = str_idxofchar((*arg), '$');
	return (*arg);
}

char	*replace_laststatus(t_var *mshell, char **arg, int *d_idx, int *end)
{
	char	*status;

	status = ft_itoa(*mshell->status_code);
	if (!status)
		return (pf_errcode(E_MALLOC), NULL);
	(*arg) = str_replace_from_to((*arg), (*d_idx), (*end), status);
	free(status);
	if (!*arg)
		return (pf_errcode(E_MALLOC), NULL);
	(*d_idx) = str_idxofchar((*arg), '$');
	return (*arg);
}

char	*replace_variable(t_var *mshell, char **arg, int *d_idx, int *end)
{
	char	*variable;
	t_list	*env;

	variable = str_substr((*arg), (*d_idx) + 1, (*end));
	if (!variable)
		return (pf_errcode(E_MALLOC), NULL);
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
		return (pf_errcode(E_MALLOC), NULL);
	(*d_idx) = str_idxofchar((*arg), '$');
	return (*arg);
}

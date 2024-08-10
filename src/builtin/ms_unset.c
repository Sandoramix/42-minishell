/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 14:25:21 by odudniak          #+#    #+#             */
/*   Updated: 2024/08/10 08:57:21 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	ms_unset_debug(t_var *mshell, t_list *args, char *stage)
{
	t_list	*node;

	if (!ft_isdebug())
		return ;
	dbg_printf(CMAGENTA"[unset]\t%s\n"CR, stage);
	if (args)
		args = args->next;
	while (args)
	{
		node = lst_findbykey_str(mshell->env, args->val);
		if (node)
			dbg_printf("\t"CGREEN"%s"CYELLOW"="CR"%s\n"CR,
				node->key, node->val);
		else
			dbg_printf("\t%s "CRED"IS NULL\n"CR, args->val);
		args = args->next;
	}
}

t_state	ms_unset(t_var *mshell, t_list *args)
{
	t_list	*tmp;
	t_uchar	final_status;

	if (!args)
		return (pf_errcode(E_INVALID_ARGC), KO);
	final_status = 0;
	tmp = args->next;
	ms_unset_debug(mshell, args, "PRE UNSET");
	while (tmp)
	{
		if (!str_isvariable(tmp->val))
		{
			final_status = 1;
			ft_perror("export: `%s`: not a valid identifier\n", tmp->val);
		}
		lst_delbykey(&mshell->env, tmp->val, (void *)str_equals, free);
		tmp = tmp->next;
	}
	setstatus(mshell, final_status);
	ms_unset_debug(mshell, args, "POST UNSET");
	return (OK);
}

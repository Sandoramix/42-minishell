/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marboccu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 14:25:21 by odudniak          #+#    #+#             */
/*   Updated: 2024/06/20 20:11:56 by marboccu         ###   ########.fr       */
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

int	ms_unset(t_var *mshell, t_list *args)
{
	t_list	*tmp;
	t_uchar status;

	if (!args)
		return (pf_errcode(E_INVALID_ARGC), KO);
	status = 0;
	tmp = args->next;
	ms_unset_debug(mshell, args, "PRE UNSET");
	while (tmp)
	{
		if (!str_isvariable(tmp->val))
		{
			status = 1;
			ft_perror("export: `%s`: not a valid identifier\n", tmp->val);
		}
		lst_delbykey(&mshell->env, tmp->val, (void *)str_equals, free);
		tmp = tmp->next;
	}
	*mshell->status_code = status;
	ms_unset_debug(mshell, args, "POST UNSET");
	return (OK);
}

//int	main(int ac, char **av, char **envp)
//{
//	t_var			mshell;
//	t_list			*listargs;

//	(void)av;
//	mshell = (t_var){0};
//	if (ac != 1)
//		return (pf_errcode(E_INVALID_ARGC), cleanup(&mshell, true, 1), 1);
//	mshell._main.envp = envp;
//	ms_init(&mshell);
//	listargs = cmd_parse(mshell, "export");
//	ms_export(&mshell, listargs);
//	lst_free(&listargs, free);
//	return (cleanup(&mshell, true, 0));
//}

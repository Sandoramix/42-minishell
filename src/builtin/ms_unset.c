/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 14:25:21 by odudniak          #+#    #+#             */
/*   Updated: 2024/05/08 15:23:15 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	ms_unset_debug(t_var *mshell, t_list *args, char *stage)
{
	t_list	*node;

	if (!DEBUG)
		return ;
	printf(COLOR_MAGENTA"[ms_unset] %s\n"CR, stage);
	while (args)
	{
		node = lst_findbykey_str(mshell->env, args->val);
		if (node)
			dbg_printf("\t"COLOR_GREEN"%s"COLOR_YELLOW"="CR"%s\n"CR,
				(char *)node->key, (char *)node->val);
		else
			dbg_printf("\t%s "COLOR_RED"IS NULL\n"CR, args->val);
		args = args->next;
	}
}

void	ms_unset(t_var *mshell, t_list *args)
{
	t_list	*tmp;

	tmp = args;
	ms_unset_debug(mshell, args, "PRE UNSET");
	while (tmp)
	{
		lst_delbykey(&mshell->env, tmp->val, (void *)str_equals, free);
		tmp = tmp->next;
	}
	ms_unset_debug(mshell, args, "POST UNSET");
}

//int	main(int ac, char **av, char **envp)
//{
//	t_var			mshell;
//	char			**args;
//	t_list			*listargs;

//	(void)av;
//	mshell = (t_var){0};
//	if (ac != 1)
//		return (pf_errcode(ERR_INVALID_ARGC), cleanup(&mshell, true, 1), 1);
//	mshell._main.envp = envp;
//	ms_init(&mshell);
//	args = cmd_parse("''un\"set\" LESS ''  'P''ATH' 'HOME' 'LS_COLORS'");
//	args = expand_and_clear_mtx(args);
//	ms_unset(&mshell, args);
//	str_freemtx(args);

//	listargs = cmd_parse_new("export");
//	listargs = expand_and_clear(listargs);
//	ms_export(&mshell, listargs);
//	lst_free(&listargs, free);
//	return (cleanup(&mshell, true, 0));
//}
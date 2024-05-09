/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 14:25:21 by odudniak          #+#    #+#             */
/*   Updated: 2024/05/09 09:20:09 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	ms_unset_debug(t_var *mshell, t_list *args, char *stage)
{
	t_list	*node;

	if (!DEBUG)
		return ;
	printf(COLOR_MAGENTA"[unset]\t%s\n"CR, stage);
	if (args)
		args = args->next;
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

int	ms_unset(t_var *mshell, t_list *args)
{
	t_list	*tmp;

	if (!args)
		return (pf_errcode(ERR_INVALID_ARGC), KO);
	if (str_cmp(args->val, "unset"))
		return (ft_perror("command not fount: %s\n"), KO);
	tmp = args->next;
	ms_unset_debug(mshell, args, "PRE UNSET");
	while (tmp)
	{
		dbg_printf(COLOR_YELLOW"[unset]\tDeleting: %s\n"CR, tmp->val);
		lst_delbykey(&mshell->env, tmp->val, (void *)str_equals, free);
		tmp = tmp->next;
	}
	ms_unset_debug(mshell, args, "POST UNSET");
	return (OK);
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
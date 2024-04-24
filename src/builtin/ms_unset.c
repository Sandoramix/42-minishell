/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 14:25:21 by odudniak          #+#    #+#             */
/*   Updated: 2024/04/24 15:37:17 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	ms_unset_debug(t_var *mshell, char **args, char *stage)
{
	int		i;
	t_list	*node;

	i = -1;
	if (!DEBUG)
		return ;
	printf(COLOR_MAGENTA"[ms_unset] %s\n"CR, stage);
	while (args && args[++i])
	{
		node = lst_findbykey_str(mshell->env, args[i]);
		if (node)
			printf("\t"COLOR_GREEN"%s"COLOR_YELLOW"="CR"%s\n"CR,
				(char *)node->key, (char *)node->val);
		else
			printf("\t%s "COLOR_RED"IS NULL\n"CR, args[i]);
	}
}

void	ms_unset(t_var *mshell, char **args)
{
	int	i;

	i = -1;
	ms_unset_debug(mshell, args, "PRE UNSET");
	while (args && args[++i])
		lst_delbykey(&mshell->env, args[i], (void *)str_equals, free);
	ms_unset_debug(mshell, args, "POST UNSET");
}

/*
int	main(int ac, char **av, char **envp)
{
	const char		*unset_args[] = {"HOME", "USER", "PATH", "CWD", NULL};
	t_var			mshell;

	(void)av;
	mshell = (t_var){0};
	if (ac != 1)
		return (pf_errcode(ERR_INVALID_ARGC), cleanup(&mshell, true, 1), 1);
	mshell._main.envp = envp;
	ms_init(&mshell);
	ms_unset(&mshell, (char **)unset_args);
	return (cleanup(&mshell, true, 0));
}
*/
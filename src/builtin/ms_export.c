/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 11:04:40 by odudniak          #+#    #+#             */
/*   Updated: 2024/04/25 18:56:22 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	ms_export_debug(t_var *mshell, char **args)
{
	if (!DEBUG)
		return ;
	printf(COLOR_CYAN"[ms_export] DEBUG:\n"CR);
	ft_putstrmtx(args);
	(void)mshell;
}

// TODO:  print with "escape" of special characters (a='\"') ?
static void	ms_export_print(t_var *mshell)
{
	t_list	*lst;
	char	*val;
	bool	addquote;

	return ;
	lst = mshell->env;
	while (lst)
	{
		printf("declare -x %s", (char *)lst->key);
		val = (char *)lst->val;
		addquote = !val || !val[0] || str_includesset(val, "\"'*;$@");
		if (!lst->_hidden && addquote)
			printf("='%s'", val);
		else if (!lst->_hidden && !addquote)
			printf("=%s", val);
		printf("\n");
		lst = lst->next;
	}
}

static int	ms_export_error(t_var *mshell, char **args, char **split)
{
	str_freemtx(args);
	str_freemtx(split);
	pf_errcode(ERR_MALLOC);
	cleanup(mshell, true, 1);
	return (KO);
}

int	ms_export(t_var *mshell, char **args)
{
	const int	len = str_mtxlen(args);
	char		**split;
	int			i;
	t_list		*node;

	ms_export_debug(mshell, args);
	if (len <= 1)
		return (ms_export_print(mshell), OK);
	i = 0;
	while (args && args[++i])
	{
		split = str_split_first(args[i], '=');
		if (!split)
			return (ms_export_error(mshell, args, NULL));
		node = lst_new(split[1], split[0]);
		if (!node)
			return (ms_export_error(mshell, args, split));
		if (!split[1])
			node->_hidden = true;
		if (!lst_addnode_tail(&mshell->env, node))
			return (ms_export_error(mshell, args, split));
		free(split);
	}
	return (OK);
}

/*
int	main(int ac, char **av, char **envp)
{
	t_var		mshell;
	const char	*args[] = {"export", "a1", "a2=t2", "a3=t3", "a4=\"t4\"",
		"a5='t5'", "a6='t 6'", NULL};
	const char	*arg_1[]= {"export", NULL};

	(void)av;
	mshell = (t_var){0};
	if (ac != 1)
		return (pf_errcode(ERR_INVALID_ARGC), cleanup(&mshell, true, 1), 1);
	mshell._main.envp = envp;
	ms_init(&mshell);

	ms_export(&mshell, (char **)args);
	ms_export(&mshell, (char **)arg_1);
	(void)arg_1;
	return (cleanup(&mshell, true, 0));
}
*/
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 11:04:40 by odudniak          #+#    #+#             */
/*   Updated: 2024/04/26 10:38:28 by odudniak         ###   ########.fr       */
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
// TODO:  create `chr_isspecial` which will check if the given character is
//		  a special one.
static void	ms_export_print(t_var *mshell)
{
	t_list	*lst;
	char	*val;
	bool	addquote;

	lst = mshell->env;
	while (lst)
	{
		printf("declare -x %s", (char *)lst->key);
		val = (char *)lst->val;
		addquote = !val || !val[0] || str_includesset(val, "\" '\t\n\v\r*;$@");
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
	if (len == 1)
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
	const char	*input = "''exp\"ort\" a=   '''b='   c='  ' d='\"' 'e' 'f= '";
	char		**args;
	const char	*arg_1[] = {"export", NULL};

	(void)av;
	mshell = (t_var){0};
	if (ac != 1)
		return (pf_errcode(ERR_INVALID_ARGC), cleanup(&mshell, true, 1), 1);
	mshell._main.envp = envp;
	ms_init(&mshell);
	ft_printf(COLOR_MAGENTA"Initial input:\t%s\n"CR, input);
	args = cmd_parse((char *)input);
	ms_export(&mshell, args);
	ms_export(&mshell, (char **)arg_1);
	return (cleanup(&mshell, true, 0));
}
*/
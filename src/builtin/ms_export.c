/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 11:04:40 by odudniak          #+#    #+#             */
/*   Updated: 2024/05/03 13:58:26 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

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

int	ms_export_checkarg(char *arg)
{
	int		i;
	bool	valid;
	int		equal_idx;

	i = 0;
	valid = true;
	equal_idx = str_idxofstr(arg, "+=");
	if (equal_idx == -1)
		equal_idx = str_idxofchar(arg, '=');
	while (arg && ((equal_idx == -1 && arg[i]) || i < equal_idx))
	{
		if (i == 0 && (!ft_isalpha(arg[0]) && arg[0] != '_'))
			valid = false;
		else if (!ft_isalnum(arg[i]) && arg[i] != '_')
			valid = false;
		if (!valid)
			break ;
		i++;
	}
	if (!valid)
	{
		ft_perror("export: `%s`: not a valid identifier\n", arg);
		return (KO);
	}
	return (OK);
}

int	ms_export_handler(t_var *mshell, char **args, int idx)
{
	char	**split;
	t_list	*found;
	t_list	*node;

	if (str_idxofstr(args[idx], "+=") != -1)
		split = str_split_firststr(args[idx], "+=");
	else
		split = str_split_first(args[idx], '=');
	if (!split)
		return (ms_export_error(mshell, args, NULL));
	found = lst_findbykey_str(mshell->env, split[0]);
	node = found;
	if (!node)
		node = lst_new(split[1], split[0]);
	if (!node)
		return (ms_export_error(mshell, args, split));
	if (!split[1])
		node->_hidden = true;
	if (str_idxofstr(args[idx], "+=") != -1 && found)
		node->val = str_freejoin(node->val, split[1]);
	if (str_idxofstr(args[idx], "+=") != -1 && found)
		free(split[1]);
	if (!lst_addnode_tail(&mshell->env, node))
		return (ms_export_error(mshell, args, split));
	return (free(split), OK);
}

int	ms_export(t_var *mshell, char **args)
{
	const int	len = str_mtxlen(args);
	int			i;
	int			curr;
	int			res;

	if (DEBUG)
	{
		dbg_printf(COLOR_CYAN"[ms_export] DEBUG:\n"CR);
		ft_putstrmtx(args);
	}
	res = OK;
	if (len == 1)
		return (ms_export_print(mshell), OK);
	i = 0;
	while (args && args[++i])
	{
		curr = ms_export_checkarg(args[i]);
		res = res && curr;
		if (curr == OK)
			ms_export_handler(mshell, args, i);
	}
	return (res);
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
	str_freemtx(args);

	args = cmd_parse("export c+=1");
	ms_export(&mshell, args);
	str_freemtx(args);

	args = cmd_parse("export c1= c_ _c");
	ms_export(&mshell, args);
	str_freemtx(args);

	args = cmd_parse("export 1c1= .c_ x55 !_c");
	ms_export(&mshell, args);
	str_freemtx(args);

	ms_export(&mshell, (char **)arg_1);
	return (cleanup(&mshell, true, 0));
}
*/
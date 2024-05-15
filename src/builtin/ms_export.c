/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 11:04:40 by odudniak          #+#    #+#             */
/*   Updated: 2024/05/12 15:51:29 by odudniak         ###   ########.fr       */
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

static int	ms_export_error(t_var *mshell, t_list *args, char **split)
{
	lst_free(&args, free);
	str_freemtx(split);
	pf_errcode(ERR_MALLOC);
	cleanup(mshell, true, 1);
	return (KO);
}

int	ms_export_checkarg(t_list *arg)
{
	const char	*val = (char *)arg->val;
	int			i;
	bool		valid;
	int			equal_idx;

	i = 0;
	equal_idx = str_idxofstr(val, "+=");
	if (equal_idx == -1)
		equal_idx = str_idxofchar(val, '=');
	valid = str_ilen(val) > 0;
	while (valid && arg && ((equal_idx == -1 && val[i]) || i < equal_idx))
	{
		if (i == 0 && (!ft_isalpha(val[0]) && val[0] != '_'))
			valid = false;
		else if (!ft_isalnum(val[i]) && val[i] != '_')
			valid = false;
		if (!valid)
			break ;
		i++;
	}
	if (!valid)
	{
		ft_perror("export: `%s`: not a valid identifier\n", arg->val);
		return (KO);
	}
	return (OK);
}

int	ms_export_handler(t_var *mshell, t_list *args, t_list *arg)
{
	char		**split;
	t_list		*found;
	t_list		*node;
	char		*val;

	if (str_idxofstr((char *)arg->val, "+=") != -1)
		split = str_split_firststr((char *)arg->val, "+=");
	else
		split = str_split_first((char *)arg->val, '=');
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
	if (found)
		free(split[0]);
	if (str_idxofstr((char *)arg->val, "+=") != -1 && found)
	{
		node->val = str_freejoin(node->val, split[1]);
		free(split[1]);
	}
	else if (str_idxofchar((char *)arg->val, '=') != -1 && found)
	{
		val = node->val;
		node->val = split[1];
		free(val);
	}
	if (!found && !lst_addnode_tail(&mshell->env, node))
		return (ms_export_error(mshell, args, split));
	return (free(split), OK);
}

int	ms_export(t_var *mshell, t_list *args)
{
	t_list			*argsp;
	const int		len = lst_size(args);
	int				curr;
	int				res;

	if (DEBUG)
	{
		dbg_printf(COLOR_CYAN"[export]\tDEBUG:\n"CR);
		lst_printstr(args);
	}
	if (!args)
		return (KO);
	res = OK;
	if (len == 1)
		return (ms_export_print(mshell), OK);
	argsp = args->next;
	while (argsp)
	{
		curr = ms_export_checkarg(argsp);
		res = res && curr;
		if (curr == OK)
			ms_export_handler(mshell, args, argsp);
		argsp = argsp->next;
	}
	return (res);
}

int	main(int ac, char **av, char **envp)
{
	t_var		mshell;
	t_list		*args;

	(void)av;
	mshell = (t_var){0};
	if (ac != 1)
		return (pf_errcode(ERR_INVALID_ARGC), cleanup(&mshell, true, 1), 1);
	mshell._main.envp = envp;
	ms_init(&mshell);

	//args = cmd_parse_new("unset LS_COLORS TERMINATOR XDG_DATA_DIRS XDG_SESSION_PATH SESSION_MANAGER GIO_LAUNCHED_DESKTOP_FILE PAGER LESS SHLVL LANGUAGE GJS_DEBUG_TOPICS ZSH LOGNAME LANG");
	//args = expand_and_clear(args);
	//ms_unset(&mshell, args);
	//lst_free(&args, free);

	args = cmd_parse_new("''exp\"ort\" a= \" \" '' '''b='   c='  ' d='\"' 'e' 'f= '");
	args = expand_and_clear(args);
	ms_export(&mshell, args);
	lst_free(&args, free);

	//args = cmd_parse_new("env bla");
	//args = expand_and_clear(args);
	//ms_env(&mshell, args);
	//lst_free(&args, free);

	//args = cmd_parse_new("export c1= c_ _c");
	//args = expand_and_clear(args);
	//ms_export(&mshell, args);
	//lst_free(&args, free);

	//args = cmd_parse_new("export 1c1= .c_ x55 !_c");
	//args = expand_and_clear(args);
	//ms_export(&mshell, args);
	//lst_free(&args, free);

	//args = cmd_parse_new("export");
	//args = expand_and_clear(args);
	//ms_export(&mshell, args);
	//lst_free(&args, free);

	return (cleanup(&mshell, true, 0));
}

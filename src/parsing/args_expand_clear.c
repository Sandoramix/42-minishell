/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_expand_clear.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/05 14:24:26 by odudniak          #+#    #+#             */
/*   Updated: 2024/05/15 22:20:06 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>


char	chr_is_inside_quotes(char *str, int idx)
{
	int		i;
	int		end;

	if (!str || idx <= 0 || idx >= str_ilen(str) - 1)
		return (0);
	i = 0;
	while (i < idx && str[i])
	{
		if (chr_isquote(str[i]))
		{
			end = chr_quoteclose_idx(str, str[i], i);
			if (end == -1 || end > idx)
				return (str[i]);
			i = end;
		}
		i++;
	}
	return (0);
}

t_list	*arg_expand(t_var *mshell, t_list *arg)
{
	int		dollar_idx;
	int		end;
	int		len;
	char	*variable;
	t_list	*env;

	dollar_idx = str_idxofchar((char *)arg->val, '$');
	dbg_printf(COLOR_YELLOW"[arg_expand] of [%s]\n", arg->val);
	if (arg->prev && arg->prev->type == A_TOKEN)
	{
		if (arg->val)
			arg->_first_char = ((char *)arg->val)[0];
		return (arg);
	}
	len = str_ilen((char *)arg->val);
	while (dollar_idx != -1)
	{
		end = str_var_ending_idx((char *)arg->val, dollar_idx);
		dbg_printf("\tFound a $ at [%3d] - [%3d]\n", dollar_idx, end);
		if (chr_is_inside_quotes((char *)arg->val, dollar_idx) == '\'')
		{
			dbg_printf(COLOR_MAGENTA"\t\tIt's inside single quote ('). Skipping\n");
			dollar_idx = str_idxofchar((char *)arg->val + dollar_idx + 1, '$');
			continue ;
		}
		if (end == len || end - 1 == dollar_idx)
		{
			dbg_printf(COLOR_MAGENTA"\t\tIt's does not have a good ending idx\n");
			dollar_idx = str_idxofchar((char *)arg->val + dollar_idx + 1, '$');
			continue ;
		}
		variable = my_substr((char *)arg->val, dollar_idx + 1, end);
		if (!variable)
			return (pf_errcode(ERR_MALLOC), cleanup(mshell, true, KO), NULL);
		dbg_printf(COLOR_GRAY"\t\tVariable to search: [%s]\n", variable);
		env = lst_findbykey_str(mshell->env, variable);
		free(variable);
		if (env)
		{
			dbg_printf(COLOR_GREEN"\t\tVariable found with value: [%s]\n", env->val);
			arg->val = str_replace_from_to((char *)arg->val, dollar_idx, end, env->val);
			if (!arg->val)
				return (pf_errcode(ERR_MALLOC), cleanup(mshell, true, KO), NULL);
		}
		else
		{
			dbg_printf(COLOR_GREEN"\t\tVariable not found, replacing with \"\"\n");
			arg->val = str_replace_from_to((char *)arg->val, dollar_idx, end, "");
			end = dollar_idx;
			if (!arg->val)
				return (pf_errcode(ERR_MALLOC), cleanup(mshell, true, KO), NULL);
		}
		dollar_idx = str_idxofchar((char *)arg->val + end, '$');
	}
	return (arg);
}

t_list	*expand_and_clear(t_var *mshell, t_list *args)
{
	t_list	*arg;

	arg = args;
	(void)mshell;
	while (arg)
	{
		dbg_printf(COLOR_CYAN"[expand_clear]\t|%s|:\n"CR, arg->val);
		if (arg->val)
		{
			if (!arg_expand(mshell, arg))
				return (lst_free(&args, free));
			if (!str_clearquotes((char **)&arg->val))
				return (lst_free(&args, free));
		}
		dbg_printf(COLOR_CYAN"\tresult|%s|\n"CR, arg->val);
		arg = arg->next;
	}
	return (args);
}

char	**expand_and_clear_mtx(t_var *mshell, char **args)
{
	int		i;

	i = -1;
	(void)mshell;
	while (args && args[++i])
	{
		dbg_printf(COLOR_CYAN"[expand_clear_mtx]\t|%s|\n"CR, args[i]);
		// TODO add expansion
		if (args[i] && !str_clearquotes(&(args[i])))
			return (str_freemtx(args));
		dbg_printf(COLOR_CYAN"\t|%s|\n"CR, args[i]);
	}
	return (args);
}
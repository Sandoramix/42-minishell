/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_pre_parse.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 20:53:10 by odudniak          #+#    #+#             */
/*   Updated: 2024/06/22 21:50:36 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>


static void	skip_closing_quotes(char *value, int *i)
{
	int	close_idx;

	if (!value)
		return ;
	close_idx = chr_quoteclose_idx(value, *i);
	while (value[*i] && chr_isquote(value[*i]) && close_idx != -1)
	{
		*i = close_idx + 1;
		close_idx = chr_quoteclose_idx(value, *i);
	}
}

static char	*escape_special_chars(char *value)
{
	char	*tmp;
	int		i;

	i = -1;
	while (value && value[++i])
	{
		skip_closing_quotes(value, &i);
		if ((chr_istoken(value[i]) || (chr_isquote(value[i])
					&& chr_quoteclose_idx(value, i) == -1))
				&& !str_ischar_inquotes(value, i))
		{
			dbg_printf("escaping: [%c] (i=%d; len=%d)\n", value[i], i, str_ilen(value));
			tmp = ft_calloc(str_ilen(value) + 4, sizeof(char));
			if (!tmp)
				return (free(value), NULL);
			str_lcpy(tmp, value, i + 1);
			tmp[i] = (char [2]){'"', '\''}[value[i] == '"'];
			tmp[i + 1] = value[i];
			tmp[i + 2] = (char [2]){'"', '\''}[value[i] == '"'];
			dbg_printf("escaped: [%s] saf[%s] stuff_after[%d]\n", tmp, value + i + 1, str_ilen(value + i + 1));
			str_lcat(tmp, value + i + 1, str_ilen(value) + 4);
			free(value);
			value = tmp;
			i = -1;
		}
	}
	return (value);
}

static char	*replace_variable_w_value(t_var *mshell,
	char **arg_p, int *dollar_idx, int *end_idx)
{
	char	*variable;
	t_list	*env;
	char	*val;

	variable = str_substr(*arg_p, *dollar_idx + 1, *end_idx);
	if (!variable)
		return (pf_errcode(E_MALLOC), NULL);
	if (str_equals(variable, "?"))
		return (free(variable), expand_statuscode(arg_p, dollar_idx, end_idx));
	env = lst_findbykey_str(mshell->env, variable);
	free(variable);
	if (!env)
		return (expand_empty(arg_p, dollar_idx, end_idx));
	dbg_printf(CGREEN"\tVariable found with value: `%s`\n"CR, env->val);
	dbg_printf(CGRAY"Escaping special chars (tokens/quotes)...\n");
	val = escape_special_chars(str_dup(env->val));
	if (!val)
		return (pf_errcode(E_MALLOC), NULL);
	*arg_p = str_replace_from_to(*arg_p, *dollar_idx, *end_idx, val);
	if (!(*arg_p))
		return (pf_errcode(E_MALLOC), free(val), NULL);
	*dollar_idx = str_idxofchar(*arg_p, '$');
	return (free(val), *arg_p);
}

static char	*expand_variable(t_var *mshell, char **arg, int *d_idx)
{
	const int		len = str_ilen((*arg));
	int				end;

	if (*d_idx == -1)
		return (*arg);
	end = str_var_ending_idx((*arg), (*d_idx));
	dbg_printf(CBGRAY"Found a $ at [%3d] - [%3d]\n"CR, (*d_idx), end, len);
	if (str_ischar_inquotes((*arg), (*d_idx)) == '\'')
	{
		(*d_idx) = str_idxofchar_from((*arg), (*d_idx) + 1, '$');
		return (dbg_printf(CMAGENTA"\t\tIt's inside single quote.\n"), *arg);
	}
	if (end == *d_idx && chr_isquote(((*arg))[*d_idx + 1]))
	{
		(*arg) = str_replace_from_to((*arg), (*d_idx), (end), "");
		(*d_idx) = str_idxofchar_from((*arg), (*d_idx) + 1, '$');
		return (*arg);
	}
	else if (end == *d_idx)
	{
		(*d_idx) = str_idxofchar_from((*arg), (*d_idx) + 1, '$');
		return (*arg);
	}
	return (replace_variable_w_value(mshell, arg, d_idx, &end));
}

char	*pre_parse_expand(t_var *mshell, char **line)
{
	int		dollar_idx;
	char	*res;

	res = *line;
	if (!line || !*line)
		return (NULL);
	dollar_idx = str_idxofchar(*line, '$');
	dbg_printf(CBYELLOW"[pre_parse_expand] of [%s]\n"CR, *line);
	while (dollar_idx != -1)
	{
		res = expand_variable(mshell, line, &dollar_idx);
		if (!res)
			return (NULL);
	}
	*line = res;
	dbg_printf(CYELLOW"\tresult `%s`\n", *line);
	return (res);
}

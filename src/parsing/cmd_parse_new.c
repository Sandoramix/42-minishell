/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_parse_new.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 13:16:03 by odudniak          #+#    #+#             */
/*   Updated: 2024/05/08 19:21:29 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

typedef enum e_cmd_parseswitch
{
	CMDP_SPACE,
	CMDP_QUOTE,
	CMDP_TOKEN,
	CMDP_WORD
}	t_cmd_parseswitch;

// TODO: add me to libftx
static void	cmd_dbgparse(t_cmd_parseswitch type, char *s, int i, int edge)
{
	if (type == CMDP_QUOTE)
		dbg_printf("Found a quote {%c} at:\t[%3d] - [%3d]:\t", s[i], i, edge);
	else if (type == CMDP_TOKEN || type == CMDP_WORD)
		dbg_printf("Found unquoted %s at:\t[%3d] - [%3d]:\t",
			(char *[2]){"TOKEN", "WORD"}[type == CMDP_WORD], edge, i);
	else
		return ;
	write(1, "[", 1);
	if (type == CMDP_QUOTE)
		write(1, s + i, edge - i + 1);
	else
		write(1, s + edge, i - edge + 1);
	write(1, "]\n", 2);
}

static bool	lst_strappend_last(t_list **res, char *append, char dbg_char)
{
	t_list	*last;

	dbg_printf("\tIt's near the char %c\n", dbg_char);
	last = lst_gettail(*res);
	if (!last && !lst_addnew_tail(res, append, NULL))
		return (lst_free(res, free), free(append), false);
	else
	{
		last->val = str_freejoin(last->val, append);
		free(append);
		if (!last->val)
			return (lst_free(res, free), false);
	}
	return (true);
}

static bool	cmdp_switch(t_cmd_parseswitch type, char *raw, int *i, int *edge)
{
	if (type == CMDP_QUOTE)
	{
		if (!chr_isquote(raw[*i]))
			return (false);
		(*edge) = chr_quoteclose_idx(raw, raw[*i], *i);
		if ((*edge) == -1)
			(*edge) = str_ilen(raw);
	}
	cmd_dbgparse(type, raw, *i, *edge);
	if (type == CMDP_QUOTE)
		return (true);
	if (type == CMDP_SPACE)
		return (ft_isspace(raw[*i]) && !ft_isspace(raw[*i + 1]));
	if (type == CMDP_TOKEN)
		return (chr_istoken(raw[*i]) && (ft_isspace(raw[*i + 1])
				|| !raw[*i + 1] || chr_isquote(raw[*i + 1])));
	if (type == CMDP_WORD)
		return (!ft_isspace(raw[*i]) && ((ft_isspace(raw[*i + 1])
					|| !raw[*i + 1] || chr_isquote(raw[*i + 1]))
				|| chr_istoken(raw[*i + 1])));
	dbg_printf("[cmd_switch]: Unknown TYPE [%d]!\n", type);
	return (false);
}

t_list	*cmd_parse_new(char *raw)
{
	t_list		*res;
	char		*tmp;
	int			i;
	int			edgeidx;

	res = NULL;
	i = -1;
	edgeidx = 0;
	dbg_printf(COLOR_RED"[cmd_parse] input: `%s`\n"CR, raw);
	while (raw && raw[++i])
	{
		if (cmdp_switch(CMDP_SPACE, raw, &i, &edgeidx))
			edgeidx = i + 1;
		else if (cmdp_switch(CMDP_QUOTE, raw, &i, &edgeidx))
		{
			tmp = my_substr(raw, i, edgeidx);
			if (!tmp)
				return (lst_free(&res, free), NULL);
			if (i != 0 && !ft_isspace(raw[i - 1]) && !chr_istoken(raw[i - 1]))
			{
				if (!lst_strappend_last(&res, tmp, raw[i - 1]))
					return (NULL);
			}
			else if (!lst_addnew_tail(&res, tmp, NULL))
				return (lst_free(&res, free), free(tmp), NULL);
			if (edgeidx - 1 == i)
			{
				i++;
				edgeidx++;
			}
			else
				i = edgeidx;
		}
		else if (cmdp_switch(CMDP_TOKEN, raw, &i, &edgeidx))
		{
			tmp = my_substr(raw, edgeidx, i);
			if (!tmp || !lst_addnew_tail(&res, tmp, NULL))
				return (lst_free(&res, free), free(tmp), NULL);
			edgeidx = i + 1;
		}
		else if (cmdp_switch(CMDP_WORD, raw, &i, &edgeidx))
		{
			tmp = my_substr(raw, edgeidx, i);
			if (!tmp)
				return (lst_free(&res, free), NULL);
			if (i != 0 && edgeidx != 0 && !ft_isspace(raw[edgeidx - 1]) && !chr_istoken(raw[edgeidx - 1]))
			{
				if (!lst_strappend_last(&res, tmp, raw[edgeidx - 1]))
					return (NULL);
			}
			else if (!lst_addnew_tail(&res, tmp, NULL))
				return (lst_free(&res, free), free(tmp), NULL);
			edgeidx = i + 1;
		}
	}
	return (res);
}

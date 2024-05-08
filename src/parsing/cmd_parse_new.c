/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_parse_new.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 13:16:03 by odudniak          #+#    #+#             */
/*   Updated: 2024/05/08 15:06:12 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

// TODO: add me to libftx
static void	print_fromto(char *s, int start, int end)
{
	if (end < start)
		return ;
	write(1, "[", 1);
	write(1, s + start, end - start + 1);
	write(1, "]", 1);
}

static bool	concat_arg(t_list **res, char *concat)
{
	t_list	*last;

	last = lst_gettail(*res);
	if (!last && !lst_addnew_tail(res, concat, NULL))
		return (lst_free(res, free), free(concat), false);
	else
	{
		last->val = str_freejoin(last->val, concat);
		free(concat);
		if (!last->val)
			return (lst_free(res, free), false);
	}
	return (true);
}

t_list	*cmd_parse_new(char *raw)
{
	t_list		*res;
	t_list		*last;
	char		*tmp;
	int			i;
	int			edgeidx;

	res = NULL;
	i = -1;
	edgeidx = 0;
	dbg_printf(COLOR_RED"[cmd_parse] input: `%s`\n"CR, raw);
	while (raw && raw[++i])
	{
		if (ft_isspace(raw[i]) && !ft_isspace(raw[i + 1]))
			edgeidx = i + 1;
		else if (chr_isquote(raw[i]))
		{
			edgeidx = chr_quoteclose_idx(raw, raw[i], i);
			dbg_printf("Found a quote {%c} at:\t[%3d] - [%3d]:\t", raw[i], i, edgeidx);
			print_fromto(raw, i, edgeidx);
			dbg_printf("\n");
			if (edgeidx == -1)
				edgeidx = str_ilen(raw);
			tmp = my_substr(raw, i, edgeidx);
			if (!tmp)
				return (lst_free(&res, free), NULL);
			if (i != 0 && !ft_isspace(raw[i - 1]) && !chr_istoken(raw[i - 1]))
			{
				last = lst_gettail(res);
				dbg_printf("\tIt's near the char %c\n", raw[i - 1]);
				if (!concat_arg(&res, tmp))
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
		else if (chr_istoken(raw[i]) && (ft_isspace(raw[i + 1]) || !raw[i + 1] || chr_isquote(raw[i + 1])))
		{
			dbg_printf("Found unquoted TOKEN at:\t[%3d] - [%3d]:\t", edgeidx, i);
			print_fromto(raw, edgeidx, i);
			dbg_printf("\n");
			tmp = my_substr(raw, edgeidx, i);
			if (!tmp)
				return (lst_free(&res, free), NULL);
			else if (!lst_addnew_tail(&res, tmp, NULL))
				return (lst_free(&res, free), free(tmp), NULL);
		}
		else if ((!ft_isspace(raw[i]) && !chr_istoken(raw[i])) && (ft_isspace(raw[i + 1]) || !raw[i + 1] || chr_isquote(raw[i + 1]) || chr_istoken(raw[i + 1])))
		{
			dbg_printf("Found unquoted WORD at:\t[%3d] - [%3d]:\t", edgeidx, i);
			print_fromto(raw, edgeidx, i);
			dbg_printf("\n");
			tmp = my_substr(raw, edgeidx, i);
			if (!tmp)
				return (lst_free(&res, free), NULL);
			if (i != 0 && edgeidx != 0 && !ft_isspace(raw[edgeidx - 1]) && !chr_istoken(raw[edgeidx - 1]))
			{
				dbg_printf("\tIt's near the char %c\n", raw[edgeidx - 1]);
				if (!concat_arg(&res, tmp))
					return (NULL);
			}
			else if (!lst_addnew_tail(&res, tmp, NULL))
				return (lst_free(&res, free), free(tmp), NULL);
			edgeidx = i + 1;
		}
	}
	return (res);
}

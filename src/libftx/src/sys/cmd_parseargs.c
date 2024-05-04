/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_parseargs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 13:16:03 by odudniak          #+#    #+#             */
/*   Updated: 2024/05/04 20:09:29 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
// TODO: add me to libftx

static void	print_fromto(char *s, int start, int end)
{
	if (end < start)
		return ;
	write(1, "[", 1);
	write(1, s + start, end - start + 1);
	write(1, "]", 1);
}

char	**cmd_parse(char *raw)
{
	int			residx;
	char		**res;
	char		*tmp;
	int			edgeidx;
	int			i;

	res = NULL;
	i = -1;
	edgeidx = 0;
	residx = -1;
	while (raw && raw[++i])
	{
		if (chr_isquote(raw[i]))
		{
			edgeidx = chr_quoteclose_idx(raw, raw[i], i);
			dbg_printf("Found a quote {%c} at:\t[%3d] - [%3d]:\t", raw[i], i, edgeidx);
			print_fromto(raw, i, edgeidx);
			dbg_printf("\n");
			if (edgeidx == -1)
				edgeidx = str_ilen(raw);
			if (i + 1 > edgeidx - 1)
				tmp = ft_calloc(1, sizeof(char));
			else
				tmp = my_substr(raw, i + 1, edgeidx - 1);
			if (!tmp)
				return (str_freemtx(res), NULL);
			if (i != 0 && !ft_isspace(raw[i - 1]))
			{
				dbg_printf("\tIt's near the char %c\n", raw[i - 1]);
				res[residx] = str_freejoin(res[residx], tmp);
				free(tmp);
				if (!res[residx])
					return (str_freemtx(res), NULL);
			}
			else if (!str_mtxpush(&res, tmp) || ++residx < -1)
				return (str_freemtx(res), free(tmp), NULL);
			if (edgeidx - 1 == i)
			{
				i++;
				edgeidx++;
			}
			else
				i = edgeidx;
		}
		else if (ft_isspace(raw[i]) && !ft_isspace(raw[i + 1]))
			edgeidx = i + 1;
		else if (!ft_isspace(raw[i]) && (ft_isspace(raw[i + 1]) || !raw[i + 1] || chr_isquote(raw[i + 1])))
		{
			dbg_printf("Found unquoted word at:\t[%3d] - [%3d]:\t", edgeidx, i);
			print_fromto(raw, edgeidx, i);
			dbg_printf("\n");
			tmp = my_substr(raw, edgeidx, i);
			if (!tmp || !str_mtxpush(&res, tmp) || ++residx < -1)
				return (str_freemtx(res), free(tmp), NULL);
		}
	}
	return (res);
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
		if (chr_isquote(raw[i]))
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
				if (!last && !lst_addnew_tail(&res, tmp, NULL))
					return (lst_free(&res, free), free(tmp), NULL);
				else
				{
					last->val = str_freejoin(last->val, tmp);
					free(tmp);
					if (!last->val)
						return (lst_free(&res, free), NULL);
				}
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
		else if (ft_isspace(raw[i]) && !ft_isspace(raw[i + 1]))
			edgeidx = i + 1;
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
				last = lst_gettail(res);
				if (!last && !lst_addnew_tail(&res, tmp, NULL))
					return (lst_free(&res, free), free(tmp), NULL);
				else
				{
					last->val = str_freejoin(last->val, tmp);
					free(tmp);
					if (!last->val)
						return (lst_free(&res, free), NULL);
				}
			}
			else if (!lst_addnew_tail(&res, tmp, NULL))
				return (lst_free(&res, free), free(tmp), NULL);
			edgeidx = i + 1;
		}
	}
	return (res);
}

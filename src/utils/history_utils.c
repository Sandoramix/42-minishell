/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 18:48:20 by odudniak          #+#    #+#             */
/*   Updated: 2024/06/22 11:02:02 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	print_history(t_list *history)
{
	t_list	*current;
	int		i;

	i = 1;
	current = history;
	while (current != NULL)
	{
		ft_printf("%d %s\n", i, (char *)current->val);
		current = current->next;
		i++;
	}
}

t_state	add_history_line(t_var *mshell, char *line)
{
	t_list	*lst;
	char	*cmd_copy;

	cmd_copy = str_dup(line);
	if (!cmd_copy)
		return (pf_errcode(E_MALLOC), free(line),
			cleanup(mshell, true, 1), KO);
	lst = lst_addnew_tail(&mshell->history, cmd_copy, NULL);
	if (!lst)
		return (pf_errcode(E_MALLOC), free(line), free(cmd_copy),
			cleanup(mshell, true, 1), KO);
	return (OK);
}

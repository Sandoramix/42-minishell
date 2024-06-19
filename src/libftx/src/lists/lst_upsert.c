/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_upsert.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 20:35:11 by odudniak          #+#    #+#             */
/*   Updated: 2024/06/19 20:39:05 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

t_list	*lst_upsert_str(t_list **head, char *key, char *value)
{
	t_list	*found;

	if (!head)
		return (NULL);
	found = lst_findbykey_str(*head, key);
	if (found)
	{
		free(found->val);
		found->val = str_dup(value);
		if (!found->val)
			return (NULL);
	}
	else
		*head = lst_addnew_tail(head, value, key);
	return (*head);
}

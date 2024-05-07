/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chr_closequote_idx.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 16:54:00 by odudniak          #+#    #+#             */
/*   Updated: 2024/05/04 16:54:25 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

int	chr_quoteclose_idx(char *s, char quote_opener, int start)
{
	while (s && s[++start])
		if (s[start] == quote_opener)
			return (start);
	return (-1);
}

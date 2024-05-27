/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_substr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 21:25:00 by odudniak          #+#    #+#             */
/*   Updated: 2024/05/27 21:31:36 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*str_lensubstr(char *s, size_t start, size_t len)
{
	char	*res;
	size_t	s_len;

	s_len = str_ulen(s);
	if (len > s_len)
		len = s_len;
	if (start > s_len)
	{
		len = 0;
		start = s_len;
	}
	if (start + len > s_len)
		len = s_len - start;
	res = ft_calloc(len + 1, sizeof(char));
	if (!res)
		return (NULL);
	ft_memcpy(res, &s[start], len);
	return (res);
}

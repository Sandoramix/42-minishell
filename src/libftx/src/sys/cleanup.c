/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 16:18:16 by odudniak          #+#    #+#             */
/*   Updated: 2024/04/17 16:19:12 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

// TODO IMPLEMENT ME
void	cleanup(t_var *t, bool should_exit, int statuscode)
{
	(void)t;
	if (DEBUG)
		ft_printf(COLOR_GREEN"Cleanup finished\n"CR);
	if (should_exit)
		exit(statuscode);
}

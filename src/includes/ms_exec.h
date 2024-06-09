/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_exec.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 14:18:15 by odudniak          #+#    #+#             */
/*   Updated: 2024/06/09 14:18:49 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MS_EXEC_H
# define MS_EXEC_H
# include <libft.h>

void	*ms_exec_commands(t_var *mshell, t_list *all);

bool	ms_wrap_commands(t_var *mshell);

#endif

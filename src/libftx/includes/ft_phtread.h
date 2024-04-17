/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_phtread.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 15:57:12 by odudniak          #+#    #+#             */
/*   Updated: 2024/04/17 16:13:30 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PHTREAD_H
# define FT_PHTREAD_H

# include <libft.h>

typedef enum e_mutex_handle
{
	MUTEX_UNLOCK,
	MUTEX_LOCK
}	t_mutex_handle;

// THE FOLLOWING FUNCTIONS WILL CALL "CLEANUP" ON `t` ON ANY ERROR.

/** @brief initialize a mutex */
int		mutex_init(t_var *t, t_mutex *m);
/** @brief lock a mutex */
int		mutex_lock(t_var *t, t_mutex *m);
/** @brief unlock a mutex */
int		mutex_unlock(t_var *t, t_mutex *m);

/*
 * Get the `val` (thread safe)
 * by locking & unlocking the given mutex.
*/
long	mutget_long(t_var *t, t_mutex *m, long *val);
int		mutget_int(t_var *t, t_mutex *m, int *val);
bool	mutget_bool(t_var *t, t_mutex *m, bool *val);
t_ulong	mutget_ulong(t_var *t, t_mutex *m, t_ulong *val);

/*
 * Set the `val` (thread safe)
 * by locking & unlocking the given mutex.
*/
long	mutset_long(t_var *t, t_mutex *m, long *val, long new_val);
bool	mutset_bool(t_var *t, t_mutex *m, bool *val, bool new_val);
t_ulong	mutset_ulong(t_var *t, t_mutex *m, t_ulong *val, t_ulong new_val);

/*
 * Set the `val` (thread safe)
 * by locking & unlocking the given mutex.
*/
long	mutinc_long(t_var *t, t_mutex *m, long *val);
int		mutinc_int(t_var *t, t_mutex *m, int *val);
t_ulong	mutinc_ulong(t_var *t, t_mutex *m, t_ulong *val);

#endif

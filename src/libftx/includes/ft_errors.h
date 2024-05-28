/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_errors.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marboccu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 07:47:06 by odudniak          #+#    #+#             */
/*   Updated: 2024/05/28 23:26:11 by marboccu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_ERRORS_H
# define FT_ERRORS_H

/**
 * @brief Enum for error codes.
 * @param ERR_DEFAULT default error message.
 * @param ERR_INVALID_ARGC invalid number of arguments.
 * @param ERR_MALLOC Malloc allocation failure.
 * @param ERR_FILE_NOT_FOUND File not found.
 * @param ERR_FILE_PERMISSION_DENIED Permissions denied on a file.
 * @param ERR_ENV_LOAD Error loading environment variables.
 * @param ERR_PATH_LOAD Error loading PATH paths.
 * @param ERR_FORK Fork failure.
 * @param ERR_PIPE Pipe failure.
 * @param ERR_DUP2 Dup2 failure.
 * @param ERR_EXECVE Execve failure.
 */
typedef enum e_errorcode
{
	ERR_DEFAULT,
	ERR_INVALID_ARGC,
	ERR_MALLOC,
	ERR_FILE_NOT_FOUND,
	ERR_FILE_PERMISSION_DENIED,
	ERR_ENV_LOAD,
	ERR_PATH_LOAD,
	ERR_FORK,
	ERR_PIPE,
	ERR_DUP2,
	ERR_EXECVE,
	ERR_SYNTAX,
}	t_errorcode;

#endif

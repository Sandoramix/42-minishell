/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_errors.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 07:47:06 by odudniak          #+#    #+#             */
/*   Updated: 2024/06/12 10:33:33 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_ERRORS_H
# define FT_ERRORS_H

/**
 * @brief Enum for error codes.
 * @param E_DEFAULT default error message.
 * @param E_INVALID_ARGC invalid number of arguments.
 * @param E_MALLOC Malloc allocation failure.
 * @param E_FILE_NOT_FOUND File not found.
 * @param E_FILE_PERMISSION_DENIED Permissions denied on a file.
 * @param E_ENV_LOAD Error loading environment variables.
 * @param E_PATH_LOAD Error loading PATH paths.
 * @param E_FORK Fork failure.
 * @param E_PIPE Pipe failure.
 * @param E_DUP2 Dup2 failure.
 * @param E_EXECVE Execve failure.
 * @param E_SYNTAX Syntax error (minishell).
 */
typedef enum e_errorcode
{
	E_DEFAULT,
	E_INVALID_ARGC,
	E_MALLOC,
	E_FILE_NOT_FOUND,
	E_FILE_PERMISSION_DENIED,
	E_ENV_LOAD,
	E_PATH_LOAD,
	E_FORK,
	E_PIPE,
	E_DUP2,
	E_EXECVE,
	E_SYNTAX,
}	t_errorcode;

#endif

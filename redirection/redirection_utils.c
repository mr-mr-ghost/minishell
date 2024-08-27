/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gklimasa <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 10:34:26 by gklimasa          #+#    #+#             */
/*   Updated: 2024/08/16 01:23:58 by gklimasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* delete array element at index and shift the rest of the array*/
void	delete_array_element(char **array, int index)
{
	int	i;

	i = index;
	free(array[index]);
	while (array[i] != NULL)
	{
		array[i] = array[i + 1];
		i++;
	}
}

/* TODO: <<*/
/* example of << delimiter: "cat <<'X' > t.txt		contentbla X"*/
/* TODO: >>*/
/* handle input/output redirection  (> and <)*/
int	handle_redirection(t_token *fname, int type)
{
	int	fd;

/*	printf("type: %d, file name: %s\n", type, fname->value);*/
/*	 TODO: handle different file permissions (with access() probably)*/
	if (type == TRUNC)
		fd = open(fname->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (type == APPEND)
		fd = open(fname->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(fname->value, O_RDONLY);
	if (fd < 0)
	{
		perror("open");
		return (-1);
	}
	if (type == TRUNC || type == APPEND)
	{
		if (dup2(fd, STDOUT_FILENO) < 0)
		{
			perror("dup2");
			close(fd);
			return (-1);
		}
	}
	else
	{
		if (dup2(fd, STDIN_FILENO) < 0)
		{
			perror("dup2");
			close(fd);
			return (-1);
		}
	}
	close(fd);
	return (0);
}

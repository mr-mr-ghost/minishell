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

#include "minishell.h"

// delete array element at index and shift the rest of the array
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

// TODO: <<
// example of << delimiter: "cat <<'X' > t.txt		contentbla X"
// TODO: >>
// handle input/output redirection  (> and <)
int	handle_redirection(char **args)
{
	int	i;
	int	fd;

	i = 0;
	while (args[i] != NULL &&
		ft_memcmp(args[i], ">", 2) != 0 && ft_memcmp(args[i], "<", 2) != 0)
		i++;
	if (args[i] == NULL)
		return (1);
	if (args[i][0] == '>')
		fd = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(args[i + 1], O_RDONLY);
	if (fd < 0)
	{
		perror("open");
		return (0);
	}
	if (args[i][0] == '>')
		dup2(fd, STDOUT_FILENO);
	else
		dup2(fd, STDIN_FILENO);
	close(fd);
	//args[i] = NULL; // bad - cuts off free_cmd and the rest of the command
	delete_array_element(args, i); // delete redirection symbol
	delete_array_element(args, i); // delete redirection file
	return (1);
}

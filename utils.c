/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhoddy <jhoddy@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 16:38:58 by jhoddy            #+#    #+#             */
/*   Updated: 2024/08/09 16:38:58 by jhoddy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_cmd(t_data *data)
{
	int	i;

	if (data->cmd->line != NULL)
	{
		i = 0;
		while (data->cmd->line[i])
			free(data->cmd->line[i++]);
		free(data->cmd->line);
	}
	if (data->cmd)
		free(data->cmd);
}

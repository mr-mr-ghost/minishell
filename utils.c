/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gklimasa <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 16:38:58 by jhoddy            #+#    #+#             */
/*   Updated: 2024/08/14 16:46:30 by gklimasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_data_content(t_data *data)
{
	int	i;

	if (data->line)
	{
		free(data->line);
		data->line = NULL;
	}
	if (data->cmd)
	{
		i = 0;
		while (data->cmd[i])
			free(data->cmd[i++]);
		free(data->cmd);
		data->cmd = NULL;
	}
}

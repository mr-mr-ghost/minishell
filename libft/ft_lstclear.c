/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhoddy <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 13:09:05 by jhoddy            #+#    #+#             */
/*   Updated: 2024/02/26 13:19:33 by jhoddy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*c_node;
	t_list	*n_node;

	if (!lst || !*lst)
		return ;
	c_node = *lst;
	while (c_node != NULL)
	{
		n_node = c_node->next;
		if (del != NULL)
			del(c_node->content);
		free(c_node);
		c_node = n_node;
	}
	*lst = NULL;
}

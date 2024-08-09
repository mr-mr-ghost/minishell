/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhoddy <jhoddy@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 16:31:08 by jhoddy            #+#    #+#             */
/*   Updated: 2024/02/28 11:08:17 by jhoddy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		i;
	int		j;
	char	*s_ult;
	size_t	tlen;

	if (!s1 || !s2)
		return (NULL);
	tlen = ft_strlen(s1) + ft_strlen(s2);
	s_ult = malloc(tlen + 1);
	if (s_ult == NULL)
		return (NULL);
	i = 0;
	while (s1[i] != '\0')
	{
		s_ult[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j] != '\0')
	{
		s_ult[i + j] = s2[j];
		j++;
	}
	s_ult[i + j] = '\0';
	return (s_ult);
}

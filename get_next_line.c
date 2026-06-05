/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msellami <msellami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 15:16:07 by msellami          #+#    #+#             */
/*   Updated: 2025/11/29 09:30:41 by msellami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*read_into_buffer(int fd)
{
	char	*buffer;
	ssize_t	b_read;

	buffer = malloc((size_t)BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	b_read = read(fd, buffer, BUFFER_SIZE);
	if (b_read <= 0)
	{
		free(buffer);
		return (NULL);
	}
	buffer[b_read] = '\0';
	return (buffer);
}

static char	*fill_line_buffer(int fd, char *left_c)
{
	char	*buffer;
	char	*tmp;

	buffer = read_into_buffer(fd);
	while (buffer)
	{
		if (!left_c)
			left_c = ft_strdup("");
		tmp = left_c;
		left_c = ft_strjoin(tmp, buffer);
		free(tmp);
		free(buffer);
		if (!left_c)
			return (NULL);
		if (ft_strchr(left_c, '\n'))
			break ;
		buffer = read_into_buffer(fd);
	}
	return (left_c);
}

static char	*extract_line(char *line_buffer)
{
	char	*line;
	size_t	i;

	if (!line_buffer || !line_buffer[0])
		return (NULL);
	i = 0;
	while (line_buffer[i] && line_buffer[i] != '\n')
		i++;
	if (line_buffer[i] == '\n')
		line = ft_substr(line_buffer, 0, i + 1);
	else
		line = ft_strdup(line_buffer);
	return (line);
}

static char	*set_next_left_c(char *line_buffer)
{
	char	*new_left_c;
	size_t	i;

	if (!line_buffer || !line_buffer[0])
		return (NULL);
	i = 0;
	while (line_buffer[i] && line_buffer[i] != '\n')
		i++;
	if (!line_buffer[i])
	{
		free(line_buffer);
		return (NULL);
	}
	new_left_c = ft_substr(line_buffer, i + 1, ft_strlen(line_buffer) - i - 1);
	free(line_buffer);
	if (!new_left_c || new_left_c[0] == '\0')
	{
		free(new_left_c);
		return (NULL);
	}
	return (new_left_c);
}

char	*get_next_line(int fd)
{
	static char	*left_c;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	left_c = fill_line_buffer(fd, left_c);
	if (!left_c)
		return (NULL);
	line = extract_line(left_c);
	left_c = set_next_left_c(left_c);
	return (line);
}

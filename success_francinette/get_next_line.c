/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anqabbal <anqabbal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 08:55:00 by anqabbal          #+#    #+#             */
/*   Updated: 2023/12/22 21:47:20 by anqabbal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"



//this function just for joining the string and free the memory

char *to_join(char *s1, char *s2, int indice)
{
	char *str1;
	char *str2;

	str1 = s1;
	str2 = s2;
	s1 = ft_strjoin(s1, s2);
	free(str1);
	str1 = NULL;
	if (indice != 0)
	{
		free (str2);
		str2 = NULL;
	}
	return (s1);
}
//this function count the character before \n if indice == 0
//this function after the character after \n if indice == 1
//this indice 3 this function return the len of the line if the buffer contain '\n'
//this function returns the len of your buffer if the buffer doesn't contain '\n'


//(probably no leak here this function only Read the memory)
int	count_line(char *buffer, int indice)
{
	int len;
	char *str;
	int	str_len;
	
	if (!buffer)
		return(0);
	len = ft_strlen(buffer);
	str = ft_strchr(buffer, '\n');
	if (!str)
		return (len);
	str_len = ft_strlen(str);
	if (indice == 0)
		return ((len - str_len) + 1);
	else if (indice == 1)
		return (len - (len - str_len));
	else
		return (count_line(str + 1, 0));
}

//this function cut the character before \n if indice == 0
//this function cut the character after \n if indice == 1
//this indice 3 this function return the line if the buffer contain '\n'
//this function returns the len of your buffer if the buffer doesn't contain '\n'

//(here you need to check the leaks bcs you allocated a memory  for str, and read from the buffer )
char *cut_buffer(char *buffer, int indice)
{
	char *str;
	int b;

	b = 1;
	if (!buffer || (buffer && *buffer == '\0'))
		return (NULL);
	while (b--)
	{
		str = ft_calloc((count_line(buffer, indice) + 2), sizeof(char));
		if (!str)
			return (0);
		if (indice == 0)
			ft_memcpy(str, buffer, count_line(buffer, indice));
		else if (indice == 1)
			ft_memcpy(str, buffer + count_line(buffer, 0), count_line(buffer, indice));
	}
	if (str && str[0] == '\0')
	{
		free (str);
		str = NULL;
		return (0);
	}
	return (str);
}
//this funciton reads from the file a number of size required BUFFER_SIZE if it does't find any new line in the buffer continues reading until it find
//the character new line in the buffer and cut the character before the first newline with the new line and consider this is the line and return it 
char	*read_the_line(char **old_buffer, int fd, ssize_t *n)
{
	char *str1;
	char *buffer;

	*n = BUFFER_SIZE;
	str1 = *old_buffer;
	buffer = ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	if (!(buffer))
		return (0);
	while(*n == BUFFER_SIZE && *n)
	{
		*n = read(fd, buffer, BUFFER_SIZE);
		if (*n < 0 || *n == 0)
			break ;
		(buffer)[*n] = '\0';
		str1 = to_join(str1, cut_buffer(buffer, 0), 1);
		if (ft_strchr(buffer, '\n'))
			break;
	 }
	if (ft_strchr(buffer, '\n'))
		*old_buffer = cut_buffer(buffer, 1);
	else
		*old_buffer = NULL;
	free(buffer);
	return (str1);
}

char *get_next_line(int fd)
{
	static char *old_buffer;
	static char *old;
	char *tmp;
	ssize_t n;
	char *read_;
	char *line;
	
	read_ = NULL;
	n = 0;
	if (fd < 0 || BUFFER_SIZE == 0 || read(fd, 0, 0) < 0)
	{
		free(old_buffer);
		old_buffer = NULL;
		return (NULL);
	}
	if (ft_strchr(old_buffer, '\n'))
	{
		old = cut_buffer(old_buffer, 0);
		tmp = old_buffer;
		old_buffer = cut_buffer(old_buffer, 1);
		free(tmp);
		line = old;
	}
	else
	{
		read_ = read_the_line(&old_buffer, fd, &n);
		old = old_buffer;
		line = read_;
	}
	return (line);
}



//when I work with normal calloc I get some ko in normal test of francinette

#include <stdio.h>
#include <string.h>

//一些和字符串处理相关的函数放这里

//截取字符串尾部空格
void Rtrim(char *str)
{
	size_t len = 0;
	if (str == NULL)
		return;

	len = strlen(str);
	while (len > 0 && str[len - 1] == ' ')
		str[--len] = 0;
	return;
}

//截取字符串首部空格
void Ltrim(char *str)
{
	if (str == NULL) // 判空
		return;
	if ((*str) == ' ')
		return;

	size_t len = 0;
	len = strlen(str);
	char *p_tmp = str;

	//找第一个不为空格的
	while ((*p_tmp) != '\0')
	{
		if ((*p_tmp) == ' ')
			p_tmp++;
		else
			break;
	}

	if ((*p_tmp) == '\0') //全是空格
	{
		*str = '\0';
		return;
	}

	char *p_tmp2 = str;
	while ((*p_tmp) != '\0')
	{
		(*p_tmp2) = (*p_tmp);
		p_tmp++;
		p_tmp2++;
	}
	(*p_tmp2) = '\0';
	return;
}
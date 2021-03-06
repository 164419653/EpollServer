﻿
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "ngx_global.h"

//设置可执行程序标题相关函数：分配内存，并且把环境变量拷贝到新内存中来
void ngx_init_setproctitle()
{
    int i;
    //统计环境变量所占的内存。注意判断方法是environ[i]是否为空作为环境变量结束标记
    for (i = 0; environ[i]; i++)
    {
        g_environlen += strlen(environ[i]) + 1; //+1是因为末尾有\0,是占实际内存位置的，要算进来
    }                                           //end for

    gp_envmem = new char[g_environlen];
    memset(gp_envmem, 0, g_environlen); //内存要清空防止出现问题

    char *ptmp = gp_envmem;

    //把原来的内存内容搬到新地方来
    for (i = 0; environ[i]; i++)
    {
        size_t size = strlen(environ[i]) + 1; //不要拉下+1，因为strlen是不包括字符串末尾的\0的
        strcpy(ptmp, environ[i]);             //把原环境变量内容拷贝到新地方【新内存】
        environ[i] = ptmp;                    //然后还要让新环境变量指向这段新内存
        ptmp += size;
    }
}

//设置可执行程序标题
void ngx_setproctitle(const char *title)
{

    // 计算新标题长度
    size_t ititlelen = strlen(title);

    // 计算总的原始的argv那块内存的总长度
    size_t e_environlen = 0; //e表示局部变量吧
    for (int i = 0; g_os_argv[i]; i++)
    {
        e_environlen += strlen(g_os_argv[i]) + 1;
    }

    size_t esy = e_environlen + g_environlen; //argv和environ内存总和
    if (esy <= ititlelen)
    {
        // 进入这个循环代表标题太长了，不做任何动作
        return;
    }

    // 设置后续的命令行参数为空，表示只有argv[]中只有一个元素了，这是好习惯；防止后续argv被滥用，因为很多判断是用argv[] == NULL来做结束标记判断的;
    g_os_argv[1] = NULL;

    //(4)把标题弄进来，注意原来的命令行参数都会被覆盖掉，不要再使用这些命令行参数,而且g_os_argv[1]已经被设置为NULL了
    char *ptmp = g_os_argv[0]; //让ptmp指向g_os_argv所指向的内存
    strcpy(ptmp, title);
    ptmp += ititlelen; //跳过标题

    //(5)把剩余的原argv以及environ所占的内存全部清0，否则会出现在ps的cmd列可能还会残余一些没有被覆盖的内容；
    size_t cha = esy - ititlelen; //内存总和减去标题字符串长度(不含字符串末尾的\0)，剩余的大小，就是要memset的
    memset(ptmp, 0, cha);
    return;
}
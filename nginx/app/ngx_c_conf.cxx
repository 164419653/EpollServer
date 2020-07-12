
//系统头文件
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

#include "ngx_func.h"   //函数声明
#include "ngx_c_conf.h" //和配置文件处理相关的类,名字带c_表示和类有关

//静态成员赋值
CConfig *CConfig::m_instance = NULL;

//构造函数
CConfig::CConfig()
{
}

//析构函数
CConfig::~CConfig()
{
    std::vector<LPCConfItem>::iterator pos;
    for (pos = m_ConfigItemList.begin(); pos != m_ConfigItemList.end(); ++pos)
    {
        delete (*pos);
    } //end for
    m_ConfigItemList.clear();
}

//装载配置文件
bool CConfig::Load(const char *pconfName)
{
    FILE *fp;
    fp = fopen(pconfName, "r");
    if (fp == NULL)
        return false;

    //每一行配置文件读出来都放这里
    char linebuf[501];

    while (!feof(fp)) //检查文件是否结束 ，没有结束则条件成立
    {
        if (fgets(linebuf, 500, fp) == NULL)
            continue;

        if (linebuf[0] == 0)
            continue;

        if (*linebuf == '[')
            continue;

        //处理注释行
        if (*linebuf == ';' || *linebuf == ' ' || *linebuf == '#' || *linebuf == '\t' || *linebuf == '\n')
            continue;

        // 将本行末尾的空格回车等字符去除
        while (strlen(linebuf) > 0)
        {
            // 10(换行符) 13(回车键) 32(空格)
            if (linebuf[strlen(linebuf) - 1] == 10 || linebuf[strlen(linebuf) - 1] == 13 || linebuf[strlen(linebuf) - 1] == 32)
            {
                linebuf[strlen(linebuf) - 1] = 0;
            }
            else
            {
                break;
            }
        }

        // 对字符串末尾进行处理后二次判断
        if (linebuf[0] == 0)
            continue;

        char *ptmp = strchr(linebuf, '=');
        if (ptmp != NULL)
        {
            LPCConfItem p_confitem = new CConfItem; //注意前边类型带LP，后边new这里的类型不带
            memset(p_confitem, 0, sizeof(CConfItem));
            strncpy(p_confitem->ItemName, linebuf, (int)(ptmp - linebuf)); //等号左侧的拷贝到p_confitem->ItemName
            strcpy(p_confitem->ItemContent, ptmp + 1);                     //等号右侧的拷贝到p_confitem->ItemContent

            Rtrim(p_confitem->ItemName);
            Ltrim(p_confitem->ItemName);
            Rtrim(p_confitem->ItemContent);
            Ltrim(p_confitem->ItemContent);

            m_ConfigItemList.push_back(p_confitem);
        }
    }

    fclose(fp); // 关闭打开的文件描述符
    return true;
}

const char *CConfig::GetString(const char *p_itemname)
{
    std::vector<LPCConfItem>::iterator pos;
    for (pos = m_ConfigItemList.begin(); pos != m_ConfigItemList.end(); ++pos)
    {
        if (strcasecmp((*pos)->ItemName, p_itemname) == 0)
            return (*pos)->ItemContent;
    }
    return NULL;
}

int CConfig::GetIntDefault(const char *p_itemname, const int def)
{
    std::vector<LPCConfItem>::iterator pos;
    for (pos = m_ConfigItemList.begin(); pos != m_ConfigItemList.end(); ++pos)
    {
        if (strcasecmp((*pos)->ItemName, p_itemname) == 0)
            return atoi((*pos)->ItemContent);
    }
    return def;
}

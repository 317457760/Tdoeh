/*************************************************************************************
 * 文件名: tdoeh_config.h
 * 作  者: 思凡
 * 日  期: 2013年06月06日
 * 描  述: 解析配置文件
 * 修  改: 
 *************************************************************************************/
#ifndef __TDOEH_CONFIG_H__
#define __TDOEH_CONFIG_H__

#include <map>
#include <vector>
#include <string>
#include "tdoeh_base.h"

namespace Tdoeh{

class CConfig : public CBase{
public:
    //配置信息最长大小
    enum{
        MAX_CONFIG_SIZE = (1024 * 1024)
    };

    //行内容标识
    enum{
        COMMENTS = 0,
        TAGS = 1,
        CONTENT = 2
    };

public:
    enum{
        E_NO_TAG = E_MAX_ERROR_NUMBER,  //没有tag就发现内容
        E_TOO_LENGTH = E_MAX_ERROR_NUMBER - 1,  //内容太长
        E_INVALID_TAG = E_MAX_ERROR_NUMBER - 2  //非法Tag
    };

    std::map<std::string, std::vector<std::map<std::string, std::string> > > m_mapData;

private:
    //去除字符串左右空白,会直接在字符串上修改
    //返回修改后的长度
    int trim(char **pLine);
    bool isBlank(char ch);
    char *getLine(char **pContent);
    int dealLine(char **pLine);
    int dealData(char *pLine, std::string &strName, std::string &strValue);

public:
    CConfig();
    ~CConfig();

public:
    int parse(std::string strContent);
    std::vector<std::map<std::string, std::string> > get(std::string strPart);
    void display();
};

};

#endif

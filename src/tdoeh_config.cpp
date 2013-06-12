/*************************************************************************************
 * 文件名: tdoeh_config.cpp
 * 作  者: 思凡
 * 日  期: 2013年06月06日
 * 描  述: 
 * 修  改: 
 *************************************************************************************/
#include <stdio.h>
#include <string.h>
#include "tdoeh_config.h"
using namespace std;

namespace Tdoeh{

CConfig::CConfig()
{
}

CConfig::~CConfig()
{
}

int CConfig::parse(string strContent)
{
    char szContent[MAX_CONFIG_SIZE];
    if(strContent.length() > MAX_CONFIG_SIZE){
        TDOEH_SET_ERROR_RETURN(E_TOO_LENGTH, "config file max length is %d,now is %d",
                MAX_CONFIG_SIZE, strContent.length());
    }
    char *pContent = szContent;
    memcpy(pContent, strContent.c_str(), strContent.length());
    pContent[strContent.length()] = '\0';
    
    map<string, string> mapTemp;
    string strTag = "";
    string strName = "";
    string strValue = "";
    int iLineNumber = 0;

    char *pLine;
    while(pLine = getLine(&pContent)){
        iLineNumber++;

        int iType = dealLine(&pLine);

        if(iType == TAGS){   //新TAG
            int iLength = trim(&pLine);
            if(iLength <= 2){
                TDOEH_SET_ERROR_RETURN(E_INVALID_TAG, "invalid tag found, line %d", iLineNumber);
            }
            if(pLine[iLength - 1] != ']'){
                TDOEH_SET_ERROR_RETURN(E_INVALID_TAG, "invalid tag found, line %d", iLineNumber);
            }
            pLine[iLength - 1] = '\0';
            pLine++;
            iLength = trim(&pLine);
            if(iLength <= 0){
                TDOEH_SET_ERROR_RETURN(E_INVALID_TAG, "invalid tag found, line %d", iLineNumber);
            }

            //把上次的结果插入到m_mapData中
            if(strTag.length() > 0 && mapTemp.size() > 0){
                if(m_mapData.find(strTag) == m_mapData.end()){
                    vector<map<string, string> > vectorTemp;
                    m_mapData[strTag] = vectorTemp;
                }
                m_mapData[strTag].push_back(mapTemp);
            }

            strTag = pLine;
            mapTemp.clear();
            continue;
        }

        if(iType == CONTENT){   //内容
            if(strTag.length() <= 0){
                TDOEH_SET_ERROR_RETURN(E_NO_TAG, "no tag found, line %d", iLineNumber);
            }
            TDOEH_RETURN_IF_ERROR(dealData(pLine, strName, strValue));
            mapTemp[strName] = strValue;
            continue;
        }
    }

    //把最后的结果插入到m_mapData中
    if(strTag.length() > 0 && mapTemp.size() > 0){
        if(m_mapData.find(strTag) == m_mapData.end()){
            vector<map<string, string> > vectorTemp;
            m_mapData[strTag] = vectorTemp;
        }
        m_mapData[strTag].push_back(mapTemp);
    }

    return E_SUCCESS;
}

char *CConfig::getLine(char **pContent)
{
    if(*pContent[0] == '\0'){
        return NULL;
    }

    int iLength = strlen(*pContent);
    char *pLine = *pContent;
    int i;
    for(i = 0; i < iLength; i++){
        if(pLine[i] == '\n'){
            pLine[i] = '\0';
            break;
        }
    }

    if(i < iLength){
        (*pContent) = &pLine[i + 1];
    }else{
        (*pContent) = &pLine[i];
    }

    return pLine;
}

bool CConfig::isBlank(char ch)
{
    switch(ch){
        case ' ':
        case '\t':
        case '\r':
        case '\n':
            return true;
    }
    return false;
}

int CConfig::trim(char **pLine)
{
    int iLength = strlen(*pLine);
    
    //去除前面空白
    for(; iLength > 0; iLength--){
        if(!isBlank(**pLine)){
            break;
        }
        (*pLine)++;
    }


    if(iLength <= 0){
        return iLength;
    }

    //去除后面空白
    for(; iLength >= 0; iLength--){
        if(!isBlank((*pLine)[iLength-1])){
            break;
        }
        (*pLine)[iLength-1] = '\0';
    }

    return iLength;
}

int CConfig::dealLine(char **pLine)
{
    int iType = COMMENTS;

    int iLength = trim(pLine);

    if(iLength <= 0){
        return COMMENTS;
    }

    if((*pLine)[0] == '['){
        return TAGS;
    }

    if((*pLine)[0] == '#'){
        return COMMENTS;
    }

    return CONTENT;
}


int CConfig::dealData(char *pLine, string &strName, string &strValue)
{
    int iLength = strlen(pLine);
    int iPos = 0;
    for(iPos = 0; iPos < iLength;iPos++){
        if(pLine[iPos] == '='){
            break;
        }
    }

    if(iPos >= iLength){
        strName = pLine;
        strValue = "";
        return E_SUCCESS;
    }

    pLine[iPos] = '\0';
    char *name = pLine;
    char *value = pLine + iPos + 1;
    for(int i = 0; i < strlen(value); i++){
        if(value[i] == '#'){
            value[i] = '\0';
            break;
        }
    }
    trim(&name);
    trim(&value);
    strName = name;
    strValue = value;
    return E_SUCCESS;
}

vector<map<string, string> > CConfig::get(string strPart)
{
    if(m_mapData.find(strPart) != m_mapData.end()){
        return m_mapData[strPart];
    }

    vector<map<string, string> > temp;
    return temp;
}

void CConfig::display()
{
    map<string, vector<map<string, string> > >::iterator it = m_mapData.begin();
    for(; it != m_mapData.end(); ++it){
        fprintf(stderr, "tag:%s\n", (it->first).c_str());
        for(int i = 0; i < (it->second).size(); i++){
            fprintf(stderr, "\t%d:\n", i);
            map<string, string>::iterator it2 = (it->second)[i].begin();
            for(; it2 != (it->second)[i].end(); ++it2){
                fprintf(stderr, "%s:%s\n", it2->first.c_str(), it2->second.c_str());
            }
        }
    }
}

};

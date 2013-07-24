/*************************************************************************************
 * 文件名: tdoeh_singleton.h
 * 作  者: 思凡
 * 日  期: 2013年06月08日
 * 描  述: 单例类基类
 * 修  改: 
 * 说  明: 单例类不继承自CBase,因为CBase中会设置最后错误信息,
 *         是非线程安全的,单例类又普遍用于多线程中,在使用中可能会造成得到的错误信息不对
 *************************************************************************************/
#ifndef __TDOEH_SINGLETON_H__
#define __TDOEH_SINGLETON_H__

#include <stdio.h>
#include "tdoeh_noncopyable.h"

namespace Tdoeh{

template<class T>
class CSingleton : public CNoncopyable{
public:
    static T m_Instance;

protected:
    CSingleton(){ 
    };
    virtual ~CSingleton(){
    };

public:
    static T& Instance()
    {
        return m_Instance;
    }
};

template<typename T>
T CSingleton<T>::m_Instance;

};

#endif

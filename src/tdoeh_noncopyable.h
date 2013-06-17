/*************************************************************************************
 * 文件名: tdoeh_noncopyable.h
 * 作  者: 思凡
 * 日  期: 2013年06月08日
 * 描  述: 不允许有赋值操作的基类
 * 修  改: 
 *************************************************************************************/
#ifndef __TDOEH_NONCOPYABLE_H__
#define __TDOEH_NONCOPYABLE_H__

namespace Tdoeh{

class CNoncopyable{
private:
    CNoncopyable(const CNoncopyable &);
    CNoncopyable &operator=(const CNoncopyable &);

protected:
    CNoncopyable(){};
    ~CNoncopyable(){};
};

};

#endif

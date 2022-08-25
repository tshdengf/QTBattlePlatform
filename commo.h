#ifndef COMMON_H
#define COMMON_H

//5XXX 用户操作
#define OK                  5000 //注册
#define REGISTER            5001 //注册
#define LOGIN               5002 //登录
#define SINGLE_GETQUESTION  5003 //个人训练获取题目
#define RANK                5004 //进行排位
#define ANSWER              5005 //答题
#define RANKRESULT          5006 //结束发送结果
#define GUANLI              5007 //管理员登录
#define GETTABLE            5008 //获取数据库信息
#define SIMAGE              5009 //发送图片数据
#define SENDHEAD            5010 //发送图片
#define EIXT                5011 //退出
#define IMG                 5012 //发送图片数据
#define GETFRIEND           5013 //查找在线好友
#define ADDFIREND           5014 //添加好友
#define AGREE               4001 //同意添加
#define DULE                4002 //邀请对战
#define YDULE               4003 //设置对决
#define CIMAG               4005 //向客户端传文件
//8XXX 错误
#define ERROR         8001 //未知错误
#define USEREXST      8002 //用户已经存在
#define NAMEORPASSWD  8003 //用户名或者密码错误
#define USEREXSTLOGIN 8004 //用户已经登陆

#define QUESTION 5
#define FILE_BLOCK_MAX (100)
#define LEN_SIZE (2000)
#endif

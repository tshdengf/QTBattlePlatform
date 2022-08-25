#include "db.h"

DB::DB(const char*host, const char*userName, const char*passwd, const char*dbName)
{
    //初始化数据库句柄
    _mysql = mysql_init(NULL);
    if(_mysql == NULL)
    {
        spdlog::get("BrainStom")->error("初始化数据库失败");
        exit(-1);
    }


    MYSQL *con = mysql_real_connect(_mysql,host,userName,passwd,dbName,0,NULL,0);
    if(con == NULL)
    {
        spdlog::get("BrainStom")->error("连接数据库失败 :{}\n",mysql_error(_mysql));
        exit(-1);
    }

    _mysql = con;
    int ret = mysql_query(_mysql,"set names utf8");
    if(ret != 0)
    {
        printf("mysql_query utf8 error!  %s\n",mysql_error(_mysql));
        exit(-1);
    }
}

bool DB::db_exec(const char *sql)
{
//    std::unique_lock<std::mutex> lock(mutex);   //数据库句柄上锁
    int ret = mysql_query(_mysql,sql);
    if(ret != 0)
    {
        printf("mysql_query error!  %s\n",mysql_error(_mysql));
        return false;
    }
    return true;
}
//进行查询，将数据保存
bool DB::db_select(const char *sql,Json::Value &outJson)
{
//    std::unique_lock<std::mutex> lock(mutex);
    //函数执行一条 MySQL 查询。
    int ret = mysql_query(_mysql,sql);
    if(ret != 0)
    {
        printf("mysql_query error!  %s\n",mysql_error(_mysql));
        return false;
    }
    //从mysql服务器下载查询结果,返回结果事件集
    MYSQL_RES *sql_res = mysql_store_result(_mysql);
    if(sql_res == NULL)//发送错误
    {
        if(mysql_errno(_mysql)==0)//未知错误
        {
            return true;
        }
        else
        {
            spdlog::get("BrainStorm")->error("mysql : %s",mysql_error(_mysql));//打印错误
            return true;
        }
    }
    MYSQL_ROW row;//从结果集中一行行获取数据
    unsigned int num_fields =  mysql_num_fields(sql_res);//获取列数
    MYSQL_FIELD*fetch_field = mysql_fetch_field(sql_res);//获取表头
    int rows = 0;
    //一行一行获取数据
    while(row = mysql_fetch_row(sql_res))//检索下一整行
    {
        for(unsigned int i=0; i<num_fields; i++)//按列赋值
        {
            outJson[fetch_field[i].name].append(row[i]);//按表头赋值
        }
        rows++;
    }
//    outJson["row"] = rows;
    mysql_free_result(sql_res);


    return true;
}

//
// Created by 404NotFound on 2022/10/3.
//

#ifndef PROJECT_DATABASE_H
#define PROJECT_DATABASE_H

#include <mysql/mysql.h>
#include <stdio.h>
#include <string.h>

int database_init(MYSQL *mysql, const char *host,
                  const char *user, const char *passwd,
                  const char *db, unsigned int port,
                  const char *unix_socket,
                  unsigned long clientflag);//初始化数据库
/*
 * 连接数据库,并检查数据库中是否已经存在存储数据的数据库
 * */
int database_info(MYSQL *mysql);//获取数据库连接的有关信息。
int get_query_info(MYSQL *mysql);//获取查询结果

#endif //PROJECT_DATABASE_H

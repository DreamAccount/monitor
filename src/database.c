//
// Created by 404NotFound on 2022/10/3.
//
#include "database.h"

int database_init(MYSQL *mysql, const char *host,
                  const char *user, const char *passwd,
                  const char *db, unsigned int port,
                  const char *unix_socket,
                  unsigned long clientflag) {
    MYSQL_RES *dblist;
    MYSQL_RES *talist;
    unsigned int column;
    unsigned int row;
    MYSQL_ROW ROW;
    unsigned int error_num;
    //连接数据库
    char *sql_createdb_str = "create database IOT";
    char *sql_createta_str = "create table data(time timestamp,temperature double, humidity double,light double,soil_humidity double)";
    const char *errmsg;
    mysql_init(mysql);
    if (mysql_real_connect(mysql, host, user,
                           passwd, db, port, NULL, 0) == NULL) {
        error_num = mysql_errno(mysql);

        if (error_num == 1049) {
            if (mysql_real_connect(mysql, host, user,
                                   passwd, NULL, port, NULL, 0) == NULL) {
                printf("error message:%s", mysql_error(mysql));
                exit(0);
            }
            if (mysql_query(mysql, sql_createdb_str)) {
                printf("Error %u: %s\n", mysql_errno(mysql), mysql_error(mysql));
                exit(0);
            }
            if (mysql_select_db(mysql, "IOT")) {
                printf("Error %u: %s\n", mysql_errno(mysql), mysql_error(mysql));
                exit(0);
            }
            if (mysql_query(mysql, sql_createta_str)) {
                printf("Error %u: %s\n", mysql_errno(mysql), mysql_error(mysql));
                exit(0);
            }
        }
        else
        {
            errmsg = mysql_error(mysql);
            printf("error number:%d,error message:%s\n",error_num,errmsg);
        }
    } else {
        talist = mysql_list_tables(mysql, NULL);
        while (1) {
            ROW = mysql_fetch_row(talist);
            if (ROW == NULL) {
                break;
            } else {
                if (strcmp("data", ROW[0]) == 0) {
                    return 0;
                }
            }
        }
        if (mysql_query(mysql, sql_createta_str)) {
            printf("Error %u: %s\n", mysql_errno(mysql), mysql_error(mysql));
            exit(0);
        }
    }
    return 0;
}

int get_query_info(MYSQL *mysql) {
    MYSQL_ROW ROW;
    MYSQL_FIELD *FIELD;
    MYSQL_RES *result = mysql_store_result(mysql);
    unsigned int column = mysql_num_fields(result);
    for(int i = 0;i<column;i++)
    {
        FIELD = mysql_fetch_field(result);
        printf("%s",FIELD->name);
        putchar('\t');
    }
    putchar('\n');
    while (1) {
        ROW = mysql_fetch_row(result);
        if (ROW) {
            for (int i = 0; i < column; i++) {
                printf("%s\t\t", ROW[i]);
            }
            putchar('\n');
        } else {
            break;
        }
    }
    mysql_free_result(result);
    return 0;
}

int database_info(MYSQL *mysql) {
    MY_CHARSET_INFO cs;
    const char *server_host_info = mysql_get_host_info(mysql);
    const char *server_version = mysql_get_server_info(mysql);
    char local_version[100];
    unsigned long version = mysql_get_client_version();
    unsigned int major_version, release_level, sub_version;
    major_version = version / 10000;
    version = version % 10000;
    release_level = version / 100;
    version = version % 100;
    sub_version = version;
    sprintf(local_version, "%u.%u.%u\n", major_version, release_level, sub_version);
    printf("\n\n");
    printf("server info:\n----------------------------\n");
    printf("server_host_info:%s\nserver_version:%s\nlocal_client_version:%s", server_host_info, server_version,
           local_version);
    mysql_get_character_set_info(mysql, &cs);
    printf("character set name: %s\n", cs.csname);
    printf("comment: %s\n", cs.comment);
    return 0;
}


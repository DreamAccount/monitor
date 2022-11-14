//
// Created by 404NotFound on 2022/10/13.
//

#ifndef PROJECT1_CONFIG_H
#define PROJECT1_CONFIG_H

#define DEFAULT_CONFIG "config.json"
#define DEFAULT_DATABASE_IP "127.0.0.1"
#define DEFAULT_DATABASE_PORT 3306
#define DEFAULT_DATABASE_NAME "IOT"
#define DEFAULT_DATABASE_USERNAME "root"
#define DEFAULT_DATABASE_PASSWORD "root"
#define DEFAULT_MQTT_IP "127.0.0.1"
#define DEFAULT_MQTT_PORT 1883
#define DEFAULT_MQTT_USERNAME "admin"
#define DEFAULT_MQTT_PASSWORD "admin"
#define DEFAULT_MQTT_CLIENTID "mqtt"
#define DEFAULT_MQTT_TOPIC "mqtt"
#define DEFAULT_MQTT_QOS 1
typedef struct config_t
{
    char database_server_ip[16];                      //服务器ip地址
    int  database_server_port;                        //服务器端口号
    char mqtt_server_ip[16];                          //mqtt服务器ip地址
    int  mqtt_server_port;                            //mqtt服务器端口
    char database[255];                               //数据库名称
    char database_username[255];                      //数据库用户名
    char database_password[255];                      //数据库密码
    char mqtt_username[255];                          //mqtt用户名
    char mqtt_password[255];                          //mqtt密码
    char mqtt_clientid[255];                          //mqtt客户端id
    char mqtt_topic[255];                             //订阅标题
    int mqtt_qos;                                     //优先级
}config_t;

enum {
    GETOPT_VAL_DATABASE_IP = 'a',
    GETOPT_VAL_DATABASE_PORT,
    GETOPT_VAL_DATABASE_NAME,
    GETOPT_VAL_DATABASE_USERNAME,
    GETOPT_VAL_DATABASE_PASSWORD,
    GETOPT_VAL_MQTT_IP,
    GETOPT_VAL_MQTT_PORT,
    GETOPT_VAL_MQTT_USERNAME,
    GETOPT_VAL_MQTT_PASSWORD,
    GETOPT_VAL_MQTT_CLIENTID,
    GETOPT_VAL_MQTT_TOPIC,
    GETOPT_VAL_MQTT_QOS,
    GETOPT_VAL_CONFIG,
};
int parse_config(char *config_path,config_t *config);   //解析配置文件
void print_config_info(config_t *config);                        //打印配置文件信息
#endif //PROJECT1_CONFIG_H

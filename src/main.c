//
// Created by 404NotFound on 2022/10/3.
//
#include "database.h"
#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include "network.h"
#include "config.h"
#include "MQTTClient.h"
#include <ev.h>

int main(int argc, char **argv) {
    int rc, ch;
    MYSQL con;
    char ADDRESS[255];
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient client;
    volatile MQTTClient_deliveryToken deliveryToken;

    char *database_server_ip;
    int database_server_port;
    char *database;
    char *database_username;
    char *database_password;
    char *mqtt_server_ip;
    int mqtt_server_port;
    char *mqtt_username;
    char *mqtt_password;
    char *mqtt_clientid;
    char *mqtt_topic;
    int mqtt_qos;

    //解析配置文件
    config_t *config_json = (config_t *) malloc(sizeof(config_t));
    parse_config(NULL, config_json);
    database_server_ip = config_json->database_server_ip;
    database_server_port = config_json->database_server_port;
    database = config_json->database;
    database_username = config_json->database_username;
    database_password = config_json->database_password;

    mqtt_server_ip = config_json->mqtt_server_ip;
    mqtt_server_port = config_json->mqtt_server_port;
    mqtt_username = config_json->mqtt_username;
    mqtt_password = config_json->mqtt_password;
    mqtt_clientid = config_json->mqtt_clientid;
    mqtt_topic = config_json->mqtt_topic;
    mqtt_qos = config_json->mqtt_qos;

    bzero(ADDRESS, 255);
    sprintf(ADDRESS, "tcp://%s:%d", mqtt_server_ip, mqtt_server_port);
    //初始化数据库
    database_init(&con, database_server_ip, database_username, database_password, database, database_server_port, NULL,
                  0);
    //创建mqtt连接对象
    MQTTClient_create(&client, ADDRESS, mqtt_clientid,
                      MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.cleansession = 1;
    conn_opts.connectTimeout = 300;
    conn_opts.username = mqtt_username;
    conn_opts.password = mqtt_password;
    //设置mqtt订阅的topic来信息的回调函数
    MQTTClient_setCallbacks(client, (void*)&con, connlost, msgarrvd, delivered);
    //连接mqtt服务器
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    } else {
        printf("connect mqtt server:%s:%d\n", mqtt_server_ip, mqtt_server_port);
    }
    //订阅消息
    MQTTClient_subscribe(client, mqtt_topic, mqtt_qos);
    do {
        ch = getchar();
    } while (ch != 'Q' && ch != 'q');

    free(config_json);
    config_json = NULL;
    mysql_close(&con);

    return 0;
}
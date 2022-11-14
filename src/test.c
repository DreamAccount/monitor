//
// Created by 404NotFound on 2022/11/9.
//
#include "database.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <mysql/mysql.h>
#include "config.h"
#include <ev.h>
#include <MQTTClient.h>
#include "network.h"
#include "utils.h"

int main(int argc, char **argv) {
    MYSQL con;
    char *database_server_ip = NULL;
    int database_server_port = 0;
    char *database_name = NULL;
    char *database_username = NULL;
    char *database_password = NULL;
    char *mqtt_server_ip = NULL;
    int mqtt_server_port = 0;
    char *mqtt_username = NULL;
    char *mqtt_password = NULL;
    char *mqtt_clientid = NULL;
    char *mqtt_topic = NULL;
    char *config_path = NULL;
    unsigned int mqtt_QOS = 1;
    int rc;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient client;
    volatile MQTTClient_deliveryToken deliveryToken;
//
    char c;
    static struct option long_options[] = {
            {"database_ip",       required_argument, NULL, GETOPT_VAL_DATABASE_IP},
            {"database_port",     required_argument, NULL, GETOPT_VAL_DATABASE_PORT},
            {"database_name",     required_argument, NULL, GETOPT_VAL_DATABASE_NAME},
            {"database_username", required_argument, NULL, GETOPT_VAL_DATABASE_USERNAME},
            {"database_password", required_argument, NULL, GETOPT_VAL_DATABASE_PASSWORD},
            {"mqtt_server_ip",    required_argument, NULL, GETOPT_VAL_MQTT_IP},
            {"mqtt_server_port",  required_argument, NULL, GETOPT_VAL_MQTT_PORT},
            {"mqtt_username",     required_argument, NULL, GETOPT_VAL_MQTT_USERNAME},
            {"mqtt_password",     required_argument, NULL, GETOPT_VAL_MQTT_PASSWORD},
            {"mqtt_topic",        required_argument, NULL, GETOPT_VAL_MQTT_TOPIC},
            {"mqtt_clientid",     required_argument, NULL, GETOPT_VAL_MQTT_CLIENTID},
            {"mqtt_qos",          required_argument, NULL, GETOPT_VAL_MQTT_TOPIC}
    };
    while ((c = getopt_long(argc, argv, "a:b:c:d:e:f:g:h:i:j:k:",
                            long_options, NULL)) != -1) {
        switch (c) {
            case GETOPT_VAL_DATABASE_IP:
                database_server_ip = optarg;
                break;
            case GETOPT_VAL_DATABASE_PORT:
                database_server_port = atoi(optarg);
                break;
            case GETOPT_VAL_DATABASE_NAME:
                database_name = optarg;
                break;
            case GETOPT_VAL_DATABASE_USERNAME:
                database_username = optarg;
                break;
            case GETOPT_VAL_DATABASE_PASSWORD:
                database_password = optarg;
                break;
            case GETOPT_VAL_MQTT_IP:
                mqtt_server_ip = optarg;
                break;
            case GETOPT_VAL_MQTT_PORT:
                mqtt_server_port = atoi(optarg);
                break;
            case GETOPT_VAL_MQTT_USERNAME:
                mqtt_username = optarg;
                break;
            case GETOPT_VAL_MQTT_PASSWORD:
                mqtt_password = optarg;
                break;
            case GETOPT_VAL_MQTT_CLIENTID:
                mqtt_clientid = optarg;
                break;
            case GETOPT_VAL_MQTT_TOPIC:
                mqtt_topic = optarg;
                break;
            case GETOPT_VAL_MQTT_QOS:
                mqtt_QOS = atoi(optarg);
                break;
            case GETOPT_VAL_CONFIG:
                config_path = optarg;
                break;
        }
    }
//    //解析配置文件
    config_t *config_json = (config_t *) malloc(sizeof(config_t));
    memset(config_json, 0, sizeof(config_t));
    parse_config(config_path, config_json);
    if (database_server_ip == NULL) {
        database_server_ip = config_json->database_server_ip;
    }
    if (database_server_port == 0) {
        database_server_port = config_json->database_server_port;
    }
    if (database_name == NULL) {
        database_name = config_json->database;
    }
    if (database_username == NULL) {
        database_username = config_json->database_username;
    }
    if (database_password == NULL) {
        database_password = config_json->database_password;
    }
    if (mqtt_server_ip == NULL) {
        mqtt_server_ip = config_json->mqtt_server_ip;
    }
    if (mqtt_server_port == 0) {
        mqtt_server_port = config_json->mqtt_server_port;
    }
    if (mqtt_username == NULL) {
        mqtt_username = config_json->mqtt_username;
    }
    if (mqtt_password == NULL) {
        mqtt_password = config_json->mqtt_password;
    }
    if (mqtt_clientid == NULL) {
        mqtt_clientid = config_json->mqtt_clientid;
    }
    if (mqtt_topic == NULL) {
        mqtt_topic = config_json->mqtt_topic;
    }
    if (mqtt_QOS == 0) {
        mqtt_QOS = config_json->mqtt_qos;
    }

    char ADDRESS[255];
    bzero(ADDRESS, 255);
    sprintf(ADDRESS, "tcp://%s:%d", mqtt_server_ip, mqtt_server_port);

    //创建mqtt连接对象
    MQTTClient_create(&client, ADDRESS, mqtt_clientid,
                      MQTTCLIENT_PERSISTENCE_NONE, NULL);
    //设置连接属性
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    conn_opts.username = mqtt_username;
    conn_opts.password = mqtt_password;

    //设置回调函数
    MQTTClient_setCallbacks(client, (void *) &con, connlost, msgarrvd, delivered);

    //连接mqtt服务器
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    } else {
        printf("connect mqtt server tcp:%s:%d successful!\n", mqtt_server_ip, mqtt_server_port);
    }

    //初始化数据库并连接数据库服务器
    database_init(&con, database_server_ip, database_username, database_password, database_name, database_server_port,
                  NULL,
                  0);

    printf("subscribe message topic:%s,qos:%u.........\n", mqtt_topic, mqtt_QOS);
    //订阅消息
    MQTTClient_subscribe(client, mqtt_topic, mqtt_QOS);
    //定义一个阻塞函数防止主进程结束
    sleep(100);

    free(config_json);
    config_json = NULL;
    mysql_close(&con);

//    return 0;
}
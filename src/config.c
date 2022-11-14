//
// Created by 404NotFound on 2022/11/9.
//
#include "config.h"
#include "cjson/cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int parse_config(char *config_path, config_t *config) {
    cJSON *database = NULL;
    cJSON *database_ip = NULL;
    cJSON *database_port = NULL;
    cJSON *database_name = NULL;
    cJSON *database_username = NULL;
    cJSON *database_password = NULL;
    cJSON *mqtt_server_ip = NULL;
    cJSON *mqtt_server_port = NULL;
    cJSON *mqtt_username = NULL;
    cJSON *mqtt_password = NULL;
    cJSON *mqtt_clientid = NULL;
    cJSON *mqtt_topic = NULL;
    cJSON *mqtt_qos = NULL;
    if (config == NULL) {
        printf("invalid config struct\n");
        return 0;
    }
    if (config_path == NULL) {
        config_path = DEFAULT_CONFIG;
    }
    unsigned long file_len = 0;
    //获取配置文件的长度
    FILE *fp = fopen(config_path, "r");
    if (fp == NULL) {
        return 0;
    } else {
        fseek(fp, 0, SEEK_END);
        file_len = ftell(fp);
        fseek(fp, 0, SEEK_SET);
    }
    //读取配置文件
    char *config_str = (char *) malloc(file_len);
    fread(config_str, file_len, 1, fp);
    fclose(fp);
    cJSON *root = cJSON_Parse(config_str);
    if (root == NULL) {
        return 0;
    }
    database = cJSON_GetObjectItem(root, "database");
    if (database != NULL) {
        database_ip = cJSON_GetObjectItem(database, "server_ip");
        if (database_ip != NULL) {
            strcpy(config->database_server_ip, database_ip->valuestring);
            config->database_server_ip[strlen(database_ip->valuestring)] = '\0';
        } else {
            strcpy(config->database_server_ip, DEFAULT_DATABASE_IP);
            config->database_server_ip[strlen(DEFAULT_DATABASE_IP)] = '\0';
        }
        database_port = cJSON_GetObjectItem(database, "server_port");
        if (database_port != NULL) {
            config->database_server_port = database_port->valueint;
        } else {
            config->database_server_port = DEFAULT_DATABASE_PORT;
        }
        database_name = cJSON_GetObjectItem(database, "database");
        if (database_name != NULL) {
            strcpy(config->database, database_name->valuestring);
            config->database[strlen(database_name->valuestring)] = '\0';
        } else {
            strcpy(config->database, DEFAULT_DATABASE_NAME);
            config->database[strlen(DEFAULT_DATABASE_NAME)] = '\0';
        }
        database_username = cJSON_GetObjectItem(database, "username");
        if (database_username != NULL) {
            strcpy(config->database_username, database_username->valuestring);
            config->database_username[strlen(database_username->valuestring)] = '\0';
        } else {
            strcpy(config->database_username, DEFAULT_DATABASE_USERNAME);
            config->database_username[strlen(DEFAULT_DATABASE_USERNAME)] = '\0';
        }
        database_password = cJSON_GetObjectItem(database, "password");
        if (database_password != NULL) {
            strcpy(config->database_password, database_password->valuestring);
            config->database_password[strlen(database_password->valuestring)] = '\0';
        } else {
            strcpy(config->database_password, DEFAULT_DATABASE_PASSWORD);
            config->database_password[strlen(DEFAULT_DATABASE_PASSWORD)] = '\0';
        }
    }


    cJSON *mqtt = cJSON_GetObjectItem(root, "mqtt");
    if (mqtt != NULL) {
        mqtt_server_ip = cJSON_GetObjectItem(mqtt, "server_ip");
        if (mqtt_server_ip != NULL) {
            strcpy(config->mqtt_server_ip, mqtt_server_ip->valuestring);
            config->mqtt_server_ip[strlen(mqtt_server_ip->valuestring)] = '\0';
        } else {
            strcpy(config->mqtt_server_ip, DEFAULT_MQTT_IP);
            config->mqtt_server_ip[strlen(DEFAULT_MQTT_IP)] = '\0';
        }
        mqtt_server_port = cJSON_GetObjectItem(mqtt, "server_port");
        if (mqtt_server_port != NULL) {
            config->mqtt_server_port = mqtt_server_port->valueint;
        } else {
            config->mqtt_server_port = DEFAULT_MQTT_PORT;
        }
        mqtt_username = cJSON_GetObjectItem(mqtt, "username");
        if (mqtt_username != NULL) {
            strcpy(config->mqtt_username, mqtt_username->valuestring);
            config->mqtt_username[strlen(mqtt_username->valuestring)] = '\0';
        } else {
            strcpy(config->mqtt_username, DEFAULT_MQTT_USERNAME);
            config->mqtt_username[strlen(DEFAULT_MQTT_USERNAME)] = '\0';
        }
        mqtt_password = cJSON_GetObjectItem(mqtt, "password");
        if (mqtt_password != NULL) {
            strcpy(config->mqtt_password, mqtt_password->valuestring);
            config->mqtt_password[strlen(mqtt_password->valuestring)] = '\0';
        } else {
            strcpy(config->mqtt_password, DEFAULT_MQTT_PASSWORD);
            config->mqtt_password[strlen(DEFAULT_MQTT_PASSWORD)] = '\0';
        }
        mqtt_clientid = cJSON_GetObjectItem(mqtt, "clientid");
        if (mqtt_clientid != NULL) {
            strcpy(config->mqtt_clientid, mqtt_clientid->valuestring);
            config->mqtt_clientid[strlen(mqtt_clientid->valuestring)] = '\0';
        } else {
            strcpy(config->mqtt_clientid, DEFAULT_MQTT_CLIENTID);
            config->mqtt_clientid[strlen(DEFAULT_MQTT_CLIENTID)] = '\0';
        }
        mqtt_topic = cJSON_GetObjectItem(mqtt, "topic");
        if (mqtt_topic != NULL) {
            strcpy(config->mqtt_topic, mqtt_topic->valuestring);
            config->mqtt_topic[strlen(mqtt_topic->valuestring)] = '\0';
        } else {
            strcpy(config->mqtt_topic, DEFAULT_MQTT_TOPIC);
            config->mqtt_topic[strlen(DEFAULT_MQTT_TOPIC)] = '\0';
        }
        mqtt_qos = cJSON_GetObjectItem(mqtt, "QOS");
        if (mqtt_qos != NULL) {
            config->mqtt_qos = mqtt_qos->valueint;
        } else {
            config->mqtt_qos = DEFAULT_MQTT_QOS;
        }
    }
    free(config_str);
    config_str = NULL;
    cJSON_Delete(root);
    return 0;
}

void print_config_info(config_t *config) {
    printf("current config info is:\n\n");
    printf("database_server_ip:\t%s\n", config->database_server_ip);
    printf("database_server_port:\t%d\n", config->database_server_port);
    printf("database_server_database:\t%s\n", config->database);
    printf("database_server_username:\t%s\n", config->database_username);
    printf("database_server_password:\t%s\n", config->database_password);
    printf("mqtt_server_ip:\t%s\n", config->mqtt_server_ip);
    printf("mqtt_server_port:\t%d\n", config->mqtt_server_port);
    printf("mqtt_server username:\t%s\n", config->mqtt_username);
    printf("mqtt_server password:\t%s\n", config->mqtt_password);
    printf("mqtt_server topic:\t%s\n", config->mqtt_topic);
}
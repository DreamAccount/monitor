//
// Created by 404NotFound on 2022/11/13.
//
#include "network.h"
#include "config.h"
#include "cjson/cJSON.h"
#include <mysql/mysql.h>

void delivered(void *context, MQTTClient_deliveryToken dt) {
    MYSQL *mysql = (MYSQL *) context;
    mysql_close(mysql);
    printf("Message with token value %d delivery confirmed\n", dt);
}

void parse_data(char *payload, int payloadlen, data_t *data) {
    //解析json字符串
    cJSON *root = cJSON_Parse(payload);
    cJSON *timestamp = cJSON_GetObjectItem(root, "time");
    cJSON *TemObject = cJSON_GetObjectItem(root, "temperature");
    cJSON *HumObject = cJSON_GetObjectItem(root, "humidity");
    cJSON *LightObject = cJSON_GetObjectItem(root,"light");
    cJSON *Soilhumobject = cJSON_GetObjectItem(root, "soil_humidity");
    data->time = timestamp->valuedouble;
    data->tem = TemObject->valuedouble;
    data->hum = HumObject->valuedouble;
    data->sohum = Soilhumobject->valuedouble;
    data->light = LightObject->valuedouble;
}

int handler(MYSQL *mysql, char *payload, int payloadlen) {
    //定义获取的数据
    data_t data;
    //定义sql语句缓冲区
    char sql[256];
    //解析数据
    parse_data(payload, payloadlen, &data);
    //更新数据库
    printf("time:%lu,tem:%lf,humidity:%lf,light:%lf,soil_humidity:%lf\n", data.time, data.tem, data.hum,data.light,data.sohum);
    //构造插入数据sql语句
    sprintf(sql, "insert into data values((select FROM_UNIXTIME(%lu)),%lf,%lf,%lf,%lf)", data.time, data.tem, data.hum,data.light,data.sohum);
    printf("%s\n", sql);
    if (mysql_query(mysql, sql) != 0) {
        printf("insert data to mysql database error!\n");
        printf("excute sql statement:%s\n",sql);
        return -1;
    }
    return 0;
}
void connlost(void* context, char* cause)
{
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
}
int msgarrvd(void* context, char* topicName, int topicLen, MQTTClient_message* message)
{
    MYSQL *mysql = (MYSQL *)context;
    printf("Message arrived\n");
    printf("topic: %s\n", topicName);
    printf("message: %s\n",(char*)message->payload);
    //处理数据
    handler(mysql,message->payload,message->payloadlen);
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

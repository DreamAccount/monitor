//
// Created by 404NotFound on 2022/11/13.
//

#ifndef PROJECT1_NETWORK_H
#define PROJECT1_NETWORK_H

#include "MQTTClient.h"
#include <mysql/mysql.h>
#include <time.h>
typedef struct data_t
{
    time_t time;            //时间戳
    double tem;             //温度
    double  hum;            //湿度
    double light;           //光照
    double sohum;           //土壤湿度
}data_t;
void delivered(void* context, MQTTClient_deliveryToken dt); //检查消息是否已经成功交付
void parse_data(char *payload,int payloadlen,data_t *data); //解析数据
int msgarrvd(void* context, char* topicName, int topicLen, MQTTClient_message* message);
int handler(MYSQL *mysql, char *payload, int payloadlen);   //得到消息后执行的动作
void connlost(void* context, char* cause);                  //丢失心跳后执行的函数
#endif //PROJECT1_NETWORK_H

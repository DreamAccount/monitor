# 基于物联网的稻田监控系统
### 1.设计思路
```text
1.后台服务器和前台采集系统都接入mqtt服务器,前台采集系统定期将采集到的数据进行上传
2.后台服务器监控数据是否达到触发某行为的阈值,如果监控到数据超过设定的阈值,则进行相关操作
3.后台服务器会将近期的数据写入到后台数据库,以便于后期管理员需要时进行查看
```
### 2.具体监控的数据对象
* 1.时间(time)
* 2.温度(temperature)
* 3.湿度(humidity)
* 4.光照强度(light intensity)
* 5.土壤湿度(soil humidity)
### 3.具体实现
1.搭建mqtt服务器和mariadb数据库服务器

* 使用docker-compose进行服务构建
```yaml
version: "3.3"
services:
  mqtt:
    image: emqx/emqx
    container_name: mqtt_emqx
    hostname: mqtt_emqx
    ports:
      - 1883:1883   #mqtt port
      - 11833:11833 #mqtt local connect port
      - 8883:8883   #mqtt over ssl/tls
      - 8083:8083   #mqtt base ws
      - 8084:8084   #mqtt base wss
    restart: unless-stopped
  mariadb:
    image: mariadb:latest
    container_name: mariadb
    hostname: mariadb
    ports:
      - 3306:3306                 
    environment:
      - MYSQL_ROOT_PASSWORD=root  #set mariadb root password
    restart: unless-stopped
```
2.启动服务
```bash
docker-compose up -d
```
3.安装后台进程
```bash
git clone URL
cd dir
mkdir build && cd build && cmake .. && make -j8
```
4.启动后台进程
```bash
monitor -c /etc/monitor/config.json
```


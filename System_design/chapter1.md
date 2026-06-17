# Chapter 1

单机 -> 加database -> 加缓存 -> 加CDN ->加负载驱动 -> database scaling

low traffic -> 1*(web server + database)

high traffic -> load balancer(把traffic分发到多server上) + vertical/horizontal scaling(增加单机性能/增加更多server) + caching(高频访问放进内存)

read-heavy: 刷xhs,看商品, 绝大多数操作是read(cache/read replicas读写分离)

write-heavy: 下单,写入database,绝大多数是write(message queue,如kafka)

## single server setup

local host 访问域名 google.com 需要该IP地址 -> **DNS解析**

1.查找browser和OS的本地缓存域名IP

2.recursive resolver递归解析器 问internet service provider(verison..) 或者配置的DNS服务器

3.root name server 问根服务器.com,去TLD top level domain服务器

4.TLD server google.com? 5. Google authorirative name server 给出IP addr

有vpn节点时,DNS解析发生在vpn服务器

**TCP连接** 跟IP建立三次握手

**TLS加密** 在TCP连接上协议加密

**HTTP请求** 加密通道传输数据

最后web server返回HTML pages / JSON response

- 传统web app: server(business logic, storage..) <----> client (HTML展示,包含大量标签< div >< span >) 

- 现代前后端分离mobile app: web server <--HTTP protocol--> mobile app
  web server返回format JSON

```
GET /users/1
{
    "id":1,
    "address":{
        "street":"11 2nd st",
        "city": "NY",
    },
    "phoneNumbers":[
        "111 111 111",
        "222 222 222"
    ]
}

```  
## Database

分为 web server和 database server可以独立拓展,请求量增大可以加web server,数据量大可以单独升级database

Web Server(Web Tier) — 处理所有client请求,返回 HTML/JSON;它负责接收请求 & 执行业务逻辑 & 返回响应

Database Server（Data Tier） — 只负责存储和查询数据,Web Server 需要数据的时候,向 Database Server 发查询请求

database -- relational database & non relational database

- relational database : MySQL, PostgreSQL

支持JOIN & 强一致性。JOIN多表查询,一个字段把分散在不同表的data合并成一个完整结果

- non relational database : MongoDB(不支持JOIN,支持高性能和可拓展性, low latency, data are unstructured, store massive data)

1. only need serialize/deserialize(JSON XML YAML): 存取一个用户profile,读的时候解析,不需要JOIN跨表关联SQL查询

serialize 序列化- 数据结构(dict)转换成可以存储/传输的JSON字符串格式

deserialize 反序列化 - 将JSON解析回程序里的对象

2. massive data : SQL 保证事务的ACID特性, 横向scaling很复杂; NOSQL没有JOIN,加机器就能加容量,线性拓展

分类:
Key-Value Store — 一个大字典,比如 Redis。适合缓存、session 存储。

Document Store — 存 JSON/BSON 文档，每条数据可以有不同的结构。比如 MongoDB。适合内容管理、用户 profile

Column Store — 数据按列存而不是按行存，适合大规模分析查询。比如 Cassandra、HBase

Graph Store — 存节点和关系，适合社交网络、推荐系统。比如 Neo4j


## vertical scaling & horizontal scaling

vertical scale up - 单机加CPU RAM... - 简单, low traffic时可用, 单机goes down没有failover保护

horizontal scale out - 加servers - 支持更大traffic, 多用户连接web server需要 load balancer

## load balancer - web tier

distributes incoming traffic among  web servers

If server 1 goes offline, all the traffic will be routed to server 2. We will also add a new web server to the server pool to balance the load

## database replication - data tier

origin DB(master) and the copies DB(slaves)

DB大小: 一对多可以分摊read压力,大小相同,slave挂了可以用另一个，master goes offline可以把某个slave promoted to be the new master

data一致性: synchronous 同步, master write等到slave确认write完再返回succ,data强一致,但是慢

**asynchronous 异步**, master write之后立刻返回succ,后台再同步data给slave,短暂旧data,快


## Cache

Cache stores the result of responses or frequently accessed data in memory. **Redis**

read through cache: after receiving a request, a web server first checks if the cache has the available response.

If it has, it sends data back to the client. If not, it queries the DB, stores the response in the cache.

**consider using cache** 1.when data is read frequently and temporary saved 2.expiration policy

3.Consistency, data store and cache in sync 4.mitigating failure, to avoid "a single point of failure",

set multiple cache servers across different data centers

## Content delivery network(CDN)内容分发网络

存静态资源static content,(图片 JS/CSS/Image,视频,不怎么变的HTML页面)复制网站上不经常变动的东西,存在各个地方更近的servers里,解决:网络延迟latency,Server Overload

 - 1.user 向系统要一张图片。2.请求先到达最近的 CDN 节点 3.CDN 找servers里没有这张图(Cache Miss,缓存未命中)

  4.CDN 节点替user去拉主服务器(Origin Server)这张图 5.主服务器把图片给 CDN,CDN 自己存一份(Cache),再把图片吐给用户

  6.第二个user再来要这张图时,CDN 找到了(Cache Hit,缓存命中),直接秒发给第二个用户，不需要再麻烦主服务器

  consideration of using a CDN: 1.三方提供,cost问题 2.合适的cache expiry time

关于CDN缓存一致性: 版本化URL versioning. user从main server中获得最新HTML,main server给user的HTML中有最新new123.png,CDN里没有,cache miss,CDN节点

从main server中获取最新的new123.png.之前的旧缓存expiry time到期删掉即可

## stateless web tier - web server层的拓展

session: 存某个user一段持续的状态信息

stateful: client的所有request都打到某个web server,否则session状态丢了. A->server1,B->server2,C->server3,后续只能访问各自server,
web server有状态

解决方案stateless: session数据从web server中取出来,存入一个共享的外部存储NoSQL(redis),每台web server都是stateless的,任何一台都能处理client的request.

web server不保留用户状态,NoSQL可以解决并发读写冲突、文件锁、读写速度慢的问题

## message queue

data center的traffic分流/data同步/自动化测试部署

在web server阶段message queue可以异步处理request

logging在很多用户时很重要,记录server的状态;metrics可以记录CPU/DB tier/cache/business;测试部署的automation

DB的vertical horizonal拓展，分别是scale up和sharding,scale up是加CPU RAM坏处是可能导致单个节点fail就全fail;sharding更灵活,跟key相关,比如可以%4,但是会导致otspot数据过多,比如lady gaga相关,需要分到一个单独的db中

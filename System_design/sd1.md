# Web design

## terms

Endpoint：一个 API 的具体入口，比如 `POST /posts`

Bottleneck：当前限制系统能力的最弱环节

### review

- 用户打开一个网页，浏览器、Web Server、Application Server、Database 分别做了什么？

    浏览器通过DNS找到server的ip地址,把url,内容包装进一个HTTP request里发给对应IP的web server

    web server 处理静态资源和转发请求,返回用户html页面/CSS/图片;动态请求转发给application server

    application server 处理业务逻辑/查DB/write DB,结果组装成JSON返回web server; web server返回给browser, browser拿到response后渲染界面

    database 存和查结构化数据,user info和订单

- 为什么`无状态服务`更容易水平扩展？
    因为stateless server中user state存在另一个NoSql(比如Redis)里,任何一台机器都能处理user request,load balancer可以自由分配流量,

    所以可以水平scale多个server从NoSql里获得user state

    相反如果是stateful server,user state保存在指定server中,request必须route到指定server,load balancer失去自由分配的能力,server run off用户状态会丢失,

    因此前者更容易scale out

- 如果数据库`读`压力太大，你会先考虑哪三种办法？

    我会考虑加上cache存储hotspot数据;加上多个slave DB读副本 read replicas;根据地域设置CDN分发静态资源让user更快加载界面数据

- 如果数据库`写`压力太大，为什么只加读副本不够？

    read replicas只涉及read request,所有write request操作还是route到master DB上,对于缓解压力没有帮助

    解决方案是`sharding(分库分表)或者是message queue`异步处理削峰

- 负载均衡器怎么知道某台后端机器已经坏了？

    `health check`,load balancer定期主动向每台server发探测请求,如果连续几次没响应或者返回error,标记成不可用&不再分配流量

### question

一个基础 Web 系统从 1 台机器扩展到多台机器时，哪些组件会被拆出来，为什么要拆，以及每一种扩展手段解决的是哪类问题。

web server和database会被拆出来,解决无法分别单独扩容的问题,web server扩容可以解决多用户请求的问题,db扩容解决用户写入数据量大的问题。如果web server增加,需要加上load balancer去管理user request route到哪个web server。如果db增加,需要加上一个nosql管理用户状态才可以拓展db

## web 应用

1.浏览器发HTTP request,request 包含 GET读数据, POST提交数据, URL访问某个资源, headers(认证tokens), body(POST)

2.server收到request,生成一个HTML document

3.server把HTML doc作为HTTP response的payload, Payload 就是 body,Payload指去掉协议头、元数据之后，真正要传的有效信息

HTTP response 里有很多东西：状态码、headers、body。状态码和 headers 是信封，body 才是信。

服务器把 HTML 文档放在 HTTP response 的 body 里发给浏览器，浏览器拿到后渲染成页面。

### 路由与视图

domain name 域名/ path路径

url: www.XYZ.com/index.html?id=xxx

**url是可变的,看到的界面也会随之变化,这是路由.**

比如server收到 /index.html?id=1 和 /about 这两个请求，路由机制就是一个映射表：/index → 执行函数 A，/about → 执行函数 B。

不同的 URL 被映射到不同的处理逻辑，所以返回的内容不一样，用户看到的界面就不一样

**url甚至还可以传递参数** www.XYZ.com/profile.php?id=xxx

Add_url_rule('/profile', user_profile_page)

Function profile(id: int):
    return '<h1> user:{id}</h1>'

### request & responce

1.web server会监听一个port等待request进来,HTTP是80,HTTPS是443

2.client send request 到80

3.server收到request,根据url mapping查找对应视图;调用视图后讲对应界面作为response响应的内容。HTTP协议中状态码也很重要200(succ)404(fail)302(重定向)

视图函数: 生成request对应的response

用户在web form输入地址和密码提交后,生成POST request;server收到request,route到视图函数。视图函数需要1.访问DB添加新用户2.生成响应回送给browser,指明状态码

业务逻辑和表现逻辑混用会导致代码难以理解和维护 需要解耦decoupling。

比如HTML文件固定部分作为template,动态部分使用placeholder占位符,真实值替换变量返回最终的response字符串,这个过程为渲染rendering

### DB

业务逻辑操作DB

1.DB接受SQL接口,业务逻辑中加入SQL语句,调用DB并执行 INSERT INTO users VALUES(1, xx@gmail.com, passwd)

2.DB和业务代码中加入抽象层,ORM对象关系映射,隐藏数据库直接操作对象 db.session.add(user); db.session.commit()

### case: 看别人的帖子

ERD(entity relationship diagram) 表示DB中有哪些表,每张表有哪些字段,表与表的关系

entity是每张表

relationship是表之间的关联

`function requirement:`

- user需要sign up/sign in
- follow 其他user
- 看其他user的帖子

用两张**模型表**存数据实体，一张**关联表**记录多对多关系

follows users posts

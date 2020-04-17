# 网络编程

> * [网络编程的步骤](#网络编程的步骤)
> * [常用API](#常用API)
> * [TCP中的accept和connect和listen的关系](#TCP中的accept和connect和listen的关系)
> * [UDP中的connect](#UDP中的connect)
> * [广播和组播过程](#广播和组播过程)
> * [服务端大量TIMEWAIT或CLOSEWAIT状态](#服务端大量TIMEWAIT或CLOSEWAIT状态)
> * [复位报文段RST](#复位报文段RST)
> * [优雅关闭和半关闭](#优雅关闭和半关闭)
> * [解决TCP粘包](#解决TCP粘包)
> * [select可以判断网络断开吗](#select可以判断网络断开吗)
> * [send和read的阻塞和非阻塞情况](#send和read的阻塞和非阻塞情况)
> * [网络字节序和主机序](#网络字节序和主机序)
> * [IP地址分类及转换](#IP地址分类及转换)
> * [select实现异步connect](#select实现异步connect)
> * [为什么忽略SIGPIPE信号](#为什么忽略SIGPIPE信号)
> * [如何设置非阻塞](#如何设置非阻塞)



## 网络编程步骤

### TCP

* 服务端：**socket -> bind -> listen -> accept -> recv/send -> close**
  * 创建一个socket，用函数socket()，设置SOCK_STREAM
  * 设置服务器地址和侦听端口，初始化要绑定的网络地址结构
  * 绑定服务器端IP地址、端口等信息到socket上，用函数bind()
  * 设置允许的最大连接数，用函数listen()
  * 接收客户端上来的连接，用函数accept()
  * 收发数据，用函数send()和recv()，或者read()和write()
  * 关闭网络连接close()，需要关闭服务端sock和accept产生的客户端sock文件描述符

* 客户端：**socket -> connect -> send/recv -> close**
  * 创建一个socket，用函数socket()
  * 设置要连接的对方的IP地址和端口等属性
  * 连接服务器，用函数connect()
  * 收发数据，用函数send()和recv()，或read()和write()
  * 关闭网络连接close()
* 注意
  * INADDR_ANY表示本机任意地址，一般服务器端都可以这样写
  * accept中接收的是客户端的地址，返回对应当前客户端的一个clisock文件描述符，表示当前客户端的tcp连接
  * send和recv中接收的是新建立的客户端的sock地址

### UDP

* 服务端：**socket -> bind -> recvfrom/sendto -> close**
  * 建立套接字文件描述符，使用函数socket()，设置SOCK_DGRAM
  * 设置服务器地址和侦听端口，初始化要绑定的网络地址结构
  * 绑定侦听端口，使用bind()函数，将套接字文件描述符和一个地址类型变量进行绑定
  * 接收客户端的数据，使用recvfrom()函数接收客户端的网络数据
  * 向客户端发送数据，使用sendto()函数向服务器主机发送数据
  * 关闭套接字，使用close()函数释放资源
* 客户端：**socket -> sendto/recvfrom -> close**
  * 建立套接字文件描述符，socket()
  * 设置服务器地址和端口，struct sockaddr
  * 向服务器发送数据，sendto()
  * 接收服务器的数据，recvfrom()
  * 关闭套接字，close()
* 注意
  * sendto和recvfrom的第56个参数是sock地址
    * 服务器端的recvfrom和sendto都是cli地址
    * 客户端sendto是服务器端的地址，最后一个参数是指针，recvfrom是新建的from地址，最后一个参数是整型
  * UDP不用listen，accept，因为UDP无连接
  * UDP通过sendto函数完成套接字的地址分配工作
    * 第一阶段：向UDP套接字注册IP和端口号
    * 第二阶段：传输数据
    * 第三阶段：删除UDP套接字中注册的目标地址信息
  * 每次调用sendto函数都重复上述过程，每次都变更地址，因此可以重复利用同一UDP套接字向不同的目标传输数据

## 常用API

sendto、recvfrom保存对端的地址

* sendto
* recvfrom

## [TCP中的accept和connect和listen的关系](https://blog.csdn.net/tennysonsky/article/details/45621341)

### listen

* listen功能
  * **设定同时连接请求上限**
  * listen函数把一个未连接的套接字转换成一个被动套接字，指示内核应接受指向该套接字的连接请求
  * 参数 backlog 的作用是设置内核中连接队列的长度
  * 根据TCP状态转换图，调用listen导致套接字从CLOSED状态转换成LISTEN状态。 

* 是否阻塞
  * listen()函数**不会阻塞**，它将该套接字和套接字对应的连接队列长度告诉 Linux 内核，然后，listen()函数就结束。

* backlog的作用
  * backlog是队列的长度，内核为任何一个给定的监听套接口维护两个队列：
    * **未完成连接队列**（incomplete connection queue），每个这样的 SYN 分节对应其中一项：已由某个客户发出并到达服务器，而服务器**正在等待完成相应的 TCP 三次握手过程**。这些套接口处于 SYN_RCVD 状态。
    * **已完成连接队列**（completed connection queue），每个已完成 TCP 三次握手过程的客户对应其中一项。这些套接口处于 ESTABLISHED 状态
  * 当有一个客户端主动连接（connect()），Linux 内核就自动完成TCP 三次握手，该项就**从未完成连接队列移到已完成连接队列的队尾**，将建立好的链接自动存储到队列中，如此重复
  * backlog 参数历史上被定义为上面两个队列的大小之和，大多数实现默认值为 5

### connect

* connect功能
  * 对于客户端的 connect() 函数，该函数的功能为客户端主动连接服务器，建立连接是通过三次握手，而这个连接的过程是由内核完成，不是这个函数完成的，**这个函数的作用仅仅是通知 Linux 内核**，**让 Linux 内核自动完成 TCP 三次握手**连接最后把连接的结果返回给这个函数的返回值（成功连接为0， 失败为-1）。
  * connect**之后**是三次握手

* 是否阻塞
  * 通常的情况，客户端的connect() 函数**默认会一直阻塞**，直到三次握手成功或超时失败才返回（正常的情况，这个过程很快完成）。


### accept

* accept功能
  * accept()函数功能是，从处于 established 状态的连接队列头部取出一个已经完成的连接(**三次握手之后**)

* 是否阻塞
  * 如果这个队列**没有已经完成的连接**，accept()函数就**会阻塞**，直到取出队列中已完成的用户连接为止。 
  * 如果，服务器不能及时调用 accept() 取走队列中已完成的连接，**队列满**掉后会怎样呢？UNP（《unix网络编程》）告诉我们，服务器的连接队列满掉后，**服务器不会对再对建立新连接的syn进行应答**，所以客户端的 connect 就会**返回 ETIMEDOUT**

## [UDP中的connect](https://blog.csdn.net/woshiluoye9/article/details/63718231)

UDP的connect和TCP的connect完全不同，UDP不会引起三次握手

- 当使用了这个方法之后，那么就限定了这个socket的使用范围： 只允许从这个指定的SocketAddress 上获得数据包和向这个指定的SocketAddress 发送数据包， 当你一旦通过这个socket向别的地址发送了数据包，或者接收到了不是这个地址发送来的数据包，那么程序就会抛出IllegalArgumentException 异常， 特殊的是如果指定的这个SocketAddress 是个多播地址或者广播地址，那么只允许向这个地址发送数据包，不允许从这个地址接收数据包.

* 可以提高传输效率
* 采用connect的UDP发送接受报文可以调用send,write和recv,read操作，也可以调用sendto,recvfrom，此时需要将第五和第六个参数置为NULL或0
* 多次调用connect拥有一个已连接UDP套接口的进程的作用
  * 指定新的IP地址和端口号
  * 断开套接口

## 广播和组播过程

* 广播
  * 只适用于局域网
  * 只能向同一网络中的主机传输数据，
* 组播
  * 适用于局域网和广域网（internet） 

## 服务端大量TIMEWAIT或CLOSEWAIT状态

首先通过TCP的四次挥手过程分析确定两个状态的出现背景。TIMEWAIT是**大量tcp短连接**导致的，确保对方收到最后发出的ACK，一般为2MSL；CLOSEWAIT是tcp连接不关闭导致的，出现在close()函数之前。

### [TIMEWAIT](https://www.cnblogs.com/dadonggg/p/8778318.html)

* TIME_WAIT状态存在的理由：

  - 可靠地实现TCP全双工连接的终止（客户端必须维护状态信息允许它重发最终的ACK）。
  - 允许老的重复分节在网络中消逝 (解决TCP超时重传而导致的化身连接问题)。

* 可以通过设置SOCKET选项SO_REUSEADDR（重用端口）来重用处于TIMEWAIT的sock地址，对应于内核中的tcp_tw_reuse，这个参数不是“消除” TIME_WAIT的，而是说当资源不够时，可以重用TIME_WAIT的连接一个端口释放后会**等待两分钟**之后才能再被使用，SO_REUSEADDR是让端口释放后立即就可以被再次使用

* 修改ipv4.ip_local_port_range，增大可用端口范围，来承受更多TIME

* 设置SOCK选项SO_LINGER选项，这样会直接消除TIMEWAIT

* 编辑内核文件/etc/sysctl.conf，加入以下内容：

  ```
  net.ipv4.tcp_syncookies = 1 表示开启SYN Cookies。当出现SYN等待队列溢出时，启用cookies来处理，可防范少量SYN攻击，默认为0，表示关闭；
  net.ipv4.tcp_tw_reuse = 1 表示开启重用。允许将TIME-WAIT sockets重新用于新的TCP连接，默认为0，表示关闭；
  net.ipv4.tcp_tw_recycle = 1 表示开启TCP连接中TIME-WAIT sockets的快速回收，默认为0，表示关闭。
  net.ipv4.tcp_fin_timeout 修改系默认的 TIMEOUT 时间
  ```

### [CLOSEWAIT](https://blog.csdn.net/libaineu2004/article/details/78886182)

在被动关闭连接情况下，在**已经接收到FIN，但是还没有发送自己的FIN的时刻**，连接处于CLOSE_WAIT状态

解决方法：

出现大量close_wait的现象，主要原因是某种情况下对方关闭了socket链接，但是我方忙与读或者写，没有关闭连接。代码需要判断socket，一旦读到0，断开连接，read返回负，检查一下errno，如果不是AGAIN，就断开连接。

- 代码需要判断socket，一旦read返回0，断开连接，read返回负，检查一下errno，如果不是AGAIN，也断开连接。(注:在UNP 7.5节的图7.6中，可以看到使用select能够检测出对方发送了FIN，再根据这条规则就可以处理CLOSE_WAIT的连接)

- 给每一个socket设置一个时间戳last_update，每接收或者是发送成功数据，就用当前时间更新这个时间戳。定期检查所有的时间戳，如果时间戳与当前时间差值超过一定的阈值，就关闭这个socket。

- 使用一个Heart-Beat线程，定期向socket发送指定格式的心跳数据包，如果接收到对方的RST报文，说明对方已经关闭了socket，那么我们也关闭这个socket。
- 设置SO_KEEPALIVE选项，并修改内核参数



## 复位报文段RST

* 访问不存在的端口，或服务器端没有启动
* 异常终止连接
  * TCP提供了异常终止连接的方法，给对方发送一个复位报文段
  * 此时对端read会返回-1，显示错误errno:Connection reset by peer
  * 这种错误可以通过shutdown来解决
* 处理半打开连接
  *  当某端崩溃退出，此时对端并不知道，若往对端发送数据，会响应一个RST复位报文段

## [优雅关闭和半关闭](https://www.cnblogs.com/liyulong1982/p/3990740.htm)

### 概念

* **优雅关闭**是指，如果发送缓存中还有数据未发出则其发出去，并且收到所有数据的ACK之后，发送FIN包，开始关闭过程。
* **强制关闭**是指如果缓存中还有数据，则这些数据都将被丢弃，然后发送RST包，直接重置TCP连接。
* close只是减少引用计数，只有当引用计数为0的时候，才发送fin，真正关闭连接
* shutdown不同，只要以SHUT_WR/SHUT_RDWR方式调用即发送FIN包
* shutdown后要调用close（shutdown函数用于关闭TCP连接，但并不关闭socket句柄）
* 保持连接的某一端想关闭连接了，但它**需要确保要发送的数据全部发送完毕以后才断开连接**，此种情况下需要使用优雅关闭，一种是**shutdown**，一种是设置**SO_LINGER的close**
* **半关闭**：当 TCP 链接中 A 发送 FIN 请求关闭， B 端回应 ACK 后（A 端进入 FIN_WAIT_2 状态）， B 没有立即发送 FIN 给 A时， A 方处在半链接状态，此时 A 可以接收 B 发送的数据，但是 A 已不能再向 B 发送数据。
* 从程序的角度， 可以使用 API 来控制实现半连接状态。  

### close

close函数会关闭文件描述符，不会立马关闭网络套接字，除非引用计数为0，则会触发调用关闭TCP连接。

* 检查接收缓冲区是否有数据未读(不包括FIN包)，如果有数据未读，协议栈会发送RST包，而不是FIN包。如果套接字设置了SO_LINGER选项，并且lingertime设置为0，这种情况下也会发送RST包来终止连接。其他情况下，会检查套接字的状态，只有在套接字的状态是TCP_ESTABLISHED、TCP_SYN_RECV和TCP_CLOSE_WAIT的状态下，才会发送FIN包
* 若有多个进程调用同一个网络套接字，会将网络套接字的文件描述符+1，close调用只是将当前套接字的文件描述符-1，只会对当前的进程有效，只会关闭当前进程的文件描述符，其他进程同样可以访问该套接字
* close函数的默认行为是，关闭一个socket，close将立即返回，TCP模块尝试把该socket对应的TCP缓冲区中的残留数据发送给对方，并不保证能到达对方
* close行为可以通过SO_LINGER修改

```C++
struct linger{
    int l_onoff;    //开启或关闭该选项
    int l_linger;   //滞留时间
}
```

* l_onoff为0，该选项不起作用，采用默认close行为。closesocket会立即返回，并关闭用户socket句柄。如果此时缓冲区中有未发送数据，则系统会在后台将这些数据发送完毕后关闭TCP连接，是一个**优雅关闭**过程。
* l_onoff不为0
  * l_linger为0，close立即返回，TCP模块丢弃被关闭的socket对应的TCP缓冲区中的数据，给对方发送RST复位信号，这样可以异常终止连接，且完全消除了TIME_WAIT状态。这是一个**强制关闭**过程。
  * l_linger不为0
    * 阻塞socket，此时如果缓冲区中有未发送数据，如果TCP在l_linger表明的时间内将所有数据发出，则发完后关闭TCP连接，这时是优雅关闭过程；如果如果TCP在l_linger表明的时间内没有将所有数据发出，则会丢弃所有未发数据然后TCP发送RST包重置连接，此时就是一个**强制关闭**过程了。
    * 非阻塞socket，close立即返回，需要根据返回值和errno判断残留数据是够发送完毕。

### shutdown

shutdown没有采用引用计数的机制，会影响所有进程的网络套接字，可以只关闭套接字的读端或写端，也可全部关闭，用于实现半关闭，会直接发送FIN包

* SHUT_RD，关闭sockfd上的读端，不能再对sockfd文件描述符进行读操作，且接收缓冲区中的所有数据都会丢弃
* SHUT_WR，关闭写端，确保发送缓冲区中的数据会在真正关闭连接之前会发送出去，不能对其进行写操作，连接处于半关闭状态
* SHUT_RDWR，同时关闭sockfd的读写

## [解决TCP粘包](https://blog.csdn.net/weixin_41047704/article/details/85340311)

### 什么是TCP粘包

由于TCP是流协议，因此TCP接收不能确保每次一个包，有可能接收一个包和下一个包的一部分。

### 产生粘包的两种情况

- TCP默认使用Nagle算法（主要作用：减少网络中报文段的数量），而Nagle算法主要做两件事：

  - 只有上一个分组得到确认，才会发送下一个分组

  - 收集多个小分组，在一个确认到来时一起发送

- 当发送内容较大时，由于服务器端的recv（buffer_size）方法中的buffer_size较小，不能一次性完全接收全部内容，因此在下一次请求到达时，接收的内容依然是上一次没有完全接收完的内容，因此造成粘包现象。

### 如何解决

思路一：禁止nagle算法（将套接字描述符设置TCP_NODELAY选项可以禁止nagle算法）

思路二：由于产生粘包的原因是接收方的无边界接收，因此发送端可以在发送数据之前向接收端告知发送内容的大小即可。包头和包体，包头中有包体长度。

UDP则是面向消息传输的，是有保护消息边界的，接收方一次只接受一条独立的信息，所以不存在粘包问题。

举个例子：有三个数据包，大小分别为2k、4k、6k，如果采用UDP发送的话，不管接受方的接收缓存有多大，我们必须要进行至少三次以上的发送才能把数据包发送完，但是使用TCP协议发送的话，我们只需要接受方的接收缓存有12k的大小，就可以一次把这3个数据包全部发送完毕。

## select可以直接判断网络断开吗

不可以。

若网络断开，select检测描述符会发生读事件，这时调用read函数发现读到的数据长度为0，表示网络断开。

## [请你说一下阻塞，非阻塞，同步，异步](https://www.cnblogs.com/guozht/p/11236199.html)

- 阻塞和非阻塞：调用者在事件没有发生的时候，一直在等待事件发生，不能去处理别的任务这是阻塞。调用者在事件没有发生的时候，可以去处理别的任务这是非阻塞。

- 同步和异步：调用者必须循环自去查看事件有没有发生，这种情况是同步。调用者不用自己去查看事件有没有发生，而是等待着注册在事件上的回调函数通知自己，这种情况是异步

## 如何设置文件描述符非阻塞

* 通过fcntl设置

```c++
int flag = fcntl(fd, F_GETFL);
flag |= O_NONBLOCK;
fctncl(fd, F_SETFL, flag);
```

## [send和recv的阻塞和非阻塞情况](https://blog.csdn.net/shy_hc/article/details/69950334?depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-1&utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-1)

send函数返回100，并不是将100个字节的数据发送到网络上或对端，而是发送到了协议栈的写缓冲区，至于什么时候发送，由协议栈决定。

### send

ssize_t send(int sockfd, const void *buf, size_t len, int flags);

* 阻塞模式下

  * 一直等待，直到写缓冲区有空闲

    - 成功写返回发送数据长度

    - 失败返回-1

* 非阻塞模式下

  * 不等待，立即返回，成功返回数据长度
  * 返回-1，判断**错误码**
    *  若错误码为EAGAIN或EWOULDBLOCK则表示写缓冲区不空闲
    *  若错误码为ERROR，则表示失败

### recv

ssize_t recv(int sockfd, void *buf, size_t len, int flags);

* 阻塞模式下
  * 一直等待，直到读缓冲区有数据
    * 成功写返回数据长度
    * 失败返回-1
* 非阻塞模式下
  * 不等待，立即返回，成功返回数据长度
  * 返回-1，判断**错误码**
    *  若错误码为EAGAIN或EWOULDBLOCK则表示读缓冲区没数据
    *  若错误码为ERROR，则表示失败
  * 返回0
    * 对端关闭连接 

## 网络字节序和主机序

字节序分为大端字节序和小端字节序，大端字节序也称网络字节序，小端字节序也称为主机字节序。

* 大端字节序（网络字节序）
  * 一个整数的**高位字节**存储在**低位地址**，**低位字节**存储在**高位地址** 
* 小端字节序（主机字节序）
  *  高位字节存储在高位地址，低位字节存储在低位地址
* 转换API
  * htonl 主机序转网络序，长整型，用于转换IP地址   uint32_t htonl(uint32_t hostlong);
  * htons 主机序转网络序，短整型，用于转换端口号  uint16_t htons(uint16_t hostshort);
  * ntohl 网络序转主机序   uint32_t ntohl(uint32_t netlong);
  * ntohs 网络序转主机序  uint16_t ntohs(uint16_t netshort);

## [IP地址分类及转换](https://blog.csdn.net/kzadmxz/article/details/73658168)

### IP地址分类

### IP转换

字符串表示的点分十进制转换成网络字节序的IP地址

* pton，点分十进制转换成地址
* ntop，地址转换成点分十进制

## [select、poll、epoll](https://www.cnblogs.com/Anker/p/3265058.html)

select，poll，epoll都是IO多路复用的机制。I/O多路复用就通过一种机制，可以监视多个描述符，一旦某个描述符就绪（一般是读就绪或者写就绪），能够通知程序进行相应的读写操作。但select，poll，epoll本质上都是同步I/O，因为他们都需要在读写事件就绪后自己负责进行读写，也就是说这个读写过程是阻塞的，而异步I/O则无需自己负责进行读写，异步I/O的实现会负责把数据从内核拷贝到用户空间。 

- select==>时间复杂度O(n)

  ```c++
  int select(int nfds, fd_set *readfds, fd_set *writefds,fd_set *exceptfds, struct timeval *timeout);
  ```

  它仅仅知道了，有I/O事件发生了，却并不知道是哪那几个流（可能有一个，多个，甚至全部），我们只能无差别轮询所有流，找出能读出数据，或者写入数据的流，对他们进行操作。所以select具有O(n)的无差别轮询复杂度，同时处理的流越多，无差别轮询时间就越长。

- poll==>时间复杂度O(n)

  ```c++
  int poll(struct pollfd *fds, nfds_t nfds, int timeout);
  ```

  poll本质上和select没有区别，它将用户传入的数组拷贝到内核空间，然后查询每个fd对应的设备状态， 但是它没有最大连接数的限制，原因是它是基于链表来存储的.

- [epoll](https://blog.csdn.net/daaikuaichuan/article/details/83862311)==>时间复杂度O(1)（红黑树存储socket文件描述符，双向链表存储就绪的事件）

  ```c++
  int epoll_create(int size);
  int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
  int epoll_wait(int epfd, struct epoll_event * events, int maxevents, int timeout);
  ```

  epoll可以理解为event poll，不同于忙轮询和无差别轮询，epoll会把哪个流发生了怎样的I/O事件通知我们。所以我们说epoll实际上是事件驱动（每个事件关联上fd）的，此时我们对这些流的操作都是有意义的。（复杂度降低到了O(1)）

## [select实现异步connect](https://blog.csdn.net/nphyez/article/details/10268723)

通常阻塞的connect 函数会等待三次握手成功或失败后返回，0成功，-1失败。如果对方未响应，要隔6s，重发尝试，可能要等待75s的尝试并最终返回超时，才得知连接失败。即使是一次尝试成功，也会等待几毫秒到几秒的时间，如果此期间有其他事务要处理，则会白白浪费时间，而用非阻塞的connect 则可以做到并行，提高效率。

### 实现步骤

* 创建socket，返回套接字描述符；
* 调用fcntl 把套接字描述符设置成**非阻塞**；
* 调用connect 开始建立连接；
* 判断连接是否成功建立。

### 判断连接是否成功建立：

* 如果为**非阻塞**模式，则调用connect()后函数立即返回，如果连接不能马上建立成功（返回-1），则errno设置为EINPROGRESS，此时TCP三次握手仍在继续。 
  * 如果connect 返回0，表示连接成功（服务器和客户端在同一台机器上时就有可能发生这种情况）
  * 失败可以调用select()检测非阻塞connect是否完成。select指定的超时时间可以比connect的超时时间短，因此可以防止连接线程长时间阻塞在connect处。
* 调用select 来等待连接建立成功完成；
  * 如果select 返回0，则表示建立连接超时。我们返回超时错误给用户，同时关闭连接，以防止三路握手操作继续进行下去。
  * 如果select 返回大于0的值，并不是成功建立连接，而是表示套接字描述符可读或可写
    *  当连接建立成功时，套接字描述符变成可写（连接建立时，写缓冲区空闲，所以可写）
    *  当连接建立出错时，套接字描述符变成既可读又可写（由于有未决的错误，从而可读又可写）
  * 如果套接口描述符可写，则我们可以通过调用getsockopt来得到套接口上待处理的错误（SO_ERROR）
    * 如果连接建立成功，这个错误值将是0
    * 如果建立连接时遇到错误，则这个值是连接错误所对应的errno值（比如：ECONNREFUSED,ETIMEDOUT等）。

## epoll的实现知道么？在内核当中是什么样的数据结构进行存储，每个操作的时间复杂度是多少？

```c++
int epoll_create(int size);  
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);  
int epoll_wait(int epfd, struct epoll_event *events,int maxevents, int timeout);  
```

1. 调用epoll_create建立一个epoll对象(在epoll文件系统中给这个句柄分配资源)；

2. 调用epoll_ctl向epoll对象中添加这100万个连接的套接字；

3. 调用epoll_wait收集发生事件的连接。

我们在调用epoll_create时，内核除了帮我们在epoll文件系统里建了个file结点，在内核cache里建了个**红黑树**用于存储以后epoll_ctl传来的**socket**外，还会再建立一个rdllist**双向链表**，用于**存储准备就绪的事件**，当epoll_wait调用时，仅仅观察这个rdllist双向链表里有没有数据即可。有数据就返回，没有数据就sleep，等到timeout时间到后即使链表没数据也返回。所以，epoll_wait非常高效。

当调用epoll_wait检查是否有发生事件的连接时，只是检查eventpoll对象中的rdllist双向链表是否有epitem元素而已，如果rdllist链表不为空，则这里的事件复制到用户态内存（使用共享内存提高效率）中，同时将事件数量返回给用户。因此epoll_waitx效率非常高。epoll_ctl在向epoll对象中添加、修改、删除事件时，从rbr红黑树中查找事件也非常快，也就是说epoll是非常高效的，它可以轻易地处理百万级别的并发连接。

**总结**

- 执行epoll_create()时，创建了红黑树和就绪链表；

- 执行epoll_ctl()时，如果增加socket句柄，则检查在红黑树中是否存在，存在立即返回，不存在则添加到树干上，然后向内核注册回调函数，用于当中断事件来临时向准备就绪链表中插入数据；

- 执行epoll_wait()时立刻返回准备就绪链表里的数据即可。

## epoll的两种触发模式

epoll有EPOLLLT和EPOLLET两种触发模式，LT是默认的模式，ET是“高速”模式。

- LT（水平触发）模式下，只要这个文件描述符还有数据可读，每次 epoll_wait都会返回它的事件，提醒用户程序去操作；

- ET（边缘触发）模式下，在它检测到有 I/O 事件时，通过 epoll_wait 调用会得到有事件通知的文件描述符，对于每一个被通知的文件描述符，如可读，则必须将该文件描述符一直读到空，让 errno 返回 EAGAIN 为止，否则下次的 epoll_wait 不会返回余下的数据，会丢掉事件。如果ET模式不是非阻塞的，那这个一直读或一直写势必会在最后一次阻塞。

**epoll为什么要有EPOLLET触发模式？**

  如果采用EPOLLLT模式的话，系统中一旦有大量你不需要读写的就绪文件描述符，它们每次调用epoll_wait都会返回，这样会**大大降低处理程序检索自己关心的就绪文件描述符的效率.**。而采用EPOLLET这种边缘触发模式的话，当被监控的文件描述符上有可读写事件发生时，epoll_wait()会通知处理程序去读写。如果这次没有把数据全部读写完(如读写缓冲区太小)，那么下次调用epoll_wait()时，它不会通知你，也就是它只会通知你一次，直到该文件描述符上**出现第二次可读写事件才会通知**你！！！这种模式比水平触发**效率高**，系统不会充斥大量你不关心的就绪文件描述符。

**总结**：

- ET模式（边缘触发）**只有数据到来才触发**，**不管缓存区中是否还有数据**，缓冲区剩余未读尽的数据不会导致epoll_wait返回；
- LT 模式（水平触发，默认）**只要有数据都会触发**，缓冲区剩余未读尽的数据会导致epoll_wait返回。

## 为什么忽略SIGPIPE信号

* 假设server和client 已经建立了连接，server调用了close, 发送FIN 段给client（其实不一定会发送FIN段，后面再说），此时server不能再通过socket发送和接收数据，此时client调用read，如果接收到FIN 段会返回0
* 但client此时还是可以write 给server的，write调用只负责把数据交给TCP发送缓冲区就可以成功返回了，所以不会出错，而server收到数据后应答一个RST段，表示服务器已经不能接收数据，连接重置，client收到RST段后无法立刻通知应用层，只把这个状态保存在TCP协议层。
* 如果client再次调用write发数据给server，由于TCP协议层已经处于RST状态了，因此不会将数据发出，而是发一个SIGPIPE信号给应用层，SIGPIPE信号的缺省处理动作是终止程序。

* 有时候代码中需要连续多次调用write，可能还来不及调用read得知对方已关闭了连接就被SIGPIPE信号终止掉了，这就需要在初始化时调用sigaction处理SIGPIPE信号，对于这个信号的处理我们通常忽略即可

* 往一个读端关闭的管道或者读端关闭的socket连接中写入数据，会引发SIGPIPE信号。当系统受到该信号会结束进程是，但我们不希望因为错误的写操作导致程序退出。

* 通过sigaction函数设置信号，将handler设置为SIG_IGN将其忽略
* 通过send函数的MSG_NOSIGNAL来禁止写操作触发SIGPIPE信号

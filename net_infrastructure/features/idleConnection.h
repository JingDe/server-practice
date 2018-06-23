
/*
    1, 使用timing wheel踢掉空闲连接

    需求：超时8秒断开连接
    数据结构：8个桶组成的循环队列。第一个桶放1秒之后超时的连接，第2个桶放2秒之后超时的连接。
    使用组件：每秒的timer
    算法：
        每个连接一旦接受到数据，即将自己放到第8个桶中。
        每秒的timer，将第一个桶里的连接断开，并把这个桶移到队尾。

    实现：
    数据结构：一个循环队列，一个指向队尾的指针。
        1)使用循环数组/vector conn[8]，数组元素是是set<连接的shared_ptr>，一个指针指向剩余生命周期1秒的位置cur
        每秒的定时器到时，断开conn[cur]的set中的所有连接，并清空set，cur指向下一个位置。
        新的连接插入到cur之前的set中。
        连接有数据到达时，从所在位置oldPos移动到cur之前的位置newPos。
            或者，不从oldPos删除，直接插入到newPos，连接的shared_ptr的引用计数增加。
            连接对象记录自己所在的位置。


    2， 使用链表+选择排序，踢掉链表开头的超时连接


    3, TcpServer，管理所有的连接。

*/
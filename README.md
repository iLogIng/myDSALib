
# myDSALib

> **2025.22.07**

> 模板化实现的数据结构库。
> 该库是一个实验性质的库，是个人根据所学的数据结构课程进行实验性编写的，实现了各个所学数据结构的类型、对应算法的库。
> 在设计时，并未将算法效率作为第一要素，而是将实现算法效果作为第一要素。不具备实用性质。

## 项目结构

### 主目录

**CMakeLists.txt** 文件进行项目构建。

**main.cpp** 最初，没有接触 **gTest** 引入测试，使用该文件运行各个结构进行测试。

**DSALib** 主目录。

- **include**

  - **DisjointSet** 不相交集实现
    - DisjointSet.hpp

  - **Graph** 图结构实现，仅进行了邻接矩阵为基础的实现
    - GraphInterface.hpp
        > 图接口
    - AdjMatrix.hpp
        > 邻接链表实现

  - **Heap** 堆结构
    - IHeap.hpp
        > 堆接口
    - BinaryHeap.hpp
        > 二叉堆
    - PriorityQueue.hpp
        > 优先队列

  - **Linear** 线性结构
    - **Array** 数组结构
      - DynArray.hpp
        > 动态数组
      - StaArray.hpp
        > 静态数组
    - **LinkedList** 链表结构
      - sNode.hpp
        > 单向链表结点
      - SigList.hpp
        > 单向链表
      - dNode.hpp
        > 双向链表结点
      - DouList.hpp
        > 双向链表
    - DeQue.hpp
        > 双向队列
    - Queue.hpp
        > 队列
    - Stack.hpp
        > 栈

  - **Tree** 树结构
    - AvlNode.hpp
        > 平衡二叉树结点
    - AVL.hpp
        > 平衡二叉树
    - BiTNode.hpp
        > 二叉树结点
    - BST.hpp
        > 二叉搜索树
    - TrbNode.hpp
        > 红黑树结点
    - RBT.hpp
        > 红黑树

- **src** 无实质性文件。

- **CMakeLists.txt** 文件进行子目录的构建

**tests** 测试目录。引入了 **gTest** 进行测试。

## TODO

在往后的编写过程中，应该对该代码进行持续性的维护。对各个结构的实现进行相应的优化。

编写该项目想法的由来，主要是进行数据结构的实现记录，当前项目代码以大致完成了其初始任务。（ **2025.11.07** ）

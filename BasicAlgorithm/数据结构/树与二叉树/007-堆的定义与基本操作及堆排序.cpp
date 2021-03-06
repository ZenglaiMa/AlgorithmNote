/*
    堆的定义与基本操作及堆排序
        1. 堆是一棵完全二叉树, 树中每个结点的值都不小于(或不大于)其左右孩子结点的值. 其中, 如果父亲结点的值大于等于孩子结点的值,
           这样的堆称为大顶堆, 这时每个结点的值都是以它为根结点的子树中的最大值; 反之, 如果父亲结点的值小于等于孩子结点的值, 这样的堆
           称为小顶堆, 这时每个结点的值都是以它为根结点的子树中的最小值. 堆一般用于优先队列的实现, 优先队列中默认使用大顶堆,
           所以在此我们以大顶堆为例, 通过类比即可得出小顶堆的操作.
        2. 因为堆是一棵完全二叉树, 对完全二叉树来说, 一般使用数组来存储: 第一个结点存储在数组的 1 号位, 数组 i 号位表示的结点的左孩子
           就存储在 2i 号位中, 右孩子存储在 (2i+1) 号位中.
*/

#include <algorithm>
using namespace std;

const int maxn = 100;
int heap[maxn], n = 10; // heap表示堆, n为堆中元素的个数

/*
    向下调整: 从最后一个非叶子结点开始, 从右到左, 从下到上进行向下调整.
        总是以当前结点 V  与它的左右孩子(如果存在)的值进行比较, 如果孩子中存在比结点 V 的权值大的, 就将其中权值最大的孩子结点与结点 V
        交换, 交换完毕后继续让结点 V 和其孩子结点进行比较, 直到结点 V 的权值比其左右孩子权值都大或者结点 V 不存在孩子结点为止.
    downAdjust(int low, int high): 对 heap 在 [low, high] 范围内进行向下调整, low 为欲调整结点的下标, high 一般为堆的最后一个结点的下标.
    时间复杂度: O(logn)
*/
void downAdjust(int low, int high) {
    int i = low, j = 2 * i; // i 为欲调整结点下标, 初始 j 为其左孩子的下标
    while(j <= high) { // 如果存在孩子结点
        if(j + 1 <= high && heap[j + 1] > heap[j]) { // 如果存在右孩子且右孩子权值大于左孩子权值
            j = j + 1; // 若满足条件, j 调整为右孩子的下标
        }
        // 经过以上处理, j 为孩子结点中权值最大的孩子结点的下标
        if(heap[j] > heap[i]) { // 若权值最大的孩子结点的权值大于欲调整结点权值
            swap(heap[i], heap[j]); // 进行交换
            i = j; // 保持 i 为欲调整结点下标, j 为 i 的左孩子下标, 进行下一轮调整
            j = 2 * i;
        } else { // 欲调整结点的权值比其孩子结点权值都大, 调整结束
            break;
        }
    }
}

/*
    建堆: 建堆是从初始状态的完全二叉树的最后一个非叶子结点开始, 从下至上, 从右至左进行向下调整. 又因为有n个结点的完全二叉树中叶子结点个数为
        ceil(n / 2), 因此非叶子结点的下标范围为 [1, floor(n / 2)]. 于是可以从 floor(n / 2) 位置倒着枚举结点, 对每个结点 i 进行[i, n]
        范围的调整即可.
    注意: 这里所说的建堆准确来说是"堆化"的过程, 即对初始 heap 数组进行堆化, 使之成为一个堆. 如果说想要通过插入建堆, 就要用到后面的向上调整.
    时间复杂度: O(n)
*/
void createHeap() {
    for(int i = n / 2; i >= 1; i--) {
        downAdjust(i, n);
    }
}

/*
    删除堆顶元素: 对于大顶堆来说即删除堆中最大元素.
        用堆中的最后一个元素覆盖堆顶元素, 然后重新对根结点进行向下调整即可.
    时间复杂度: O(logn)
*/
void deleteTop() {
    heap[1] = heap[n]; // 用最后一个元素覆盖堆顶元素
    n--; // 堆中元素个数减 1
    downAdjust(1, n); // 向下调整堆顶元素
}

/*
    向上调整: 当向已有堆里添加元素或通过插入建堆时, 要用到向上调整.
        向上调整总是把欲调整结点与其父亲结点进行比较, 如果权值比父亲结点权值大, 则交换其与父亲结点, 这样反复比较, 
        直到到达堆顶或者欲调整结点权值小于其父结点权值为止.
    upAdjust(int low, int high): 对 heap 在 [low, high] 范围内进行向上调整, low 一般为 1, high为欲调整结点下标.
    时间复杂度: O(logn)
*/
void upAdjust(int low, int high) {
    int i = high, j = i / 2; // i 为欲调整结点下标, j 为其父结点下标
    while(j >= low) { // 若父结点存在
        if(heap[i] > heap[j]) { // 若欲调整结点的权值大于其父结点权值
            swap(heap[i], heap[j]); // 欲调整结点与其父结点进行交换
            i = j; // 保持 i 为欲调整结点下标, j 为其父结点下标
            j = i / 2;
        } else { // 欲调整结点权值不大于其父结点权值, 调整结束
            break;
        }
    }
}

/*
    向堆中添加元素: 将欲添加元素放在数组最后, 也就是放在堆的最后一个结点后面, 然后进行向上调整即可.
*/
void insert(int x) {
    heap[++n] = x; // 将欲添加元素放在堆的最后一个结点后面
    upAdjust(1, n); // 向上调整
}

/*
    堆排序: 对大顶堆排序结果递增, 对小顶堆排序结果递减, 这里以对序列递增排序为例.
        因为一个大顶堆的堆顶元素永远是堆中权值最大的, 于是堆排序的基本思路就是从堆的最后一个元素开始, 将其与堆顶元素进行交换, 然后
        对堆顶元素进行向下调整即可. 具体实现为逆向遍历 heap, 假设当前访问的是 i 号位, 那么将 i 号元素与堆顶元素交换, 然后对堆顶元素
        进行在 [1, i-1] 范围内的向下调整即可.
    时间复杂度: O(nlogn)
    空间复杂度: O(1)
    稳定性: 不稳定
*/
void heapSort() {
    createHeap(); // 建堆
    for(int i = n; i > 1; i--) {
        swap(heap[i], heap[1]);
        downAdjust(1, i - 1);
    }
}

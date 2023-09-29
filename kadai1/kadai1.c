#include <stdio.h>

#define BUFSIZE 100
#define MAX_NEIGHBORS 10

typedef struct node {
    int id; /* ノード名*/
    int visited; /* 探索された回数 */
    int neighborCount; /* 隣接するノードの数、neighbors 配列の最大引数 */
    struct node * neighbors[MAX_NEIGHBORS]; /* 隣接するノードの配列、ポインターとして記録 */
} node_t;

void printNode(node_t * node) {
    printf("(node %d, visited %d times)\n", node->id, node->visited);
    fflush(stdout);
}

node_t nodes[BUFSIZE]; /* 大域変数（配列） */

/* n ノードを初期化する */
void initNodes(int n) {
    int i;
    for(i = 0; i<n; i++) {
        nodes[i].id = i;
        nodes[i].visited = 0;
        nodes[i].neighborCount = 0;
    }
}

/* origin から destination へ枝を追加 */
void link(node_t * origin, node_t * destination) {
    origin->neighbors[origin->neighborCount] = destination;
    origin->neighborCount++;
}

/* 深さ優先探索 */
void dfs(node_t * node) {
    printNode(node);
    node->visited++;
    if (node->visited == 1) { /* node が初めて出た場合は… */
        int n;
        for (n = 0; n < node->neighborCount; n++) { /* …隣接するノードを再帰的に探索 */
            node_t * next = node->neighbors[n];
            dfs(next);
        }
    }
    /* 初めてじゃ無かった時、あるいは隣接するノード全部探索済みになった時に、呼出側に戻る */
}

void test1(void) {
    /* グラフの定義 */
    initNodes(4);
    link(&nodes[0], &nodes[2]); /* 0 -> 2 */
    link(&nodes[0], &nodes[1]); /* 0 -> 1 */

    link(&nodes[1], &nodes[2]); /* 1 -> 2 */
    link(&nodes[1], &nodes[3]); /* 1 -> 3 */
    
    link(&nodes[2], &nodes[3]); /* 2 -> 3 */
    
    link(&nodes[3], &nodes[0]); /* 3 -> 0 */

    /* 深さ優先探索 */
    dfs(&nodes[0]); /* ノード 0 からスタート */
}

void test2(void) {
    int i;
    initNodes(13); /* ノード 0 を使わない、ノード 1～12 を利用 */
    for(i=3; i<13; i++) {
        link(&nodes[i], &nodes[i-1]); /* i -> i-1 */
        link(&nodes[i], &nodes[i-2]); /* i -> i-2 */
    }
    
    /* 深さ優先探索 */
    dfs(&nodes[12]); /* ノード 0 からスタート */
}

int main(void)  {
    test1();
    /* test2(); */
    return 0;
}

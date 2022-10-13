#include <stdio.h>
#include <assert.h>
#include <math.h>

#define MAX_N 100
#define MAX_T 100

/**********************
 * boardの情報
 ************************/
typedef enum { W, L, B } state_t;
state_t board[MAX_N+1];

void setupBoard(FILE * in, int n, int l, int b) {
    int i;
    for(i=0; i< n+1; i++) {
        board[i]=W;
    }

    for(i=0; i<l; i++) {
        int d;
        fscanf(in, "%d", &d);
        board[d] = L;
    }
    for(i=0; i<b; i++) {
        int d;
        fscanf(in, "%d", &d);
        board[d] = B;
    }
}

/**
 * Debug用、Boardを表示する
 * This procedure prints the board (for debugging purposes)
 */
void printBoard(int n) {
  int i;
  for(i=0; i< n; i++) {
        char tile = '_';
        if (board[i]==L) {
            // Lose one turn
            tile = 'L';
        } else if (board[i]==B) {
            // Back to start
            tile = 'B';
        } else {
            // Normal tile
            tile = '_';
        }
        printf("%d:%c,", i, tile);
  }
  printf("%d:G", n);
}

/* 確率配列 */
/* Probability array */
double prob[MAX_T+2][MAX_N+1];

double solve(int n, int t) {
    /* 確率の配列を全部0.0に初期化*/
    /* Initialized probability array to 0.0 */
    int i,j;
    for(i=0; i<t+1; i++) {
        for (j=0; j<n+1; j++) {
            prob[i][j]=0.0;
        }
    }
    /* 最初のステップに、スタートにいる確率は1.0です */
    /* At turn 0, we are at the start with probability 1.0 */
    prob[0][0] = 1.0;

    /* iはステップ数です*/
    /* i is the step number*/
    for(i=0; i<t; i++) {
        /* jはボードにおいているところ */
        /* j is the placement on the board */
        for(j=0; j<n; j++) {
            /* iのステップに、jのところにいる、次のステップにどの確率でどこにいるかを計算する */
            /* knowing that at step i you are on tile j of the board with probability prob[i][j], 
             * we compute the probabilities of where we will land next */

            /* kはトランプの価値（１～６）*/
            /* k is the dice value*/
            int k;
            for(k=1; k<=6; k++) {
                int nPos = (j+k>n)? 2*n-k-j: j+k; /* ゴールを超えた場合 Check case we go beyong the Goal */
                int nStep = (board[nPos]==L)? i+2: i+1; /* Lの場合は、次の次のステップに動く if L, we move again in 2 turns */
                if(board[nPos]==B) {
                    /* 着くタイルはBの場合は、スタート（０）に戻る*/
                    /* If B, the destination tile is the start (0) */
                    nPos = 0;
                } 

                prob[nStep][nPos] += prob[i][j] / 6.0;
                /* printf("prob[%d][%d]: %lf (+%lf)\n", nStep, nPos, prob[nStep][nPos], prob[i][j]/6.0);　*/
                }
        }
    }

    /* 各ステップにゴールにたどり着く確率を合計する */
    /* Sum the probability to reach the goal at each step */
    double result = 0.0;
    for(i=0; i<=t; i++) {
        result += prob[i][n];
    } 
    return result;
}

/*******
 * こちらで用意したmain 関数。
 * 問題準備してから、solve() をよび、正解比較もおこなう。
 * 
 * This main method parses the input files and calls method solve(), then compares the result against the correct answer 
 */
int main(int argc, char* argv[]) {
    struct {
        char * in, * ans;
    } filepairs[] = { {"sample.in", "sample.ans"},
                      { "C.in", "C.ans"}};

    int i;
    for(i=0; i<2; i++) {
        char* inFile = filepairs[i].in;
        char* ansFile = filepairs[i].ans;
        FILE * in = fopen(inFile, "r");
        FILE * ansIn = fopen(ansFile, "r");
        if(in==NULL) {
            printf("Cannot open file: %s.\n", inFile);
            return 0;
        }
        if(ansIn==NULL) {
            printf("Cannot open file: %s.\n", ansFile);
            return 0;
        }
        int failCount = 0;
        int totalCount = 0;
        printf("Processing input: %s\n", inFile);
        while(1) {
            int n, t, l, b; double result, ans;
            fscanf(in, "%d %d %d %d", &n, &t, &l, &b);
            if(n==0) break;
            setupBoard(in, n, l, b);
            result = solve(n, t);
            fscanf(ansIn, "%lf", &ans);
            double diff = ans-result;
            double diffx = fabs(diff);
            if(diffx > 0.00001) {
                failCount++;
                printf("! You failed problem No. %d (result: %lf, ans: %lf, diff:%lf, %lf)\n", totalCount, result, ans,diff,diffx);
            }
            totalCount++;
        }
        if(failCount==0) {
            printf("Congratulation! You passed all problems (%d) in %s!\n", totalCount, inFile);
        } else {
            printf("Im sorry you missed %d/%d in %s!\n", failCount, totalCount, inFile);
            return 0;
        }
    }
    return 0;
}


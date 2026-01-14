/** A simple console program to test user keypresses against random symbols with
 * Verilog. Author: ChatGPT
 * Date: 2025-12
 * Attention: Change to EN-US input to avoid _getch errors.🚀
 */
#include <conio.h> //不输入回车就响应，因为它不使用缓冲区
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

char PhyKeyBrd[4][22][2] = {{{'`', '~'},
                             {'1', '!'},
                             {'2', '@'},
                             {'3', '#'},
                             {'4', '$'},
                             {'5', '%'},
                             {'6', '^'},
                             {'7', '&'},
                             {'8', '*'},
                             {'9', '('},
                             {'0', ')'},
                             {'-', '_'},
                             {'=', '+'}},

                            {{'q', 'Q'},
                             {'w', 'W'},
                             {'e', 'E'},
                             {'r', 'R'},
                             {'t', 'T'},
                             {'y', 'Y'},
                             {'u', 'U'},
                             {'i', 'I'},
                             {'o', 'O'},
                             {'p', 'P'},
                             {'[', '{'},
                             {']', '}'},
                             {'\\', '|'}},

                            {{'a', 'A'},
                             {'s', 'S'},
                             {'d', 'D'},
                             {'f', 'F'},
                             {'g', 'G'},
                             {'h', 'H'},
                             {'j', 'J'},
                             {'k', 'K'},
                             {'l', 'L'},
                             {';', ':'},
                             {'\'', '"'}},

                            {{'z', 'Z'},
                             {'x', 'X'},
                             {'c', 'C'},
                             {'v', 'V'},
                             {'b', 'B'},
                             {'n', 'N'},
                             {'m', 'M'},
                             {',', '<'},
                             {'.', '>'},
                             {'/', '?'}}};

typedef struct Key {
    char lineId;
    char rowId;
    bool shift;
} Key;

void keyDist(Key *rightKey, Key *wrong, short *lineDist,
             short *rowDist) { // set right,down direction to be axis
    *rowDist = rightKey->lineId - wrong->lineId;
    *lineDist = rightKey->rowId - wrong->rowId;
}

int main() {
    srand(time(NULL)); // 初始化随机数种子
    int cnt = 0, flag = 0;
    char symbols[] = {
        ';', ',', ':', '*', '[', ']', '@', '\'', '<', '&',
        '(', ')', '!', '{', '}', '^', '-', '_',  '+', '$',
        '~', '`', '=', '|', '>', '?', '.', '#',  '\"' /*, '\\', '/', '%'*/};
    int num_symbols = sizeof(symbols) / sizeof(symbols[0]);

    time_t start_time, end_time;
    start_time = time(NULL); // or time(&start);
    while (true) {
        int r = rand() % num_symbols; // 0 到 num_symbols-1
        printf("Press %c", symbols[r]);

    label_:
        int ch = _getch(); // 读取用户输入的第一个字符
        printf("\n");

        if (ch == symbols[r]) {
            cnt++;
            if (cnt == 10) { // 达到10次正确输入
                printf("10 Bingo\n");
                cnt = 0;
                flag++;
            }
            continue;
        } else if (ch == 26) { // Ctrl+D (Unix) or Ctrl+Z (Windows) to exit
            printf("\nEOF detected, exiting...\n");
            break;
        } else if (ch == '\b') { // 退格键
            printf("Backspace detected, ignoring...\n");
            printf("Press %c", symbols[r]);
            goto label_;
        } else { // 输入错误
            Key rightKey, yourKey;
            for (int lineId = 0; lineId < 4; lineId++) {
                for (int colId = 0; colId < 22; colId++) {
                    if (PhyKeyBrd[lineId][colId][0] == symbols[r] ||
                        PhyKeyBrd[lineId][colId][1] == symbols[r]) {
                        rightKey.lineId = lineId;
                        rightKey.rowId = colId;
                    }
                    if (PhyKeyBrd[lineId][colId][0] == ch ||
                        PhyKeyBrd[lineId][colId][1] == ch) {
                        yourKey.lineId = lineId;
                        yourKey.rowId = colId;
                    }
                }
            } // 搜索字符在物理键盘的位置
            short lineDist, rowDist;
            keyDist(&rightKey, &yourKey, &lineDist, &rowDist);
            if (lineDist >= 0 && rowDist >= 0) {
                printf("Expected %c, you pressed %c, right %d, down %d",
                       symbols[r], ch, lineDist, rowDist);
            } else if (lineDist >= 0 && rowDist < 0) {
                printf("Expected %c, you pressed %c, right %d, up %d",
                       symbols[r], ch, lineDist, -rowDist);
            } else if (lineDist < 0 && rowDist >= 0) {
                printf("Expected %c, you pressed %c, left %d, down %d",
                       symbols[r], ch, -lineDist, rowDist);
            } else {
                printf("Expected %c, you pressed %c, left %d, up %d",
                       symbols[r], ch, -lineDist, -rowDist);
            } // 计算距离并输出提示
            cnt = 0;     // 错误则清零
            goto label_; // 跳转到行22，重新输入，必须输入前面的符号
        }
    }
    end_time = time(NULL);
    double diff = difftime(end_time, start_time);
    printf("Total Sec       %.0f\n", diff); // 十进制之后不显示小鼠
    printf("Total Score     %d\n", flag);
    printf("SecPerPress     %.2f\n", diff / (10 * flag)); // 显示两位小数
    return 0;
}
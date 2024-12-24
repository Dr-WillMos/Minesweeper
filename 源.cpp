#include <easyx.h>  // EasyX图形库，安装方式请见官网文档
#include <stdio.h>
#include <time.h>       //计时库
#include <windows.h> //windows的库，用于获取时间戳

#define MAX_ROW 20
#define MAX_COL 20  //以最大雷区作为常量
#define IMG_SIZE 40     // 这个数字用于实现从二维数组到游戏窗口的坐标转化

IMAGE img[13];           // 以数组的方式引入图片
int mine[MAX_ROW + 2][MAX_COL + 2];  //+2是为了解决边缘问题
int num = 0;
int mx;
int my;
int ROW = 16, COL = 16, MINE_NUM = 40;

//定义时间常量
time_t start_time;
time_t current_time;
int elapsed_time;
bool isFirstClickDone = false;          //一个布尔值，用来判定是否触发第一次保护机制

void gameInit() {
    // 初始化图形界面
    initgraph(IMG_SIZE * ROW + 240, IMG_SIZE * COL);  //创建游戏窗口
    int mid1 = (IMG_SIZE * ROW + 240) / 2;
    int mid2 = (IMG_SIZE * COL) / 2;
    int y_length = IMG_SIZE * COL;
    int x_length = IMG_SIZE * ROW + 240;

    settextstyle(30, 0, _T("宋体"));         //这里是以字符串的左上角作为基准
    settextcolor(WHITE);
    const char* text = "开始";

    // 获取字符串的宽度和高度
    int textWidth = textwidth(text);
    int textHeight = textheight(text);

    // 计算字符串的中央位置
    int centerX = x_length / 2 - textWidth / 2;
    int centerY = y_length / 2 - textHeight / 2;


    cleardevice();

    outtextxy(centerX, centerY, text);


    while (1) {
        ExMessage em;
        if (peekmessage(&em, EX_MOUSE)) {
            if (em.message == WM_LBUTTONDOWN) {
                if (em.x >= centerX && em.x <= centerX + textWidth &&
                    em.y >= centerY && em.y <= centerY + textHeight) {     // 点击开始（处于坐标范围内），否则用户永远卡在该循环内。
                    break;
                }
            }
        }
    }
    // 初始化计时器
    start_time = time(NULL);
    current_time = start_time;
    elapsed_time = 0;

    // 设置字体和颜色
    settextstyle(20, 0, _T("宋体"));
    settextcolor(RGB(255, 255, 255));
    outtextxy(IMG_SIZE * ROW + 20, 20, _T("Time: "));

    // 加载图片
    char buf[260] = "";      //用于存储图片路径
    for (int i = 0; i < 12; ++i) {
        memset(buf, 0, sizeof(buf));//每次循环前，使用 memset 函数将 buf 数组清零
        sprintf_s(buf, "./img/%d.jpg", i);  //将路径格式化
        loadimage(&img[i], buf, IMG_SIZE, IMG_SIZE);
    }
    //sprintf(buf, "./img/12.jpg");
   // loadimage(&img[12], buf, 240, IMG_SIZE * COL );        //加载背景,先注释掉看看要不要加上

                // 生成随机数种子，防止每次游戏启动时相同的随机数
    LARGE_INTEGER frequency;
    LARGE_INTEGER start;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);
    srand((unsigned int)(start.QuadPart));

    // 生成雷区
    int row, col;
    memset(mine, 0, sizeof(mine));  // 清空雷区

    for (int i = 0; i < MINE_NUM;) {  //随机生成雷
        row = rand() % ROW + 1;
        col = rand() % COL + 1;        //rand函数，1-9之间随机生成值
        if (mine[row][col] == 0) {
            mine[row][col] = 9;      //9表示地雷
            ++i;
        }
    }

    // 更新周围数字（九宫格内雷的数量）
    for (int i = 1; i < ROW; ++i) {
        for (int j = 1; j < COL; ++j) {
            if (mine[i][j] == 9) {
                for (int a = i - 1; a <= i + 1; ++a) {
                    for (int b = j - 1; b <= j + 1; ++b) {
                        if (mine[a][b] != 9) {
                            mine[a][b]++;
                        }
                    }
                }
            }
        }
    }

    // 给所有格子加上偏移量，防止与被翻开格子冲突，也是一种加密数据。
    for (int i = 1; i < ROW + 1; ++i) {
        for (int j = 1; j < COL + 1; ++j) {
            mine[i][j] += 20;  //只有显示为29的才是雷，在20-28之间的都是非雷格子，点击翻开格子和递归翻开格子的操作就是-20
        }
    }

}

void showMenu() {
    initgraph(400, 300);  // 创建菜单窗口
    setbkcolor(WHITE);
    cleardevice();

    settextstyle(20, 0, _T("宋体"));
    settextcolor(BLACK);
    outtextxy(150, 50, _T("选择游戏难度："));
    outtextxy(150, 100, _T("1. 简单"));
    outtextxy(150, 140, _T("2. 中等"));
    outtextxy(150, 180, _T("3. 困难"));

    while (1) {
        ExMessage em;
        if (peekmessage(&em, EX_MOUSE)) {
            if (em.message == WM_LBUTTONDOWN) {
                if (em.x > 150 && em.x < 250 && em.y > 100 && em.y < 120) {     // 点击简单（处于坐标范围内）
                    ROW = 9;
                    COL = 9;
                    MINE_NUM = 10;
                    break;
                }
                else if (em.x > 150 && em.x < 250 && em.y > 140 && em.y < 160) {   // 点击中等
                    ROW = 16;
                    COL = 16;
                    MINE_NUM = 40;
                    break;
                }
                else if (em.x > 150 && em.x < 250 && em.y > 180 && em.y < 200) {    // 点击困难
                    ROW = 20;
                    COL = 20;
                    MINE_NUM = 70;
                    break;
                }
            }
        }
    }
    closegraph();  // 关闭菜单窗口
}

void restartGame() {
    isFirstClickDone = false;  // 重置第一次点击状态保证第一次保护机制。
    // 重新初始化随机数种子
    LARGE_INTEGER frequency;
    LARGE_INTEGER start;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);
    srand((unsigned int)(start.QuadPart));
    num = 0;
    start_time = time(NULL);        // 重置游戏开始时间
    elapsed_time = 0;                    // 重置计时器
    showMenu();
    gameInit();
}



void isOver() {
    // 检查是否暴雷
    bool isHitMine = false;  //布尔型变量，只有0和1两种形式（即true和flase）
    for (int i = 1; i < ROW + 1; ++i) {
        for (int j = 1; j < COL + 1; ++j) {
            if (mine[i][j] == 9 && (mine[i][j] < 20 || mine[i][j] > 29)) {  // 雷被翻开
                isHitMine = true;
                break;
            }
        }
        if (isHitMine) break;
    }

    // 如果踩雷
    if (isHitMine) {
        int ok = MessageBox(GetHWnd(), "还要排雷吗", "你失败了", MB_OKCANCEL);  //easyx的函数，弹出消息盒子
        if (ok == IDOK) {
            // 重置雷区
            for (int i = 1; i < ROW + 1; ++i) {
                for (int j = 1; j < COL + 1; ++j) {
                    if (mine[i][j] == 9) {
                        mine[i][j] += 20;         // 将雷标记为已翻开
                    }
                }
            }
            restartGame();                         // 重新初始化雷区和图像
        }
        else {
            exit(-1);  // 退出游戏
        }
    }

    // 如果非雷全部被翻开则胜利
    int uncoveredNonMine = 0;            //翻开的非雷格子数量
    for (int i = 1; i < ROW + 1; ++i) {
        for (int j = 1; j < COL + 1; ++j) {
            if (mine[i][j] < 20 && mine[i][j] != 9) {          // 非雷且已翻开
                uncoveredNonMine++;
            }
        }
    }

    // 游戏胜利条件是所有非雷格子都被翻开
    if (uncoveredNonMine == ROW * COL - MINE_NUM) {
        // 计算已用时间
        elapsed_time = (int)(time(NULL) - start_time);

        // 格式化胜利提示信息，显示耗时
        char win_message[100];         //字符型数组
        sprintf_s(win_message, "你赢了！\n耗时：%d秒", elapsed_time);    //输出已经消耗的时间

        // 弹出胜利消息盒子，
        int ok = MessageBox(GetHWnd(), win_message, "恭喜你！", MB_OKCANCEL);
        if (ok == IDOK) {
            restartGame();          // 重新初始化游戏
        }
        else {
            exit(0);  // 退出游戏
        }
    }
}


void gameDraw() {

    for (int i = 1; i < ROW + 1; ++i) {
        for (int j = 1; j < COL + 1; ++j) {
            int x = (j - 1) * IMG_SIZE;
            int y = (i - 1) * IMG_SIZE;
            if (mine[i][j] >= 0 && mine[i][j] <= 9) {
                putimage(x, y, &img[mine[i][j]]);
            }
            else if (mine[i][j] >= 20 && mine[i][j] < 30) {
                putimage(x, y, &img[10]);
            }
            else if (mine[i][j] > 29) {
                putimage(x, y, &img[11]);
            }
        }
    }
    // 显示计时器
    char time_str[20];
    sprintf_s(time_str, "%d s", elapsed_time);
    settextcolor(RGB(255, 255, 255));
    outtextxy(IMG_SIZE * ROW + 80, 20, _T("     "));
    settextcolor(RGB(0, 0, 0));
    outtextxy(IMG_SIZE * ROW + 110, 20, time_str);
}

void openNUll(int r, int c) {   //递归实现格子展开
    if (mine[r][c] == 0) {
        for (int i = r - 1; i <= r + 1; ++i) {
            for (int j = c - 1; j <= c + 1; ++j) {
                if ((mine[i][j] == 20 || mine[i][j] != 29) && mine[i][j] > 9) {
                    mine[i][j] -= 20;
                    num++;
                    openNUll(i, j);
                }
            }
        }
    }
}

void mouseClick() {
    ExMessage em;
    if (peekmessage(&em, EX_MOUSE)) {
        mx = em.y / IMG_SIZE + 1;
        my = em.x / IMG_SIZE + 1;

        if (em.lbutton) {
            if (!isFirstClickDone) {  // 只有当还未进行第一次点击时，执行第一次点击相关的雷区初始化保护逻辑
                isFirstClickDone = true;  // 标记第一次点击已完成

                // 生成雷区并保护第一次点击及其周围，不能在周围生成雷
                int protectedRow = mx;
                int protectedCol = my;

                // 随机生成雷区
                int row, col;
                memset(mine, 0, sizeof(mine));
                for (int i = 0; i < MINE_NUM;) {
                    row = rand() % ROW + 1;
                    col = rand() % COL + 1;

                    // 跳过第一次点击区域及其周围九宫格
                    if (row >= protectedRow - 1 && row <= protectedRow + 1 &&
                        col >= protectedCol - 1 && col <= protectedCol + 1) {
                        continue;
                    }

                    if (mine[row][col] == 0) {
                        mine[row][col] = 9;
                        ++i;
                    }
                }

                // 更新周围数字
                for (int i = 1; i < ROW + 1; ++i) {
                    for (int j = 1; j < COL + 1; ++j) {
                        if (mine[i][j] == 9) {
                            for (int a = i - 1; a <= i + 1; ++a) {
                                for (int b = j - 1; b <= j + 1; ++b) {
                                    if (mine[a][b] != 9) {
                                        mine[a][b]++;
                                    }
                                }
                            }
                        }
                    }
                }

                // 加密雷区
                for (int i = 1; i < ROW + 1; ++i) {
                    for (int j = 1; j < COL + 1; ++j) {
                        mine[i][j] += 20;
                    }
                }
            }
            else {
                // 非第一次点击时，正常展开点击的格子等操作
                if (mine[mx][my] > 9) {
                    mine[mx][my] -= 20;
                    openNUll(mx, my);
                    num++;
                }
            }
        }
        else if (em.rbutton) {  // 鼠标右键标记
            if (mine[mx][my] > 9 && mine[mx][my] <= 29) {
                mine[mx][my] += 20;
            }
            else {
                mine[mx][my] -= 20;
            }
        }
    }
}



void tips() {                //棋盘右方提示函数
    current_time = time(NULL);
    elapsed_time = (int)(current_time - start_time);

    char time_str[20];
    sprintf_s(time_str, "%d s", elapsed_time);

    settextcolor(RGB(255, 255, 255));         //设置时间字体颜色。
    outtextxy(IMG_SIZE * ROW + 85, 20, time_str);                 //easyx的输出
    outtextxy(IMG_SIZE * ROW + 15, 50, _T("鼠标左键：翻开格子"));
    outtextxy(IMG_SIZE * ROW + 15, 80, _T("鼠标右键：标记地雷"));
    outtextxy(IMG_SIZE * ROW + 15, 110, _T("所有的非雷格子都被打开"));
    outtextxy(IMG_SIZE * ROW + 15, 140, _T("才算游戏胜利"));
    outtextxy(IMG_SIZE * ROW + 15, 170, _T("如果是第一次点击"));
    outtextxy(IMG_SIZE * ROW + 15, 200, _T("需要双击同一格子中心"));
    outtextxy(IMG_SIZE * ROW + 15, 230, _T("祝你游戏愉快"));
}


int main() {
    showMenu();
    gameInit();
    while (true) {          //不断循环保证数据随时更新
        tips();
        mouseClick();
        gameDraw();
        isOver();
    }

    return 0;
}

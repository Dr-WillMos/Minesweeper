#include <easyx.h>  // EasyXͼ�ο⣬��װ��ʽ��������ĵ�
#include <stdio.h>
#include <time.h>       //��ʱ��
#include <windows.h> //windows�Ŀ⣬���ڻ�ȡʱ���

#define MAX_ROW 20
#define MAX_COL 20  //�����������Ϊ����
#define IMG_SIZE 40     // �����������ʵ�ִӶ�ά���鵽��Ϸ���ڵ�����ת��

IMAGE img[13];           // ������ķ�ʽ����ͼƬ
int mine[MAX_ROW + 2][MAX_COL + 2];  //+2��Ϊ�˽����Ե����
int num = 0;
int mx;
int my;
int ROW = 16, COL = 16, MINE_NUM = 40;

//����ʱ�䳣��
time_t start_time;
time_t current_time;
int elapsed_time;
bool isFirstClickDone = false;          //һ������ֵ�������ж��Ƿ񴥷���һ�α�������

void gameInit() {
    // ��ʼ��ͼ�ν���
    initgraph(IMG_SIZE * ROW + 240, IMG_SIZE * COL);  //������Ϸ����
    int mid1 = (IMG_SIZE * ROW + 240) / 2;
    int mid2 = (IMG_SIZE * COL) / 2;
    int y_length = IMG_SIZE * COL;
    int x_length = IMG_SIZE * ROW + 240;

    settextstyle(30, 0, _T("����"));         //���������ַ��������Ͻ���Ϊ��׼
    settextcolor(WHITE);
    const char* text = "��ʼ";

    // ��ȡ�ַ����Ŀ�Ⱥ͸߶�
    int textWidth = textwidth(text);
    int textHeight = textheight(text);

    // �����ַ���������λ��
    int centerX = x_length / 2 - textWidth / 2;
    int centerY = y_length / 2 - textHeight / 2;


    cleardevice();

    outtextxy(centerX, centerY, text);


    while (1) {
        ExMessage em;
        if (peekmessage(&em, EX_MOUSE)) {
            if (em.message == WM_LBUTTONDOWN) {
                if (em.x >= centerX && em.x <= centerX + textWidth &&
                    em.y >= centerY && em.y <= centerY + textHeight) {     // �����ʼ���������귶Χ�ڣ��������û���Զ���ڸ�ѭ���ڡ�
                    break;
                }
            }
        }
    }
    // ��ʼ����ʱ��
    start_time = time(NULL);
    current_time = start_time;
    elapsed_time = 0;

    // �����������ɫ
    settextstyle(20, 0, _T("����"));
    settextcolor(RGB(255, 255, 255));
    outtextxy(IMG_SIZE * ROW + 20, 20, _T("Time: "));

    // ����ͼƬ
    char buf[260] = "";      //���ڴ洢ͼƬ·��
    for (int i = 0; i < 12; ++i) {
        memset(buf, 0, sizeof(buf));//ÿ��ѭ��ǰ��ʹ�� memset ������ buf ��������
        sprintf_s(buf, "./img/%d.jpg", i);  //��·����ʽ��
        loadimage(&img[i], buf, IMG_SIZE, IMG_SIZE);
    }
    //sprintf(buf, "./img/12.jpg");
   // loadimage(&img[12], buf, 240, IMG_SIZE * COL );        //���ر���,��ע�͵�����Ҫ��Ҫ����

                // ������������ӣ���ֹÿ����Ϸ����ʱ��ͬ�������
    LARGE_INTEGER frequency;
    LARGE_INTEGER start;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);
    srand((unsigned int)(start.QuadPart));

    // ��������
    int row, col;
    memset(mine, 0, sizeof(mine));  // �������

    for (int i = 0; i < MINE_NUM;) {  //���������
        row = rand() % ROW + 1;
        col = rand() % COL + 1;        //rand������1-9֮���������ֵ
        if (mine[row][col] == 0) {
            mine[row][col] = 9;      //9��ʾ����
            ++i;
        }
    }

    // ������Χ���֣��Ź������׵�������
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

    // �����и��Ӽ���ƫ��������ֹ�뱻�������ӳ�ͻ��Ҳ��һ�ּ������ݡ�
    for (int i = 1; i < ROW + 1; ++i) {
        for (int j = 1; j < COL + 1; ++j) {
            mine[i][j] += 20;  //ֻ����ʾΪ29�Ĳ����ף���20-28֮��Ķ��Ƿ��׸��ӣ�����������Ӻ͵ݹ鷭�����ӵĲ�������-20
        }
    }

}

void showMenu() {
    initgraph(400, 300);  // �����˵�����
    setbkcolor(WHITE);
    cleardevice();

    settextstyle(20, 0, _T("����"));
    settextcolor(BLACK);
    outtextxy(150, 50, _T("ѡ����Ϸ�Ѷȣ�"));
    outtextxy(150, 100, _T("1. ��"));
    outtextxy(150, 140, _T("2. �е�"));
    outtextxy(150, 180, _T("3. ����"));

    while (1) {
        ExMessage em;
        if (peekmessage(&em, EX_MOUSE)) {
            if (em.message == WM_LBUTTONDOWN) {
                if (em.x > 150 && em.x < 250 && em.y > 100 && em.y < 120) {     // ����򵥣��������귶Χ�ڣ�
                    ROW = 9;
                    COL = 9;
                    MINE_NUM = 10;
                    break;
                }
                else if (em.x > 150 && em.x < 250 && em.y > 140 && em.y < 160) {   // ����е�
                    ROW = 16;
                    COL = 16;
                    MINE_NUM = 40;
                    break;
                }
                else if (em.x > 150 && em.x < 250 && em.y > 180 && em.y < 200) {    // �������
                    ROW = 20;
                    COL = 20;
                    MINE_NUM = 70;
                    break;
                }
            }
        }
    }
    closegraph();  // �رղ˵�����
}

void restartGame() {
    isFirstClickDone = false;  // ���õ�һ�ε��״̬��֤��һ�α������ơ�
    // ���³�ʼ�����������
    LARGE_INTEGER frequency;
    LARGE_INTEGER start;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);
    srand((unsigned int)(start.QuadPart));
    num = 0;
    start_time = time(NULL);        // ������Ϸ��ʼʱ��
    elapsed_time = 0;                    // ���ü�ʱ��
    showMenu();
    gameInit();
}



void isOver() {
    // ����Ƿ���
    bool isHitMine = false;  //�����ͱ�����ֻ��0��1������ʽ����true��flase��
    for (int i = 1; i < ROW + 1; ++i) {
        for (int j = 1; j < COL + 1; ++j) {
            if (mine[i][j] == 9 && (mine[i][j] < 20 || mine[i][j] > 29)) {  // �ױ�����
                isHitMine = true;
                break;
            }
        }
        if (isHitMine) break;
    }

    // �������
    if (isHitMine) {
        int ok = MessageBox(GetHWnd(), "��Ҫ������", "��ʧ����", MB_OKCANCEL);  //easyx�ĺ�����������Ϣ����
        if (ok == IDOK) {
            // ��������
            for (int i = 1; i < ROW + 1; ++i) {
                for (int j = 1; j < COL + 1; ++j) {
                    if (mine[i][j] == 9) {
                        mine[i][j] += 20;         // ���ױ��Ϊ�ѷ���
                    }
                }
            }
            restartGame();                         // ���³�ʼ��������ͼ��
        }
        else {
            exit(-1);  // �˳���Ϸ
        }
    }

    // �������ȫ����������ʤ��
    int uncoveredNonMine = 0;            //�����ķ��׸�������
    for (int i = 1; i < ROW + 1; ++i) {
        for (int j = 1; j < COL + 1; ++j) {
            if (mine[i][j] < 20 && mine[i][j] != 9) {          // �������ѷ���
                uncoveredNonMine++;
            }
        }
    }

    // ��Ϸʤ�����������з��׸��Ӷ�������
    if (uncoveredNonMine == ROW * COL - MINE_NUM) {
        // ��������ʱ��
        elapsed_time = (int)(time(NULL) - start_time);

        // ��ʽ��ʤ����ʾ��Ϣ����ʾ��ʱ
        char win_message[100];         //�ַ�������
        sprintf_s(win_message, "��Ӯ�ˣ�\n��ʱ��%d��", elapsed_time);    //����Ѿ����ĵ�ʱ��

        // ����ʤ����Ϣ���ӣ�
        int ok = MessageBox(GetHWnd(), win_message, "��ϲ�㣡", MB_OKCANCEL);
        if (ok == IDOK) {
            restartGame();          // ���³�ʼ����Ϸ
        }
        else {
            exit(0);  // �˳���Ϸ
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
    // ��ʾ��ʱ��
    char time_str[20];
    sprintf_s(time_str, "%d s", elapsed_time);
    settextcolor(RGB(255, 255, 255));
    outtextxy(IMG_SIZE * ROW + 80, 20, _T("     "));
    settextcolor(RGB(0, 0, 0));
    outtextxy(IMG_SIZE * ROW + 110, 20, time_str);
}

void openNUll(int r, int c) {   //�ݹ�ʵ�ָ���չ��
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
            if (!isFirstClickDone) {  // ֻ�е���δ���е�һ�ε��ʱ��ִ�е�һ�ε����ص�������ʼ�������߼�
                isFirstClickDone = true;  // ��ǵ�һ�ε�������

                // ����������������һ�ε��������Χ����������Χ������
                int protectedRow = mx;
                int protectedCol = my;

                // �����������
                int row, col;
                memset(mine, 0, sizeof(mine));
                for (int i = 0; i < MINE_NUM;) {
                    row = rand() % ROW + 1;
                    col = rand() % COL + 1;

                    // ������һ�ε����������Χ�Ź���
                    if (row >= protectedRow - 1 && row <= protectedRow + 1 &&
                        col >= protectedCol - 1 && col <= protectedCol + 1) {
                        continue;
                    }

                    if (mine[row][col] == 0) {
                        mine[row][col] = 9;
                        ++i;
                    }
                }

                // ������Χ����
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

                // ��������
                for (int i = 1; i < ROW + 1; ++i) {
                    for (int j = 1; j < COL + 1; ++j) {
                        mine[i][j] += 20;
                    }
                }
            }
            else {
                // �ǵ�һ�ε��ʱ������չ������ĸ��ӵȲ���
                if (mine[mx][my] > 9) {
                    mine[mx][my] -= 20;
                    openNUll(mx, my);
                    num++;
                }
            }
        }
        else if (em.rbutton) {  // ����Ҽ����
            if (mine[mx][my] > 9 && mine[mx][my] <= 29) {
                mine[mx][my] += 20;
            }
            else {
                mine[mx][my] -= 20;
            }
        }
    }
}



void tips() {                //�����ҷ���ʾ����
    current_time = time(NULL);
    elapsed_time = (int)(current_time - start_time);

    char time_str[20];
    sprintf_s(time_str, "%d s", elapsed_time);

    settextcolor(RGB(255, 255, 255));         //����ʱ��������ɫ��
    outtextxy(IMG_SIZE * ROW + 85, 20, time_str);                 //easyx�����
    outtextxy(IMG_SIZE * ROW + 15, 50, _T("����������������"));
    outtextxy(IMG_SIZE * ROW + 15, 80, _T("����Ҽ�����ǵ���"));
    outtextxy(IMG_SIZE * ROW + 15, 110, _T("���еķ��׸��Ӷ�����"));
    outtextxy(IMG_SIZE * ROW + 15, 140, _T("������Ϸʤ��"));
    outtextxy(IMG_SIZE * ROW + 15, 170, _T("ף����Ϸ���"));
}


int main() {
    showMenu();
    gameInit();
    while (true) {          //����ѭ����֤������ʱ����
        tips();
        mouseClick();
        gameDraw();
        isOver();
    }

    return 0;
}

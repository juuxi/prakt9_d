#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <termios.h>
#include <time.h>

static struct termios oldt;

void restore_terminal_settings(void)
{
tcsetattr(0, TCSANOW, &oldt); /* Apply saved settings */
}

void disable_waiting_for_enter(void)
{
struct termios newt;

/* Make terminal read 1 char at a time */
tcgetattr(0, &oldt); /* Save terminal settings */
newt = oldt; /* Init new settings */
newt.c_lflag &= ~(ICANON | ECHO); /* Change settings */
tcsetattr(0, TCSANOW, &newt); /* Apply settings */
atexit(restore_terminal_settings); /* Make sure settings will be restored when program ends */
}

void clean_stdin(void) //очистка потока ввода, применяется там где без нее scanf начнет читать всякие необработанные символы
{
int c;
do {
c = getchar();
} while (c != '\n' && c != EOF);
}

int symbol_scanning(int overfmax) //эта функция занимается посимвольной читкой введенного числа
{
    bool inputfailure = false, count = false, r = false, overf = false;
    int num = 0, counter = 0; //отобрать возможность сразу нажать "энтр"
    while(true){
                overf = false;
                inputfailure = false;
                char c = '\0';
                scanf("%c", &c);
                if (c < '0' || c > '9')   //если прочитанный символ - не цифра, лезем проверять что же это              
                {
                    if (c != '\n') {printf("Вы ввели неверный символ, повторите попытку\n"); clean_stdin();} // если это не перенос строки - кидаем ошибку
                    else if(counter != 0) {count = true; break;} //если это перенос строки не первым же символом - все круто, прерываем цикл
                    else {printf("Вы ввели неверный символ, повторите попытку\n");} //если первым - выдаем ошибку
                }
                else{
                if (num*10+c-'0' > overfmax) {overf = true; num = 0; clean_stdin(); r = true;} //если дописывание полученной цифры к имеющемуся (см. ниже)
                else r = false;  //числу не выйдет за установленные нами рамки (число, которое мы указали при вызове функции) - записываем его (см. ниже) 
                if(r == false)   //если же выйдет - выходим из цикла с ошибкой overf
                num = num*10+c-'0';
                }
                if(overf == true) printf("Число превысило максимальный размер (%d). Повторите попытку\n", overfmax);
                counter++;
    }
    return num;
}

struct st
{
    int holex;
    int holey;
    bool vyxod_est;
};

struct st creating(int pitch[20][20], int n, int m, int codesymb, bool fenceerr)
{
    struct st s1;
    int holex, holey;
    bool err = false, foundpl = false, endd = false; 
    if(codesymb == 10) s1.vyxod_est = false;
    if(n % 2 != 0) holey = n/2+1; else (holey = n/2);
    if(m % 2 != 0) holex = m/2+1; else (holex = m/2);
    if(pitch[holey][holex] != 0)
    {
        for(int d = 0; d < n; d++)
        {
            for(int s = 0; s < m; s++)
            {
                if(pitch[d][s] == 0) {holey = d; holex = s; foundpl = true;}
            }
        }
        if(foundpl == false) {printf("Место закончилось");}
    }
    system("clear");     
    printf("\n\n"); 
    while(true)
    {
        pitch[holey][holex] = codesymb;  
        for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < m; j++)
        {
            switch(pitch[i][j])
            {
            case 0:  printf("%s", "\U0001f9f1"); break;   
            case 10: printf("%s", "\U0001f300"); break;     
            case 20: printf("%s", "\U0001f6a7"); break;
            case 35: printf("%s", "\U0001f600"); break;
            case 70: printf("%s", "\U0001f9f1"); break;
            default: printf("%s", "\U0001f300");
            } 
        }
        printf("\n");
    }
    if(err == true) printf("Введен неверный символ\n");
    if(codesymb == 10)
    {
        printf("Эта часть программы создана для удаления границ. Перемещайте символ %s клавишами w, a, s, d\n", "\U0001f300");
        printf("Чтобы удалить стенку займите ее место и нажмите клавишу p, для перехода к установке стенок - Q\n");
    }
    if(codesymb == 20) 
    {
        printf("Установите стенки. Для передвижения используйте w, a, s, d\n");
        printf("Для устновки стенки нажмите клавишу p, для перехода к установке смайлика - Q\n");
    }
    if(codesymb == 35) 
    {
        printf("Установите положение смайлика. Для передвижения используйте w, a, s, d\n");
        printf("Для установки смайлика нажмите p или Q\n");
    }
    char getbut;
    disable_waiting_for_enter();
    getbut = getchar();
    restore_terminal_settings();
    bool fin = false, rdy = false;
    system("clear");      
    if((holex == 0 || holex == m-1 || holey == 0 || holey == n-1) && pitch[holey][holex] != 70) pitch[holey][holex] = 20;
    if(fenceerr == 0) pitch[holey][holex] = 0;
    else pitch[holey][holex] = 20;
    fenceerr = 0;
    switch(getbut) 
    {
        case 119: //w
         if(codesymb == 10) {if(holey == 0) printf("Невозможно смещение в указанном направлении\n"); else holey--; break;}
         if(holey == 1) printf("Невозможно смещение в указанном направлении\n"); else holey--; 
         if(pitch[holey][holex] == 20) fenceerr = true;  break; 
        case 97:  //a
         if(codesymb == 10) {if(holex == 0) printf("Невозможно смещение в указанном направлении\n"); else holex--; break;}
         if(holex == 1) printf("Невозможно смещение в указанном направлении\n"); else holex--; 
         if(pitch[holey][holex] == 20) fenceerr = true; break; 
        case 115: //s
         if(codesymb == 10) {if(holey == n-1) printf("Невозможно смещение в указанном направлении\n"); else holey++; break;} 
         if(holey == n) printf("Невозможно смещение в указанном направлении\n"); else holey++; 
         if(pitch[holey][holex] == 20) fenceerr = true; break; 
        case 100: //d
         if(codesymb == 10) {if(holex == m-1) printf("Невозможно смещение в указанном направлении\n"); else holex++; break;}
         if(holex == m) printf("Невозможно смещение в указанном направлении\n"); else holex++; 
         if(pitch[holey][holex] == 20) fenceerr = true; break; 
        case 112: //p
        if(codesymb != 35) pitch[holey][holex] = codesymb; 
        if(codesymb == 10) {pitch[holey][holex] = 70; s1.vyxod_est = true;}
        if(codesymb == 35) {if(pitch[holey][holex] != 20) {endd = true; s1.holex = holex; s1.holey = holey; return s1;}
        else printf("Невозможно установка смайлика на этом месте\n"); break;}
        if(holex != m-2 && pitch[holey][holex+1] != 20 && pitch[holey][holex+1] != 35 && codesymb != 10) 
        {
            holex++;    
        }  
        else {
            for(int d = 1; d < n; d++)
        {
            if(rdy == true) break;
            for(int s = 1; s < m; s++)
            {
                if(rdy == true) break;
                if(pitch[d][s] == 0) {
                    holey = d; holex = s; foundpl = true; rdy = true;
                }
            }
        }
        if(foundpl == false) {printf("Место закончилось");}
        }
        break; 
        case 81: endd = true; if(codesymb == 35) {s1.holex = holex; s1.holey = holey; return s1;} break;
        default: printf("Введен неверный символ\n\n"); err = true;
    }
    if(endd == true) break;
    if(err == false) printf("\n");
    }
    return s1;
}

void moving_checker_sub(int pitch[20][20], int escapes[4], bool escapeses[4], int holex, int holey, int a, 
int mx, int my, int escapes_sub[4], int counter, int endd[4], int smallest, int good_escape[200][2], int n, int m)
{
    if(counter == 0) escapes_sub[a] = 200;
    if(escapes_sub[a] > escapes[a] || escapes_sub[a] > smallest) return;
    bool escapeses_sub[4] = {1, 1, 1, 1};
    holex += mx; holey += my;
    if(holey == 0 || holex == 0 || holey == n-1 || holex == m-1)
    return;
    if(mx != -1)
    {
        if(escapes_sub[a] > escapes[a] || escapes_sub[a] > smallest) return;
        if(pitch[holey][holex+1] == 70) {escapes_sub[a]++; if(escapes_sub[a] < escapes[a]) escapes[a] = escapes_sub[a]; endd[a] = 1; 
        escapes_sub[a]--; return;}
    else
    {
        if(pitch[holey][holex+1] == 0) 
        {
            if(escapes_sub[a] == 200) escapes_sub[a] = 1;
            escapes_sub[a]++;
            moving_checker_sub(pitch, escapes, escapeses, holex, holey, a, 1, 0, escapes_sub, counter+1, endd, smallest, good_escape, n, m);            
        } 
    }
    }

    if(mx != 1)
    {
        if(escapes_sub[a] > escapes[a] || escapes_sub[a] > smallest) return;
    if(pitch[holey][holex-1] == 70) {escapes_sub[a]++; if(escapes_sub[a] < escapes[a]) escapes[a] = escapes_sub[a]; endd[a] = 1; 
    escapes_sub[a]--; return;}
    else
    {
        if(pitch[holey][holex-1] == 0) 
        {
            if(escapes_sub[a] == 200) escapes_sub[a] = 1;
            escapes_sub[a]++;
            moving_checker_sub(pitch, escapes, escapeses, holex, holey, a, -1, 0, escapes_sub, counter+1, endd, smallest, good_escape, n, m);            
        } 
    }
    }

    if(my != -1)
    {
        if(escapes_sub[a] > escapes[a] || escapes_sub[a] > smallest) return;
    if(pitch[holey+1][holex] == 70) {escapes_sub[a]++; if(escapes_sub[a] < escapes[a]) escapes[a] = escapes_sub[a]; endd[a] = 1; 
    escapes_sub[a]--; return;}
    else
    {
        if(pitch[holey+1][holex] == 0) 
        {
            if(escapes_sub[a] == 200) escapes_sub[a] = 1;
            escapes_sub[a]++;
            moving_checker_sub(pitch, escapes, escapeses, holex, holey, a, 0, 1, escapes_sub, counter+1, endd, smallest, good_escape, n, m);
        } 
    }
    }

    if(my != 1)
    {
        if(escapes_sub[a] > escapes[a] || escapes_sub[a] > smallest) return;
    if(pitch[holey-1][holex] == 70) {escapes_sub[a]++; if(escapes_sub[a] < escapes[a]) escapes[a] = escapes_sub[a]; endd[a] = 1;
    escapes_sub[a]--; return;}
    else
    {
        if(pitch[holey-1][holex] == 0) 
        {
            if(escapes_sub[a] == 200) escapes_sub[a] = 1;
            escapes_sub[a]++;
            moving_checker_sub(pitch, escapes, escapeses, holex, holey, a, 0, -1, escapes_sub, counter+1, endd, smallest, good_escape, n, m);
        } 
    }
    }
}

void moving_checker_main(int pitch[20][20], int escapes[4], bool escapeses[4], int holex, int holey, int endd[4])
{
        if(pitch[holey][holex+1] == 70) {escapes[0] = 1; endd[0] = 1; return;}
    else
    {
        if(pitch[holey][holex+1] != 0) escapeses[0] = false;
    }

    if(pitch[holey][holex-1] == 70) {escapes[1] = 1; endd[1] = 1; return;}
    else
    {
        if(pitch[holey][holex-1] != 0) escapeses[1] = false;
    }

    if(pitch[holey+1][holex] == 70) {escapes[2] = 1; endd[2] = 1; return;}
    {
        if(pitch[holey+1][holex] != 0) escapeses[2] = false;       
    }

    if(pitch[holey-1][holex] == 70) {escapes[3] = 1; endd[3] = 1; return;}
    else
    {
        if(pitch[holey-1][holex] != 0) escapeses[3] = false;        
    } 
}


int main()
{
    struct st s1;
    int pitch[20][20], escapes[] = {200, 200, 200, 200}, escapes_sub[4] = {200, 200, 200, 200};
    bool escapeses[] = {1, 1, 1, 1};
    int n = 0, m = 0, holex, holey;
    int endd[4] = {0, 0, 0, 0};
    printf("Введите длину лабиринта (максимум - 20)\n");
    n = symbol_scanning(20);
    printf("Введите ширину лабиринта (максимум - 20)\n");
    m = symbol_scanning(20);
    for (int i = 0; i < n; i++)
    {
        for(int j = 0; j < m; j++)
        {
        if(i == 0 || j == 0 || i == n-1 || j == m-1) pitch[i][j] = 20;
        else pitch[i][j] = 0;
        }
    }
    s1 = creating(pitch, n, m, 10, 0); 
    creating(pitch, n, m, 20, 0); 
    s1 = creating(pitch, n, m, 35, 0);
    holex = s1.holex; holey = s1.holey;
    if(s1.vyxod_est == false)
    {
        for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < m; j++)
        {
            switch(pitch[i][j])
            {
            case 0:  printf("%s", "\U0001f9f1"); break;   
            case 10: printf("%s", "\U0001f300"); break;     
            case 20: printf("%s", "\U0001f6a7"); break;
            case 35: printf("%s", "\U0001f600"); break;
            case 70: printf("%s", "\U0001f9f1"); break;
            default: printf("%s", "\U0001f300");
            } 
        }
        printf("\n");
    }
        printf("Выхода нет\n"); 
        return 0;
    }
    moving_checker_main(pitch, escapes, escapeses, holex, holey, endd);
    int smallest = 200;
    int good_escape[200][2];
    for(int h = 0; h < 200; h++)
    {
        for(int k = 0; k < 2; k++)
        good_escape[h][k] = 0;
    }
    if(endd[0] != 1 && endd[1] != 1 && endd[2] != 1 && endd[3] != 1)
    {
    if(escapeses[0] == true) moving_checker_sub(pitch, escapes, escapeses, holex, holey, 0,  1, 0, escapes_sub, 0, endd, smallest, good_escape, n, m);
    if(endd[0] != 1) escapes[0] = 0; 
    if(escapes[0] > 0 && escapes[0] < smallest) smallest = escapes[0];

    if(escapeses[1] == true) moving_checker_sub(pitch, escapes, escapeses, holex, holey, 1, -1, 0, escapes_sub, 0, endd, smallest, good_escape, n, m);
    if(endd[1] != 1) escapes[1] = 0; 
    if(escapes[1] > 0 && escapes[1] < smallest) smallest = escapes[1];

    if(escapeses[2] == true) moving_checker_sub(pitch, escapes, escapeses, holex, holey, 2, 0,  1, escapes_sub, 0, endd, smallest, good_escape, n, m);
    if(endd[2] != 1) escapes[2] = 0; 
    if(escapes[2] > 0 && escapes[2] < smallest) smallest = escapes[2];

    if(escapeses[3] == true) moving_checker_sub(pitch, escapes, escapeses, holex, holey, 3, 0, -1, escapes_sub, 0, endd, smallest, good_escape, n, m);
    if(endd[3] != 1) escapes[3] = 0; 
    if(escapes[3] > 0 && escapes[3] < smallest) smallest = escapes[3];
    }

    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < m; j++)
        {
            switch(pitch[i][j])
            {
            case 0:  printf("%s", "\U0001f9f1"); break;   
            case 10: printf("%s", "\U0001f300"); break;     
            case 20: printf("%s", "\U0001f6a7"); break;
            case 35: printf("%s", "\U0001f600"); break;
            case 70: printf("%s", "\U0001f9f1"); break;
            default: printf("%s", "\U0001f300");
            } 
        }
        printf("\n");
    }
    if(smallest == 200) printf("Выхода нет\n");
    else printf("Кратчайший путь до выхода - %d\n", smallest);
    return 0;
}
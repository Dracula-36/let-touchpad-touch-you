#include <stdint.h>

#include <linux/version.h>
#include <linux/input.h>

#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define KEYLONG 20
#ifndef EV_SYN
#define EV_SYN 0
#endif

char *keys[KEY_MAX + 1] = {
	[0 ... KEY_MAX] = NULL,
    [BTN_TOUCH] = "Touch",          	[BTN_TOOL_FINGER] = "ToolFinger"

};

char *events[EV_MAX + 1] = {
	[0 ... EV_MAX] = NULL,
	[EV_SYN] = "Sync",			[EV_KEY] = "Key",
	[EV_ABS] = "Absolute",
};

char *absval[6] = { "Value", "Min  ", "Max  ", "Fuzz ", "Flat ", "Resolution "};

char *absolutes[ABS_MAX + 1] = {
	[0 ... ABS_MAX] = NULL,
	[ABS_X] = "X",			[ABS_Y] = "Y",
	[ABS_TOOL_WIDTH] = "Tool Width",		
    [ABS_PRESSURE] = "Pressure",	
};

char **names[EV_MAX + 1] = {
	[0 ... EV_MAX] = NULL,
	[EV_SYN] = events,				[EV_ABS] = absolutes,
};

struct coor 
{
    int x;
    int y;
}minco ,maxco;


int x_gap, y_gap, keyword[KEYLONG];
char cmd[20];

int  *creatkey(char *device)
{
	int fd, rd, i, j =0, flag, m, n, current = 0;;
    struct coor coxy;
	struct input_event ev[64];
	int abs[6] = {0};
    for (i = 0;i < KEYLONG; i++) keyword[i] = 0;

    fd = open(device, O_RDONLY);
	while (1) {
		rd = read(fd, ev, sizeof(struct input_event) * 64);

		for (i = 0, flag = 0; i < rd / sizeof(struct input_event); i++){
			if (ev[i].type != EV_SYN) {
				if (ev[i].code == 0) m = ev[i].value, flag++;
                if (ev[i].code == 1) n = ev[i].value, flag++;
                if ((ev[i].code == 24)&&(ev[i].value == 0)) goto end;
			}	
	    }
        if (flag == 2) coxy.x = m,coxy.y = n;
        if (current != judge(coxy.x, coxy.y)) {
            current = judge(coxy.x, coxy.y);
            keyword[j++] = current;
        }
    }
    end:return keyword;
}
void getmm (char *device)
{
	int fd, rd, i, flag, m, n;
	struct input_event ev[64];
	int abs[6] = {0};

	fd = open(device, O_RDONLY);
	while (1) {
		rd = read(fd, ev, sizeof(struct input_event) * 64);

		for (i = 0, flag = 0; i < rd / sizeof(struct input_event); i++){
			if (ev[i].type != EV_SYN) {
				if (ev[i].code == 0) m = ev[i].value;
                if (ev[i].code == 1) n = ev[i].value;
			    if ((ev[i].code == 24)&&(ev[i].value == 0)) return;
            }
	    }
        if (m > maxco.x) maxco.x = m;
        if (m < minco.x) minco.x = m;
        if (n > maxco.y) maxco.y = n;
        if (n < minco.y) minco.y = n;
    }
}

int judge(int x, int y)
{
	int x0 = x - minco.x, y0= y - minco.y;
    int n;
    if (x0 <= x_gap/3 && y0 <= y_gap/3)
		n = 1;
	else if (x0 <= x_gap/3 && y0 <= 2*y_gap/3)
		n = 2;
	else if (x0 <= x_gap/3 && y0 <= y_gap)
		n = 3;
	else if (x0 <= 2*x_gap/3 && y0 <= y_gap/3)
		n = 4;
	else if (x0 <= 2*x_gap/3 && y0 <= 2*y_gap/3)
		n = 5;
	else if (x0 <= 2*x_gap/3 && y0 <= y_gap)
		n = 6;
	else if (x0 <= x_gap && y0 <= y_gap/3)
		n = 7;
	else if (x0 <= x_gap && y0 <= 2*y_gap/3)
		n = 8;
	else if (x0 <= x_gap && y0 <= y_gap)
		n = 9;

	return n;
}	

void save_cmd(int *key, char *cmd)
{
	freopen("./config", "a+", stdout);
	int i=0;
	while (key[i] != 0)
	{
		printf("%d", key[i]);
		key[i++] = 0;
	}
	printf("%d%s\n", -1, cmd);
	i = 0;
	while(cmd[i] != '\0')
	{
		cmd[i++] = '\0';
	}
	freopen("/dev/tty", "w", stdout);
}

void readfile(void)
{
	freopen("./config", "r", stdin);
	int x,y;
	scanf("x_gap=%d\ny_gap=%d\n", &x, &y);
	char current_cmd[20];
	int current_key[KEYLONG];
	while(1)
	{
		int i=0;
		do{
			scanf("%d", &current_key[i]);
			if(current_key==-2)
			{
				goto:end;
			}
		}while(current_key[i++] != -1);
		current_key[i - 1]=0;
		i = 0;
		while(current_key[i]!=0)
		{
			printf("%d\n", current_key[i++]);
		}
		scanf("%s", current_cmd);
		printf("%s\n", current_cmd);
	}
end:
	freopen("/dev/tty", "w", stdin);
}

int main(int argc, char **argv)
{
    int i = 0;
    printf("Please correct your touchpad by drawing your touchpad !\n");
    getmm(argv[argc - 1]);
    x_gap = maxco.x - minco.x;
    y_gap = maxco.y - minco.y;
    freopen("./config", "a+", stdout);
    printf("x_gap=%d\n", x_gap);
    printf("y_gap=%d\n", y_gap);
    freopen("/dev/tty", "w", stdout);
    printf("Please draw your picture on your touchpad !\n");
    creatkey(argv[argc - 1]);
   	strcpy(cmd, "unclock");
   	save_cmd(keyword, cmd);

   	//存储快速启动程序
   	char ch;
   	while( 1 ){
   		printf("Do you want to start app quickly?(y/n)\n");
   		scanf(" %c", &ch);
   		if (ch == 'y')
   		{		
 			break;
   		}
	    printf("Please input the app you want to open : \n");
	    scanf("%s", cmd);
	    printf("Please draw a key : \n");
	    creatkey(argv[argc-1]);
	    save_cmd(keyword, cmd);
	}
	freopen("./config", "a+", stdout);
	printf("%d\n", -2);
	freopen("/dev/tty", "w", stdout);
    readfile();
    return 0;
}

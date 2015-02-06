#include <stdint.h>

#include <linux/version.h>
#include <linux/input.h>

#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

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

char *getpath(void)
{
    DIR *dir;
    struct dirent *ptr;
    static char dpath[100] = "/dev/input/";
    char name[256] = "Unknown";
    if ((dir = opendir("/dev/input")) == NULL)
    {  
        perror("Open /dev/input error...");
        exit(1);
    }
    while ((ptr=readdir(dir)) != NULL)
    {
        if (ptr->d_type == 2)
        {
            char tpath[100] = "/dev/input/";
            ioctl(open(strcat(tpath,ptr->d_name), O_RDONLY),EVIOCGNAME(sizeof(name)),name);
            if (strcmp(name,"SynPS/2 Synaptics TouchPad") == 0)
            {
                strcat(dpath,ptr->d_name);
                break;
            }
        }
    }
    closedir(dir);
    return dpath;
}

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
    int n = 0;
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
		printf(" %d", key[i]);
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

char* readfile(void)
{
	freopen("./config", "r", stdin);
	char path[20];
	scanf("path=%s\n", path);
	scanf("x_gap=%d\ny_gap=%d\n", &x_gap, &y_gap);
	static char current_cmd[20];
	int current_key[KEYLONG];
	int flag ;
    while(1)
	{
		int i=0;
        flag = 1;
		do{
			scanf(" %d", &current_key[i]);
			if(current_key [i] == -2)
			{
				flag = 0;
                goto end;
			}
		}while(current_key[i++] != -1);
		current_key[i - 1]=0;
		i = 0;
		while(current_key[i]!=0)
		{
            if(current_key[i] != keyword[i])
			{
				flag = 0;
				break;
			}
			i++;
		}		
		scanf("%s", current_cmd);
		printf("matching : %s\n", current_cmd);
		if (flag == 1)
		{
			break;
		}
	}
end:
	freopen("/dev/tty", "w", stdin);
	if (flag == 0)
	{
		current_cmd[0] = -1;
	}
	return   current_cmd;
}

int main(void)
{
    FILE *fp;
	fp = fopen("./config", "r");
	char *path = getpath();
    if(fp == NULL)
    {
	    printf("Please correct your touchpad by drawing your touchpad !\n");
        getmm(path);
	    x_gap = maxco.x - minco.x;
	    y_gap = maxco.y - minco.y;
	    freopen("./config", "a+", stdout);
	    printf("x_gap=%d\n", x_gap);
	    printf("y_gap=%d\n", y_gap);
	    freopen("/dev/tty", "w", stdout);
	    printf("Please draw your picture for unlock on your touchpad !\n");
	    creatkey(path);
	   	strcpy(cmd, "unclock");
	   	save_cmd(keyword, cmd);

	   	//存储快速启动程序
	   	char ch;
	   	while( 1 ){
	   		printf("Do you want to start app quickly?(y/n)\n");
	   		scanf(" %c", &ch);
	   		if (ch == 'n')
	   		{		
	 			break;
	   		}
		    printf("Please input the app you want to open : \n");
		    scanf("%s", cmd);
		    printf("Please draw a key : \n");
		    creatkey(path);
		    save_cmd(keyword, cmd);
		}
		freopen("./config", "a+", stdout);
		printf("%d\n", -2);
		freopen("/dev/tty", "w", stdout);
	}
    while(1)
    {
	    char *p;
	    creatkey(path);
	    int i =0;
        p = readfile();
	    if (p[0] != -1)
	    {
		    printf("%s\n", p);
	    }
    }
	fclose(fp);
    return 0;
}

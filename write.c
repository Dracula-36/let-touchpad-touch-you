#include <stdio.h>
#include <stdlib.h>

struct CMD
{
	int key;
	char app[10];
};

int main(void)
{
	FILE *fp;
	char path[10] = "./confiug";
	fp = fopen(path, "w+");	
	struct CMD cmd[10];
	int i;
	for (i=0; i<10; i++)
	{
		cmd[i].key = 0;
	}
	i = 0;
	char ch = 'y';	
	while (ch == 'y' && i < 10)
	{
		printf("please input n : ");
		scanf(" %d", &cmd[i].key);
		fwrite(&cmd[i].key, sizeof(int), 1, fp);
		printf("please input app : ");
		scanf(" %s", cmd[i].app);
		fwrite(cmd[i].app, sizeof(cmd[i].app), 1, fp);
		printf(" go on?");
		scanf(" %c", &ch);
		i++;
	}
	return 0;
}
#include <stdio.h>
#include <stdlib.h>

int judge(int x, int y);
int create_key(void);

int main(int argc, char const *argv[])
{
	printf("%d\n", create_key());
	return 0;
}

int judge(int x_min, int x_max, int y_min, int y_max)
{
	//触摸板分割为9个区块，依次对应1-9
	x = x_max - x_min;
	y = y_max - y_min;
	if (x0 <= x/3 && y0 <= y/3)
		n = 1;
	else if (x0 <= x/3 && y0 <= 2*y/3)
		n = 2;
	else if (x0 <= x/3 && y0 <= y)
		n = 3;
	else if (x0 <= 2*x/3 && y0 <= y/3)
		n = 4;
	else if (x0 <= 2*x/3 && y0 <= 2*y/3)
		n = 5;
	else if (x0 <= 2*x/3 && y0 <= y)
		n = 6;
	else if (x0 <= x && y0 <= y/3)
		n = 7;
	else if (x0 <= x && y0 <= 2*y/3)
		n = 8;
	else if (x0 <= x && y0 <= y)
		n = 9;

	return n;
}	

int create_key(void)
{
	//产生对应的密码
	int temp = 0;
	int current = judge(9, 9);
	int key[20];
	int i = 0;
	int key[i] = current;
	while(1)
	{
		temp = judge(2, 7);
		if(current != temp)
		{
			current = temp;
			key[i++] = current;
		} 
	}
	return key; 
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	FILE *fp;
	freopen("./config", "r", stdin);
	int n;
	scanf("%d", &n);
	if (n == 123)
	{
		char str[10];
		scanf(" %s", str);
		system(str);
	}
	return 0;
}

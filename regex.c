#include <stdio.h>
#include <sys/types.h>
#include <regex.h>

int main(int argc, char * argv[])
{
	int statis;
	int i;
	int cflags = REG_EXTENDED;
	regmatch_t pmatch[1];
	const size_t nmatch = 1;
	regex_t = reg;
	const char * pattern = "\\w+([-+.]\\w+)*@\\w+([-.]\\w+)*.\\w+([-x]\\w+)&$";
	char * buf = "djaflkkfajl@gmail.com";

	recomp(&reg, pattern, cflags);
	status = regexec(&reg, buf, nmatch, pmatch, 0);
	if ( status == 0)
	{
		printf("Match : \n");
		for (i = pmatch[0].rm_so; i < pmatch[0].rm_eo; i++)
		{
			putchar(buf[i]);
		}
		printf("\n");
	}
}











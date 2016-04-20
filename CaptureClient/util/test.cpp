#include <stdio.h>
int main(int argc, char const *argv[])
{
	unsigned char t;
	t = 0x7ff<<6;
	printf("%x\n",(unsigned char) t);
	return 0;
}
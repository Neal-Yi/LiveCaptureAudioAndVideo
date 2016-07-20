#include "cstdio"

char lineStr[] = "this is a test file";

char opFilename[] = "bigfile.txt";

int main(int argc, char const *argv[])
{
	/* code */
	FILE* of = fopen(opFilename,"w");
	for (int i = 0; i < 1e7; ++i)
	{
		/* code */
		fprintf(of, lineStr);
		fprintf(of, "\n");
	}
	fclose(of);
	return 0;
}
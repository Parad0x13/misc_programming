// Something from Yongsan I made

#include <stdio.h>

#define Records_Location "\"P:/NEW TECH SVC/00_IA_IT_Records\""
#define Accountability_Worksheet "\"IA Accountability Worksheet.xlsx\""
#define Accountability_Worksheet_Location Records_Location"\\"Accountability_Worksheet

void logArguments(int argc, char **argv);
void openAccountabilityWorksheet();

int main(int argc, char **argv) {
	logArguments(argc, argv);
	openAccountabilityWorksheet();

	system("Pause");
}

void logArguments(int argc, char **argv) {
	int d;
	printf("Have %i arguments\n", argc);
	for(d=0;d<argc;d++)
		printf("is: %s\n", argv[d]);
}

void openAccountabilityWorksheet() {
	char command[100];
	snprintf(command, 100, "\"%s\"", Accountability_Worksheet_Location);
	system(command);
}

/*
Name: Game Modifier
Description:
	1. List all process
	2. Select spefic process and modify memory.
	3. Kill the process
	4. Exit
Build by: etandmouse@gmail.com
*/
#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <TlHelp32.h>

int main(void)
{
	//input for user
	int select = 0;
	while (true)
	{
		//display menu
		ShowMenu();

		while (!scanf_s("%d", &select))
		{
			rewind(stdin);
		}

		switch (select)
		{
		case 1:
			ShowProcessList();
			break;
		case 2:
			EditProcessData();
			break;
		case 3:
			KillProcess();
			break;
		case 4:
			printf("Thank for your using....");
			system("pause");
			return 0;
		default:
			printf("Please select again 1~4");
			break;
		}
	}	

	return 0;
}
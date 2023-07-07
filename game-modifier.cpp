/*
Name: Game Modifier
Function:
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

void ShowMenu();
void ShowProcessList();
void EditProcessData();
void KillProcess();

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
			printf("Please enter your choice.");
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
			printf("Thank for your using....\n");
			system("pause");
			return 0;
		default:
			printf("Please select again 1~4\n");
			break;
		}
	}	

	return 0;
}

void ShowMenu()
{
	printf("-----------------------------------------------\n");
	printf("Menu:\n");
	printf("\t1. Show system process list.\n");
	printf("\t2. Input porcess id which you want to edit.\n");
	printf("\t3. Input porcess id which you want to kill.\n");
	printf("\t4. Exit.\n");
	printf("------------------------------------------------\n");
}
void ShowProcessList()
{
	
	PROCESSENTRY32 pc;
	pc.dwSize = sizeof(pc);
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	BOOL bMore = Process32First(hProcessSnap, &pc);
	while(bMore)
	{
		printf("\n----------------------------\n");
		printf("id:%d\n", pc.th32ProcessID);
		wprintf(L"name:%s\n", pc.szExeFile);
		bMore = Process32Next(hProcessSnap, &pc);
	}
	CloseHandle(hProcessSnap);
}
void EditProcessData()
{

}
void KillProcess()
{
	//input pid
	BOOL bRet;
	DWORD dwId = 0;
	printf("Enter process id which you want to kill...\n");
	while (!scanf_s("%u", &dwId))
	{
		rewind(stdin);
		printf("Please enter process id which you want to kill...");
	}	
	//Open process
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwId);
	if (hProcess != NULL)
	{
		bRet = TerminateProcess(hProcess, 0);
	}
	CloseHandle(hProcess);

	if (bRet)
	{
		printf("Kill process success.\n");
	}
	else
	{
		printf("Kill process fail.\n");
	}
	
}
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

const DWORD KONEK = 1024;
const DWORD KPAGE = 4 * KONEK;
const DWORD KONEG = KONEK * KONEK * KONEK;

void ShowMenu();
void ShowProcessList();
void EditProcessData();
void KillProcess();

void FirstRound(HANDLE hProcess, DWORD dwValue, DWORD* pAddrList, DWORD* pAddrListCounter, const DWORD addrListMax);
BOOL CompareOnePage(HANDLE hProcess, DWORD dwBaseAddr, DWORD dwValue, DWORD* pAddrList, DWORD* pAddrListCounter, const DWORD addrListMax);
void ShowAddrList(DWORD* dwAddrList, DWORD dwAddrCount);

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
	DWORD dwId = 0;
	DWORD dwSearchValue = 0;
	DWORD dwAddrList[4 * KONEK] = { 0 };
	DWORD dwAddrCount = 0;
	BOOL bRet = FALSE;
	printf("Please enter process id which you want to edit...");
	while (!scanf_s("%u", &dwId))
	{
		rewind(stdin);
		printf("Please enter process id which you want to edit...");
	}
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwId);
	if (NULL != hProcess)
	{
		printf("Open process fail\n");
		return;
	}
	else
	{
		printf("Open process success\n");
		system("pause");
		printf("Please enter valuse which you want to search first round\n");
		while (!scanf_s("%u", &dwSearchValue))
		{
			rewind(stdin);
			printf("Please enter valuse which you want to search first round\n");
		}
		
		FirstRound (hProcess, dwSearchValue, dwAddrList, &dwAddrCount, 4 * KONEK);
		ShowAddrList(dwAddrList, dwAddrCount);

		if (dwAddrCount == 0)
		{
			return;
		}
		else if (dwAddrCount == 1)
		{
			DWORD value;
			scanf_s("%u", &value);
			bRet = WriteProcessMemory(hProcess, (LPVOID) dwAddrList[0], (LPCVOID)&value, sizeof(DWORD), NULL);
			if (bRet)
			{
				printf("Edit success!!!\n");
			}
			else
			{
				printf("Edit failed!!!\n");
			}
		}
		else
		{
			//dwAddrCount is more than 1
			SecondRound();
		}
	}
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
void FirstRound(HANDLE hProcess, DWORD dwValue, DWORD *pAddrList, DWORD *pAddrListCounter, const DWORD addrListMax)
{
	DWORD dwBaseAddr = 64 * KONEK;
	DWORD dwPageCount = (2 * KONEG - 64 * KONEK * 2) / KPAGE;
	printf("%u pages \n", dwPageCount);
	printf("Start searching ...\n");
	DWORD dwBeginAddr = dwBaseAddr;
	for (; dwBaseAddr < 2 * KONEG - 64 * KONEK; dwBaseAddr += KPAGE)
	{
		if (!CompareOnePage(hProcess, dwBaseAddr, dwValue, pAddrList, pAddrListCounter, addrListMax))
		{
			return;
		}

		DWORD page = (dwBaseAddr - dwBeginAddr) / KPAGE + 1;
		printf("current is %u page\n", page);
		double temp = ((double)page / dwPageCount) * 100;
		printf("-------------%%%f--------------", temp);
	}
	printf("\nSearch finished...\n");
	system("pause");
}

BOOL CompareOnePage(HANDLE hProcess, DWORD dwBaseAddr, DWORD dwValue, DWORD *pAddrList, DWORD *pAddrListCounter, const DWORD addrListMax)
{
	BYTE byPage[KPAGE] = { 0 };
	if (!ReadProcessMemory(hProcess, (LPCVOID)dwBaseAddr, (LPVOID)byPage, KPAGE, NULL))
	{
		printf("Read Memory error!!!\n");
		return TRUE;
	}

	DWORD* pdwPointer = NULL;
	pdwPointer = (DWORD *)byPage;

	for (DWORD i = 0; i < KONEK; i++)
	{
		if (*pAddrListCounter >= addrListMax)
		{
			printf("Too many data, can not save...\n");
			return FALSE;
		}
		if (pdwPointer[i] == dwValue)
		{
			pAddrList[*pAddrListCounter] = dwBaseAddr + i * sizeof(DWORD);
			(*pAddrListCounter)++;
		}
	}
	return TRUE;
}

void ShowAddrList(DWORD *pDwAddrList, DWORD dwAddrCount)
{
	printf("\n--------------Address list brgin--------------------\n");
	for (DWORD i = 0; i < dwAddrCount; i++)
	{
		if (pDwAddrList[i] != 0)
		{
			printf("dwAddrList[%u] = %X", dwAddrCount, pDwAddrList[i]);
		}
		
	}
	printf("\n--------------Address list end--------------------\n");
}

void SecondRound(HANDLE hProcess, DWORD dwValue, DWORD *pDwAddrList, DWORD dwAddrListCount,)
{

}
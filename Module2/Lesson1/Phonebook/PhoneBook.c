﻿/*
* Телефонная книга
* 
* Функционал: добавление, удаление, редактирование и просмотр контактов
* 
*/

#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSTRING 50
#define BOOKVOLUME 15
#define MAXPHONES 3
#define MAXEMAILS 3

typedef struct
{
	char VK[MAXSTRING];
	char TG[MAXSTRING];
	char inst[MAXSTRING];
	char OK[MAXSTRING];
	char FB[MAXSTRING];
}SocNet;

typedef struct
{
	char Name[MAXSTRING];
	char Surname[MAXSTRING];
	char Patronymic[MAXSTRING];

	char PhonesNumber[MAXPHONES][MAXSTRING];
	char EMail[MAXEMAILS][MAXSTRING];
	char JobPlace[MAXSTRING];
	char JobTitle[MAXSTRING];

	SocNet Networks;
} contact;

contact PhoneBook[BOOKVOLUME];
int volume = 0;

int addContact();
int editContact();
int deleteContact();
int displayPhoneBook();

int main()
{
	setlocale(LC_ALL, "ru");
	SetConsoleCP(1251);

	int choice;

	while (1)
	{
		printf("\n-----МЕНЮ-----\n");
		printf("1. Добавить контакт\n");
		printf("2. Редактировать контакт\n");
		printf("3. Удалить контакт\n");
		printf("4. Показать все контакты\n");
		printf("5. Выход\n");
		printf("Выбор: ");
		scanf("%d", &choice);

		switch (choice) {
		case 1:
			addContact();
			break;
		case 2:
			editContact();
			break;
		case 3:
			deleteContact();
			break;
		case 4:
			displayPhoneBook();
			break;
		case 5:
			return 0;
			break;
		default:
			printf("Некорректный выбор, попробуйте снова.\n");
		}
	}

	return 0;
}

int addContact()
{
	if (volume >= BOOKVOLUME)
	{
		printf("Телефонная книга заполнена.\n");
		return -1;
	}

	contact newContact;
	printf("Введите фамилию: ");
	scanf("%s", newContact.Surname);
	printf("Введите имя: ");
	scanf("%s", newContact.Name);

	printf("Введите отчество: ");
	getchar();
	fgets(newContact.Patronymic, MAXSTRING, stdin);
	newContact.Patronymic[strcspn(newContact.Patronymic, "\n")] = '\0';

	printf("Введите место работы: ");
	fgets(newContact.JobPlace, MAXSTRING, stdin);
	newContact.JobPlace[strcspn(newContact.JobPlace, "\n")] = '\0';

	printf("Введите должность: ");
	fgets(newContact.JobTitle, MAXSTRING, stdin);
	newContact.JobTitle[strcspn(newContact.JobTitle, "\n")] = '\0';

	printf("Количество номеров телефона(не более %d): ", MAXPHONES);
	int n;
	scanf("%d", &n);
	getchar();
	for (int i = 0; i < n && i < MAXPHONES; ++i)
	{
		printf("Введите номер телефона: ");
		fgets(newContact.PhonesNumber[i], MAXSTRING, stdin);
		newContact.PhonesNumber[i][strcspn(newContact.PhonesNumber[i], "\n")] = '\0';
	}
	for (int i = n; i < MAXPHONES; ++i) strcpy(newContact.PhonesNumber[i], "\0");


	printf("Количество электронных почт(не более %d): ", MAXEMAILS);
	scanf("%d", &n);
	getchar();
	for (int i = 0; i < n && i < MAXPHONES; ++i)
	{
		printf("Введите электронную почту: ");
		fgets(newContact.EMail[i], MAXSTRING, stdin);
		newContact.EMail[i][strcspn(newContact.EMail[i], "\n")] = '\0';
	}
	for (int i = n; i < MAXPHONES; ++i) strcpy(newContact.EMail[i], "\0");

	printf("Аккаунт FB: ");
	fgets(newContact.Networks.FB, MAXSTRING, stdin);
	newContact.Networks.FB[strcspn(newContact.Networks.FB, "\n")] = '\0';
	printf("Аккаунт VK: ");
	fgets(newContact.Networks.VK, MAXSTRING, stdin);
	newContact.Networks.VK[strcspn(newContact.Networks.VK, "\n")] = '\0';
	printf("Аккаунт OK: ");
	fgets(newContact.Networks.OK, MAXSTRING, stdin);
	newContact.Networks.OK[strcspn(newContact.Networks.OK, "\n")] = '\0';
	printf("Аккаунт inst: ");
	fgets(newContact.Networks.inst, MAXSTRING, stdin);
	newContact.Networks.inst[strcspn(newContact.Networks.inst, "\n")] = '\0';
	printf("Аккаунт TG: ");
	fgets(newContact.Networks.TG, MAXSTRING, stdin);
	newContact.Networks.TG[strcspn(newContact.Networks.TG, "\n")] = '\0';

	PhoneBook[volume++] = newContact;
	printf("Контакт добавлен!\n");
	return 0;
}

int editContact()
{
	printf("\nИЗМЕНЕНИЕ КОНТАКТНЫХ ДАННЫХ\nПоиск пользователя:\n\n");

	char SN[MAXSTRING], N[MAXSTRING];
	printf("Введите фамилию: ");
	scanf("%s", &SN);
	printf("Введите имя: ");
	scanf("%s", &N);

	for (int i = 0; i < volume; ++i)
	{

		if (strcmp(PhoneBook[i].Name, N) == 0 &&
			strcmp(PhoneBook[i].Surname, SN) == 0)
		{
			while (1)
			{
				int state = -1;

				printf("Что хотите изменить?\n\
1. Имя\n\
2. Фамилию\n\
3. Отчество\n\
4. Место работы\n\
5. Должность\n\
6. Социальные сети\n\
7. Номер(-а) телефона(-ов)\n\
8. Электронная(-ые) почта(-ы)\n\
9. Выход из режима редактирования\n\
Выбор: ");
				scanf("%d", &state);

				switch (state)
				{
				case 1:
				{
					printf("Текущее имя: %s\nВведите новое имя: ", PhoneBook[i].Name); getchar();
					if (scanf("%s", PhoneBook[i].Name) == 0)
					{
						printf("Имя не введено");
						return -1;
					}
					break;
				}

				case 2:
				{
					printf("Текущая фамилия: %s\nВведите новую фамилию: ", PhoneBook[i].Surname); getchar();
					if (scanf("%s", PhoneBook[i].Surname) == 0)
					{
						printf("Имя не введено");
						return -1;
					}
					break;
				}

				case 3:
				{
					printf("Текущее отчество: %s\nВведите новое отчество: ", PhoneBook[i].Patronymic); getchar();
					scanf("%s", PhoneBook[i].Patronymic);
					break;
				}

				case 4:
				{
					printf("Текущее место работы: %s\nВведите новое место работы: ", PhoneBook[i].JobPlace); getchar();
					scanf("%s", PhoneBook[i].JobPlace);
					break;
				}

				case 5:
				{
					printf("Текущее место работы: %s\nВведите новое место работы: ", PhoneBook[i].JobTitle); getchar();
					scanf("%s", PhoneBook[i].JobTitle);
					break;
				}

				case 6:
				{
					printf("Аккаунт FB: ");
					fgets(PhoneBook[i].Networks.FB, MAXSTRING, stdin);
					PhoneBook[i].Networks.FB[strcspn(PhoneBook[i].Networks.FB, "\n")] = '\0';
					printf("Аккаунт VK: ");
					fgets(PhoneBook[i].Networks.VK, MAXSTRING, stdin);
					PhoneBook[i].Networks.VK[strcspn(PhoneBook[i].Networks.VK, "\n")] = '\0';
					printf("Аккаунт OK: ");
					fgets(PhoneBook[i].Networks.OK, MAXSTRING, stdin);
					PhoneBook[i].Networks.OK[strcspn(PhoneBook[i].Networks.OK, "\n")] = '\0';
					printf("Аккаунт inst: ");
					fgets(PhoneBook[i].Networks.inst, MAXSTRING, stdin);
					PhoneBook[i].Networks.inst[strcspn(PhoneBook[i].Networks.inst, "\n")] = '\0';
					printf("Аккаунт TG: ");
					fgets(PhoneBook[i].Networks.TG, MAXSTRING, stdin);
					PhoneBook[i].Networks.TG[strcspn(PhoneBook[i].Networks.TG, "\n")] = '\0';
					break;
				}

				case 7:
				{
					printf("Текущие номера телефонов:\n");
					for (int j = 0; j < MAXPHONES; ++j)
					{
						if (strcmp(PhoneBook[i].PhonesNumber[j], "\0")) j = MAXPHONES; break;
						printf("№&d. %s\n", j, PhoneBook[i].PhonesNumber[j]);
					}

					printf("Новое количество номеров телефона(не более %d): ", MAXPHONES);
					int n;
					scanf("%d", &n);
					for (int j = 0; j < n && j < MAXPHONES; ++j)
					{
						printf("Введите номер телефона №%d: ", j + 1); getchar();
						fgets(PhoneBook[i].PhonesNumber[j], MAXSTRING, stdin);
					}
					for (int j = n; j < MAXPHONES; ++j) strcpy(PhoneBook[i].PhonesNumber[j], "\0");
					break;
				}

				case 8:
				{
					printf("Текущие электронные почты:\n");
					for (int j = 0; j < MAXPHONES; ++i)
					{
						if (strcmp(PhoneBook[i].EMail[j], "\0")) j = MAXEMAILS; break;
						printf("№&d. %s\n", j, PhoneBook[i].EMail[j]);
					}

					printf("Новое количество электронных почт(не более %d): ", MAXPHONES);
					int n;
					scanf("%d", &n); getchar();
					for (int j = 0; j < n && j < MAXPHONES; ++j)
					{
						printf("Введите электронную почту №%d: ", j + 1);
						fgets(PhoneBook[i].EMail[j], MAXSTRING, stdin);
						PhoneBook[i].EMail[j][strcspn(PhoneBook[i].EMail[j], "\n")] = '\0';
					}
					for (int j = n; j < MAXEMAILS; ++j) strcpy(PhoneBook[i].EMail[j], "\0");
					break;
				}

				case 9:
					return 0;

				default:
					break;
				}
			}
		}
	}

	printf("Контакт не найден");
	return -1;
}

int deleteContact()
{
	printf("\nУДАЛЕНИЕ КОНТАКТА\nПоиск пользователя:\n");
	char surname[MAXSTRING];
	printf("Введите фамилию контакта для удаления: ");
	scanf("%s", surname);
	char name[MAXSTRING];
	printf("Введите фамилию контакта для удаления: ");
	scanf("%s", name);

	for (int i = 0; i < volume; i++) {
		if (strcmp(PhoneBook[i].Surname, surname) == 0 &&
			strcmp(PhoneBook[i].Name, name) == 0) {
			for (int j = i; j < volume - 1; j++) {
				PhoneBook[j] = PhoneBook[j + 1];
			}
			volume--;
			printf("Контакт удален.\n");
			return 0;
		}
	}
	printf("Контакт не найден.\n");
	return -1;
}

int displayPhoneBook()
{
	printf("\n\nСписок контактов:\n");

	for (int i = 0; i < volume; i++) {
		printf("Контакт №%d:\n", i + 1);
		printf("Имя: %s %s %s\n", PhoneBook[i].Surname, PhoneBook[i].Name, PhoneBook[i].Patronymic);
		printf("Место работы: %s\n", PhoneBook[i].JobPlace);
		printf("Должность: %s\n", PhoneBook[i].JobTitle);
		printf("Номера телефонов:\n");
		for (int j = 0; j < MAXPHONES; ++j)
		{
			if (!strcmp(PhoneBook[i].PhonesNumber[j], "\0"))
			{
				j = MAXPHONES;
				break;
			}
			printf("№%d. %s\n", j, PhoneBook[i].PhonesNumber[j]);
		}

		printf("Эл. почты:\n");
		for (int j = 0; j < MAXEMAILS; ++j)
		{
			if (!strcmp(PhoneBook[i].EMail[j], "\0"))
			{
				j = MAXEMAILS;
				break;
			}
			printf("№%d. %s\n", j, PhoneBook[i].EMail[j]);
		}

		printf("Аккаунт FB: %s\n", PhoneBook[i].Networks.FB);
		printf("Аккаунт VK: %s\n", PhoneBook[i].Networks.VK);
		printf("Аккаунт OK:  %s\n", PhoneBook[i].Networks.OK);
		printf("Аккаунт inst:  %s\n", PhoneBook[i].Networks.inst);
		printf("Аккаунт TG:  %s\n", PhoneBook[i].Networks.TG);
		printf("\n");
	}
	return 0;

}
#include <time.h>
#include <string.h>
#include <ctype.h>
#include "iGraphics.h"
#include <mmsystem.h>
#include "custom_structs.h"

#define DEF_FONT GLUT_BITMAP_HELVETICA_18
#define DEF_FONT_SM GLUT_BITMAP_HELVETICA_12
#define PI acos(-1.0)

const double secAngle = 2 * PI / 60.0;

char filepath[50];

struct tm *currTimeTM;
tm_short currTimeTM_Short;
dayRoutine tempRoutine;
dayRoutine todayRoutine;
dayRoutine tomorrowRoutine;
int currentWaqtVar = 0;

char reminderInputString[100] = "Enter Reminder";
int reminderInputStringInd = 0;
tm_short reminderInputTM;

int inputState = 0;
int reminderCount = 0;
Reminder savedReminders[5];

char screenState = 'h';

int calIconHover = 0;
Date calDisplayDate = {2023, 1, 1};
Date calSetDate = {2023, 1, 1};
int routineToday = 1;

void todayRoutineInit();
void loadDayRoutine(int day, int month, int year);
void updateCurrTime();
int currentWaqt();
double timePercentage(tm_short *start, tm_short *now, tm_short *end);
int timeCompare(tm_short *time1, tm_short *time2);
tm_short *timeUntil(tm_short *buffer, tm_short *time);
void setThemeColor();
int clickedOnRect(int left, int bottom, int width, int height, int mouseX, int mouseY);
int min(int a, int b);

void loadReminders();
void removeReminder(int ind);
void saveReminders();
void recordSingleReminder();
void checkAllReminder();

int checkCalDayClick(int mx, int my);

#include "render_blocks.h"

void iDraw()
{
	iClear();
	updateCurrTime();
	currentWaqtVar = currentWaqt();

	char bgPath[50];

	if (screenState == 'h')
	{
		sprintf(bgPath, ".\\resources\\image\\background-%d.png", currentWaqtVar);
		iShowBMP(0, 0, bgPath);
		renderDateBlock();
		renderForbiddenBlock();
		renderWaqtBlock();
		renderWaqtCountdownBlock();
		renderReminderBlock();
	}
	else if (screenState == 'q')
	{
		sprintf(bgPath, ".\\resources\\image\\bg-quran-%d.png", currentWaqtVar);
		iShowBMP(0, 0, bgPath);
		renderSurahList();
		checkAllReminder();
	}
	else if (screenState == 'c')
	{
		sprintf(bgPath, ".\\resources\\image\\bg-cal-%d.png", currentWaqtVar);
		iShowBMP(0, 0, bgPath);
		renderCalender();

		checkAllReminder();
	}
}

void iMouseMove(int mx, int my)
{
}

void iMousePos(int mx, int my)
{
	if (clickedOnRect(60, 530, 128, 128, mx, my))
	{
		calIconHover = 1;
	}
	else
	{
		calIconHover = 0;
	}
}

void iMouse(int button, int state, int mx, int my)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		// printf("%d %d\n", mx, my);
		if (screenState == 'h')
		{
			if (clickedOnRect(60, 530, 128, 128, mx, my))
			{
				inputState = 0;
				screenState = 'c';
				return;
			}
			if (clickedOnRect(210, 540, 10, 10, mx, my))
			{
				todayRoutineInit();
				routineToday = 1;
				return;
			}
			for (int i = 0; i < reminderCount; i++)
			{
				if (clickedOnRect(1220, 655 - 130 * i, 14, 14, mx, my))
				{
					removeReminder(i);
					saveReminders();
					return;
				}
			}

			if (clickedOnRect(900, 130, 320, 30, mx, my))
			{
				if (inputState != 1)
				{
					reminderInputStringInd = 0;
					memset(reminderInputString, 0, sizeof(reminderInputString));
				}

				inputState = 1;

				return;
			}
			else if (clickedOnRect(900, 90, 50, 30, mx, my))
			{
				inputState = 2;
				return;
			}
			else if (clickedOnRect(960, 90, 50, 30, mx, my))
			{
				inputState = 3;
				return;
			}
			else if (reminderCount < 5 && clickedOnRect(1150, 90, 70, 30, mx, my))
			{
				recordSingleReminder();
				inputState = 0;
				return;
			}
			else if (clickedOnRect(640, 0, 640, 60, mx, my))
			{
				screenState = 'q';
				inputState = 0;
			}
			else
			{
				inputState = 0;

				return;
			}
		}
		else if (screenState == 'q')
		{
			if (clickedOnRect(0, 0, 640, 60, mx, my))
			{
				screenState = 'h';
				inputState = 0;
				return;
			}

			int surahClicked = 0;
			for (int c = 0; c < 3; c++)
			{
				int x;
				switch (c)
				{
				case 0:
					x = 75;
					break;
				case 1:
					x = 500;
					break;
				case 2:
					x = 920;
					break;
				default:
					break;
				}
				for (int i = 0; i < 38; i++)
				{
					if (!surahClicked && clickedOnRect(x, 675 - 16 * i, 200, 15, mx, my))
					{
						surahHover[c][i] = 1;
						surahClicked = 1;

						mciSendString("close surah", NULL, 0, NULL);
						sprintf(displayString2, "open \".\\resources\\audio\\surah\\surah-%03d.mp3\" type mpegvideo alias surah", 38 * c + i + 1);
						mciSendString(displayString2, NULL, 0, NULL);
						mciSendString("seek surah to start", NULL, 0, NULL);
						mciSendString("play surah", NULL, 0, NULL);
					}
					else
					{
						surahHover[c][i] = 0;
					}
				}
			}
			if (!surahClicked)
			{
				mciSendString("close surah", NULL, 0, NULL);
			}
			surahClicked = 0;
		}
		else if (screenState == 'c')
		{
			if (clickedOnRect(0, 0, 640, 60, mx, my))
			{
				screenState = 'h';
				return;
			}
			if (clickedOnRect(640, 0, 640, 60, mx, my))
			{
				screenState = 'q';
				return;
			}
			if (clickedOnRect(400, 550, 60, 60, mx, my))
			{
				if (calDisplayDate.month > 1)
				{
					calDisplayDate.month--;
				}
				return;
			}
			if (clickedOnRect(820, 550, 60, 60, mx, my))
			{
				if (calDisplayDate.month < 12)
				{
					calDisplayDate.month++;
				}
				return;
			}
			int clickedDay = checkCalDayClick(mx, my);
			if (clickedDay)
			{
				calSetDate = calDisplayDate;
				calSetDate.day = clickedDay;
				routineToday = 0;
				loadDayRoutine(calSetDate.day, calSetDate.month, calSetDate.year % 100);
				screenState = 'h';
				return;
			}
		}
	}
}

void iKeyboard(unsigned char key)
{
	if (key == 27)
	{
		exit(0);
	}
	if (inputState)
	{
		if (inputState == 1)
		{
			if (isgraph(key) || key == ' ')
			{
				reminderInputString[reminderInputStringInd] = key;
				reminderInputStringInd++;
			}
			else if (key == '\b')
			{
				if (reminderInputStringInd > 0)
				{
					reminderInputStringInd--;
				}
				reminderInputString[reminderInputStringInd] = '\0';
			}
		}
		else if (inputState == 2)
		{
			if (isdigit(key))
			{
				reminderInputTM.hour *= 10;
				reminderInputTM.hour += key - '0';
				reminderInputTM.hour %= 100;
			}
			else if (key == '\b')
			{
				reminderInputTM.hour /= 10;
			}
		}
		else if (inputState == 3)
		{
			if (isdigit(key))
			{
				reminderInputTM.min *= 10;
				reminderInputTM.min += key - '0';
				reminderInputTM.min %= 100;
			}
			else if (key == '\b')
			{
				reminderInputTM.min /= 10;
			}
			else if (key == '\r')
			{
				recordSingleReminder();
				inputState = 0;
			}
		}
		if (key == '\t')
		{
			inputState++;
			if (inputState > 3)
			{
				inputState = 0;
			}
		}
	}
}

void iSpecialKeyboard(unsigned char key)
{
}

void loadSurahNames()
{
	FILE *surahFile = fopen(".\\resources\\database\\surahNames.txt", "r");
	if (!surahFile)
	{
		printf("Failed to load database.\n");
		return;
	}
	int i = 0;
	while (!feof(surahFile) && i < 114)
	{
		// fscanf(surahFile, "%[^\n]s ", surahNames[i]);
		fgets(surahNames[i], 49, surahFile);
		// surahNames[i][strcspn(surahNames[i], "\n")] = 0;
		i++;
	}
	fclose(surahFile);
}

void loadDayRoutine(int day, int month, int year)
{
	sprintf(filepath, ".\\resources\\database\\%02d-%02d.bin", month, year);
	FILE *database = fopen(filepath, "rb");
	if (!database)
	{
		printf("Failed to load database.\n");
		return;
	}
	int count = 0;
	while (count < day)
	{
		if (feof(database))
		{
			printf("File ended unexpectedly\n");
			fclose(database);
			return;
		}
		fread(&todayRoutine, 1, sizeof(dayRoutine), database);
		count++;
	}

	fread(&tomorrowRoutine, sizeof(dayRoutine), 1, database);
	if (feof(database))
	{
		sprintf(filepath, ".\\resources\\database\\%02d-%02d.bin", month + 1, year);
		database = freopen(filepath, "r", database);
		fread(&tomorrowRoutine, sizeof(dayRoutine), 1, database);
	}

	fclose(database);
}

int min(int a, int b)
{
	if (b < a)
		return b;
	else
		return a;
}

void updateCurrTime()
{
	time_t currTime = time(NULL);
	currTimeTM = localtime(&currTime);
	currTimeTM_Short = {currTimeTM->tm_hour, currTimeTM->tm_min, currTimeTM->tm_sec};
}

int timeCompare(tm_short *time1, tm_short *time2)
{
	if (time1->hour > time2->hour)
		return -1;
	if (time1->hour < time2->hour)
		return 1;

	if (time1->min > time2->min)
		return -1;
	if (time1->min < time2->min)
		return 1;

	if (time1->sec > time2->sec)
		return -1;
	if (time1->sec < time2->sec)
		return 1;

	return 0;
}

double timePercentage(tm_short *start, tm_short *now, tm_short *end)
{
	int total = (end->hour - start->hour) * 3600 + (end->min - start->min) * 60 + (end->sec - start->sec);
	int elapsed = (now->hour - start->hour) * 3600 + (now->min - start->min) * 60 + (now->sec - start->sec);
	if (end->hour < start->hour)
		total += 24 * 3600;
	if (now->hour < start->hour)
		elapsed += 24 * 3600;
	return (double)elapsed / total;
}

tm_short *timeUntil(tm_short *buffer, tm_short *time)
{
	int carry = 0;
	buffer->sec = time->sec - currTimeTM_Short.sec;
	buffer->min = time->min - currTimeTM_Short.min;
	buffer->hour = time->hour - currTimeTM_Short.hour;
	if (buffer->sec < 0)
	{
		buffer->min--;
		buffer->sec += 60;
	}
	if (buffer->min < 0)
	{
		buffer->hour--;
		buffer->min += 60;
	}
	if (buffer->hour < 0)
	{
		buffer->hour += 24;
	}

	return buffer;
}

int currentWaqt()
{
	if (timeCompare(&currTimeTM_Short, &(todayRoutine.fajr)) != -1)
		return 4;
	if (timeCompare(&currTimeTM_Short, &(todayRoutine.sunrise)) != -1)
		return 0;
	if (timeCompare(&currTimeTM_Short, &(todayRoutine.dhuhr)) != -1)
		return -1;
	if (timeCompare(&currTimeTM_Short, &(todayRoutine.asr)) != -1)
		return 1;
	if (timeCompare(&currTimeTM_Short, &(todayRoutine.maghrib)) != -1)
		return 2;
	if (timeCompare(&currTimeTM_Short, &(todayRoutine.isha)) != -1)
		return 3;
	return 4;
}

int clickedOnRect(int left, int bottom, int width, int height, int mouseX, int mouseY)
{
	if (left <= mouseX && mouseX <= left + width)
	{
		if (bottom <= mouseY && mouseY <= bottom + height)
		{
			return 1;
		}
	}
	return 0;
}

void loadReminders()
{
	FILE *ifp = fopen(".\\resources\\database\\reminders.bin", "rb");

	if (!ifp)
	{
		printf("Error While Opening Reminder File.");
		return;
	}
	fread(&reminderCount, sizeof(int), 1, ifp);
	fread(savedReminders, sizeof(Reminder), reminderCount, ifp);

	fclose(ifp);
}

void removeReminder(int ind)
{
	for (int i = ind; i < reminderCount && i < 4; i++)
	{
		savedReminders[i] = savedReminders[i + 1];
	}
	reminderCount--;
}

void recordSingleReminder()
{
	reminderInputTM.hour = min(reminderInputTM.hour, 23);
	reminderInputTM.min = min(reminderInputTM.min, 59);
	reminderInputTM.sec = min(reminderInputTM.sec, 59);

	strcpy(savedReminders[reminderCount].text, reminderInputString);
	savedReminders[reminderCount].time = reminderInputTM;
	reminderCount++;
	saveReminders();

	// Reset Reminder Input
	reminderInputStringInd = 0;
	strcpy(reminderInputString, "Enter Reminder");
	memset(&reminderInputTM, 0, sizeof(reminderInputTM));
}

void saveReminders()
{
	FILE *ofp = fopen(".\\resources\\database\\reminders.bin", "wb");

	if (!ofp)
	{
		printf("Error While Opening Reminder File.");
		return;
	}
	fwrite(&reminderCount, sizeof(int), 1, ofp);
	fwrite(savedReminders, sizeof(Reminder), reminderCount, ofp);

	fclose(ofp);
}

void checkAllReminder()
{
	char azanTime = 0;
	switch (currentWaqtVar)
	{
	case 1:
		azanTime = timeCompare(&currTimeTM_Short, &(todayRoutine.asr)) == 0;
		break;
	case 2:
		azanTime = timeCompare(&currTimeTM_Short, &(todayRoutine.maghrib)) == 0;
		break;
	case 3:
		azanTime = timeCompare(&currTimeTM_Short, &(todayRoutine.sunset)) == 0;
		break;
	case 4:
		azanTime = timeCompare(&currTimeTM_Short, &(tomorrowRoutine.fajr)) == 0;
		break;
	case -1:
		azanTime = timeCompare(&currTimeTM_Short, &(todayRoutine.dhuhr)) == 0;
		break;

	default:
		break;
	}
	if (azanTime)
	{
		printf("azan time\n");
		mciSendString("seek azan to start", NULL, 0, NULL);
		mciSendString("play azan", NULL, 0, NULL);
	}

	for (int i = 0; i < reminderCount; i++)
	{
		if (timeCompare(&savedReminders[i].time, &currTimeTM_Short) == 0)
		{
			mciSendString("seek reminder_tone to start", NULL, 0, NULL);
			mciSendString("play reminder_tone", NULL, 0, NULL);
		}
	}
}

int checkCalDayClick(int mx, int my)
{
	int day = 1;
	int x = 400;
	int dist = 70;
	int y = 420;
	while (day <= monthDays[calDisplayDate.month - 1])
	{
		setThemeColor();
		iFilledRectangle(x, y, 60, 60);

		if (clickedOnRect(x, y, 60, 60, mx, my))
		{
			return day;
		}

		day++;
		x += dist;
		if (x >= 400 + dist * 7)
		{
			x = 400;
			y -= dist;
		}
	}
	return 0;
}

void todayRoutineInit()
{
	calSetDate.day = currTimeTM->tm_mday;
	calSetDate.month = currTimeTM->tm_mon + 1;
	calSetDate.year = currTimeTM->tm_year + 1900;
	calDisplayDate = calSetDate;
	loadDayRoutine(currTimeTM->tm_mday, currTimeTM->tm_mon + 1, currTimeTM->tm_year % 100);
}

int main()
{
	mciSendString("open \".\\resources\\audio\\reminder_tone.mp3\" type mpegvideo alias reminder_tone", NULL, 0, NULL);
	mciSendString("open \".\\resources\\audio\\azan6.mp3\" type mpegvideo alias azan", NULL, 0, NULL);
	// mciSendString("open \".\\resources\\audio\\surah\\surah-001.mp3\" type mpegvideo alias surah", NULL, 0, NULL);
	time_t currTime = time(NULL);
	currTimeTM = localtime(&currTime);

	todayRoutineInit();

	loadReminders();
	loadSurahNames();

	if (todayRoutine.weekday == 0)
	{
		strcpy(waqtNames[1], "Jum'ah");
	}

	iInitialize(1280, 720, "Muslim's Day Clone");
	mciSendString("close all", NULL, 0, NULL);
	return 0;
}
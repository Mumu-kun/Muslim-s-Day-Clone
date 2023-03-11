#define DEF_FONT GLUT_BITMAP_HELVETICA_18
#define DEF_FONT_SM GLUT_BITMAP_HELVETICA_12

char displayString[50];
char displayString2[80];
tm_short counter;
tm_short zero;

char waqtNames[5][10] = {"Fajr", "Dhuhr", "Asr", "Maghrib", "Isha"};
char surahNames[114][50];
char surahHover[3][38];
short monthDays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
double arrowLeft[2][3] = {{415, 440, 440}, {580, 595, 565}};
double arrowRight[2][3] = {{865, 840, 840}, {580, 595, 565}};

void setThemeColor()
{
    switch (currentWaqtVar)
    {
    case 0:
        iSetColor(136, 188, 185);
        break;
    case 1:
        iSetColor(124, 197, 118);
        break;
    case 2:
        iSetColor(205, 147, 115);
        break;
    case 3:
        iSetColor(206, 114, 132);
        break;
    case 4:
        iSetColor(113, 130, 202);
        break;

    default:
        iSetColor(124, 197, 118);
        break;
    }
}

void animateColor1(int rTarget, int gTarget, int bTarget)
{
    int div = 16;
    static double dr = -(255 - rTarget) / (double)div;
    static double dg = -(255 - gTarget) / (double)div;
    static double db = -(255 - bTarget) / (double)div;
    static double r = 255;
    static double g = 255;
    static double b = 255;
    r += dr;
    g += dg;
    b += db;
    if (r < 255 - (255 - rTarget) * .5)
    {
        dr = -dr;
        dg = -dg;
        db = -db;
    }
    if (r > 255)
    {
        dr = -dr;
        dg = -dg;
        db = -db;
    }
    iSetColor(r, g, b);
}

void animateColor2(int rTarget, int gTarget, int bTarget)
{
    int div = 16;
    static double dr = -(255 - rTarget) / (double)div;
    static double dg = -(255 - gTarget) / (double)div;
    static double db = -(255 - bTarget) / (double)div;
    static double r = 255;
    static double g = 255;
    static double b = 255;
    r += dr;
    g += dg;
    b += db;
    if (r < 255 - (255 - rTarget) * .5)
    {
        dr = -dr;
        dg = -dg;
        db = -db;
    }

    if (r > 255)
    {
        dr = -dr;
        dg = -dg;
        db = -db;
    }
    iSetColor(r, g, b);
}

void renderTimeSpan(char *displayString, tm_short *tm_buffer, tm_short *start, tm_short *end, int rx, int ry)
{
    *tm_buffer = *start;
    timeString(displayString, (tm_buffer));
    iText(rx, ry, displayString, DEF_FONT);

    iText(rx + 50, ry, "-", DEF_FONT);

    *tm_buffer = *end;
    addMinutes(tm_buffer, -1);
    timeString(displayString, (tm_buffer));
    iText(rx + 65, ry, displayString, DEF_FONT);
}

void renderDateBlock()
{
    int clockX = 280;
    int clockY = 430;
    int clockR = 65;
    // iSetColor(255, 255, 255);
    int themeColor[3];
    iGetPixelColor(clockX, clockY, themeColor);
    animateColor1(themeColor[0], themeColor[1], themeColor[2]);
    iFilledCircle(clockX, clockY, clockR);

    iSetColor(themeColor[0] * 1.08, themeColor[1] * 1.08, themeColor[2] * 1.08);
    iFilledSector(clockX, clockY, clockR, PI / 2.0 + secAngle * (currTimeTM_Short.sec - .75), 1.5 * secAngle, 360);
    iSetColor(themeColor[0], themeColor[1], themeColor[2]);
    iFilledCircle(clockX, clockY, clockR - 5);

    char displayString[50];
    timeStringSec(displayString, &currTimeTM_Short);
    iSetColor(255, 255, 255);
    iText(245, 422, displayString, DEF_FONT);

    strcpy(displayString, weekday[todayRoutine.weekday]);
    iText(230, 610, displayString, DEF_FONT);

    dateString(displayString, &(todayRoutine.dateArabic), 1);
    iText(230, 570, displayString, DEF_FONT_SM);

    dateString(displayString, &(todayRoutine.dateEn), 0);
    iText(230, 540, displayString, DEF_FONT_SM);

    timeString(displayString, &(todayRoutine.sunrise));
    iText(80, 440, displayString, DEF_FONT);

    timeString(displayString, &(todayRoutine.sunset));
    iText(80, 380, displayString, DEF_FONT);

    iSetColor(240, 240, 240);
    iText(80, 460, "Sunrise", DEF_FONT_SM);
    iText(80, 400, "Sunset", DEF_FONT_SM);

    sprintf(filepath, ".\\resources\\image\\calendar-%d.bmp", calIconHover);
    iShowBMP2(60, 530, filepath, 0);

    if (!routineToday)
    {
        iSetColor(255, 255, 255);
        iFilledRectangle(210, 540, 10, 10);
        iSetColor(255, 0, 0);
        iLine(210 + 2, 540 + 2, 210 + 8, 540 + 8);
        iLine(210 + 8, 540 + 2, 210 + 2, 540 + 8);
    }
}

void renderForbiddenBlock()
{
    char displayString[50];
    tm_short tm_buffer;
    tm_short forbiddenTime;
    tm_short forbiddenTimeEnd;
    iSetColor(0, 0, 0);

    iText(125, 260, "Forbidden Salat Times", DEF_FONT);

    // Dawn
    forbiddenTime = todayRoutine.sunrise;
    forbiddenTimeEnd = forbiddenTime;
    addMinutes(&forbiddenTimeEnd, 16);

    iText(80, 200, "Dawn :", DEF_FONT);
    renderTimeSpan(displayString, &tm_buffer, &forbiddenTime, &forbiddenTimeEnd, 240, 200);

    // Noon
    forbiddenTime = todayRoutine.dhuhr;
    addMinutes(&forbiddenTime, -7);
    forbiddenTimeEnd = todayRoutine.dhuhr;

    iText(80, 160, "Noon :", DEF_FONT);
    renderTimeSpan(displayString, &tm_buffer, &forbiddenTime, &forbiddenTimeEnd, 240, 160);

    // Dusk
    forbiddenTime = todayRoutine.maghrib;
    addMinutes(&forbiddenTime, -16);
    forbiddenTimeEnd = todayRoutine.maghrib;

    iText(80, 120, "Dusk :", DEF_FONT);
    renderTimeSpan(displayString, &tm_buffer, &forbiddenTime, &forbiddenTimeEnd, 240, 120);
}

void renderWaqtBlock()
{
    char displayString[50];
    tm_short waqtTime;
    iSetColor(255, 255, 255);
    iText(595, 670, "Waqt Times", DEF_FONT);

    setThemeColor();
    switch (currentWaqt())
    {
    case 0:
        iFilledRectangle(454, 593, 375, 46);
        break;
    case 1:
        iFilledRectangle(454, 533, 375, 46);
        break;
    case 2:
        iFilledRectangle(454, 473, 375, 46);
        break;
    case 3:
        iFilledRectangle(454, 413, 375, 46);
        break;
    case 4:
        iFilledRectangle(454, 353, 375, 46);
        break;
    default:
        break;
    }

    iSetColor(0, 0, 0);
    // Fajr
    iText(500, 610, "Fajr", DEF_FONT);
    renderTimeSpan(displayString, &waqtTime, &(todayRoutine.fajr), &(todayRoutine.sunrise), 650, 610);

    // Dhuhr
    if (todayRoutine.weekday == 0)
        iText(500, 550, "Jum'ah", DEF_FONT);
    else
        iText(500, 550, "Dhuhr", DEF_FONT);
    renderTimeSpan(displayString, &waqtTime, &(todayRoutine.dhuhr), &(todayRoutine.asr), 650, 550);

    // Asr
    iText(500, 490, "Asr", DEF_FONT);
    renderTimeSpan(displayString, &waqtTime, &(todayRoutine.asr), &(todayRoutine.maghrib), 650, 490);

    // Maghrib
    iText(500, 430, "Maghrib", DEF_FONT);
    renderTimeSpan(displayString, &waqtTime, &(todayRoutine.maghrib), &(todayRoutine.isha), 650, 430);

    // Isha
    iText(500, 370, "Isha", DEF_FONT);
    if (timeCompare(&currTimeTM_Short, &todayRoutine.fajr) == 1)
        renderTimeSpan(displayString, &waqtTime, &(todayRoutine.isha), &(todayRoutine.fajr), 650, 370);
    else
        renderTimeSpan(displayString, &waqtTime, &(todayRoutine.isha), &(tomorrowRoutine.fajr), 650, 370);
}

void renderWaqtCountDown()
{
    double elapsedPercent;

    strcpy(displayString2, "Time until end of");

    switch (currentWaqtVar)
    {
    case 0:
        timeUntil(&counter, &(todayRoutine.sunrise));
        elapsedPercent = timePercentage(&todayRoutine.fajr, &currTimeTM_Short, &todayRoutine.sunrise);
        break;
    case 1:
        timeUntil(&counter, &(todayRoutine.asr));
        elapsedPercent = timePercentage(&todayRoutine.dhuhr, &currTimeTM_Short, &todayRoutine.asr);
        break;
    case 2:
        timeUntil(&counter, &(todayRoutine.maghrib));
        elapsedPercent = timePercentage(&todayRoutine.asr, &currTimeTM_Short, &todayRoutine.maghrib);
        break;
    case 3:
        timeUntil(&counter, &(todayRoutine.isha));
        elapsedPercent = timePercentage(&todayRoutine.maghrib, &currTimeTM_Short, &todayRoutine.isha);
        break;
    case 4:
        timeUntil(&counter, &(tomorrowRoutine.fajr));
        elapsedPercent = timePercentage(&todayRoutine.isha, &currTimeTM_Short, &tomorrowRoutine.fajr);
        break;
    case -1:
        timeUntil(&counter, &(todayRoutine.dhuhr));
        elapsedPercent = timePercentage(&todayRoutine.sunrise, &currTimeTM_Short, &todayRoutine.dhuhr);
        strcpy(displayString2, "Time until start of");
        break;

    default:
        break;
    }

    if (currentWaqtVar != 0)
    {
        if (timeCompare(&counter, &zero) == 0)
        {
            mciSendString("seek azan to start", NULL, 0, NULL);
            mciSendString("play azan", NULL, 0, NULL);
        }
    }

    int clockX = 750;
    int clockY = 225;
    int clockR = 50;
    int auxColor[3];
    iGetPixelColor(clockX, clockY, auxColor);

    // iSetColor(255, 255, 255);
    animateColor2(auxColor[0], auxColor[1], auxColor[2]);
    iFilledCircle(clockX, clockY, clockR);

    // iSetColor(auxColor[0] * 1.05, auxColor[1] * 1.05, auxColor[2] * 1.05);
    setThemeColor();
    iFilledSector(clockX, clockY, clockR, PI / 2.0, 2 * PI * elapsedPercent);

    iSetColor(auxColor[0], auxColor[1], auxColor[2]);
    iFilledCircle(clockX, clockY, clockR - 5);

    timeStringSec(displayString, &counter);
    iSetColor(0, 0, 0);
    iText(715, 218, displayString, DEF_FONT);
    iText(500, 240, displayString2, DEF_FONT_SM);
    if (currentWaqtVar == -1)
        iText(500, 210, waqtNames[1], DEF_FONT);
    else
        iText(500, 210, waqtNames[currentWaqtVar], DEF_FONT);
}

void renderSehriIftarCountDown()
{
    iSetColor(0, 0, 0);
    // Sehri
    iText(500, 100, "Next Sehri", DEF_FONT_SM);
    if (timeCompare(&currTimeTM_Short, &(todayRoutine.fajr)) == 1)
        counter = todayRoutine.fajr;
    else
        counter = tomorrowRoutine.fajr;

    addMinutes(&counter, -1);
    timeString(displayString, &counter);
    iText(500, 120, displayString, DEF_FONT);

    // Iftar
    iText(600, 100, "Next Iftar", DEF_FONT_SM);
    if (timeCompare(&currTimeTM_Short, &(todayRoutine.maghrib)) == 1)
        counter = todayRoutine.maghrib;
    else
        counter = tomorrowRoutine.maghrib;
    timeString(displayString, &counter);
    iText(600, 120, displayString, DEF_FONT);

    // Countdown
    if (timeCompare(&currTimeTM_Short, &(todayRoutine.fajr)) == 1)
    {
        timeUntil(&counter, &(todayRoutine.fajr));
        iText(700, 100, "Until Sehri Today", DEF_FONT_SM);
    }
    else if (timeCompare(&currTimeTM_Short, &(todayRoutine.maghrib)) == 1)
    {
        timeUntil(&counter, &(todayRoutine.maghrib));
        iText(700, 100, "Until Iftar Today", DEF_FONT_SM);
    }
    else
    {
        timeUntil(&counter, &(tomorrowRoutine.fajr));
        iText(680, 100, "Until Sehri Tomorrow", DEF_FONT_SM);
    }

    timeStringSec(displayString, &counter);
    iText(715, 120, displayString, DEF_FONT);
}

void renderWaqtCountdownBlock()
{
    renderWaqtCountDown();
    renderSehriIftarCountDown();
}

void renderInput()
{
    iSetColor(255, 255, 255);
    iFilledRectangle(900, 130, 320, 30);
    iFilledRectangle(900, 90, 50, 30);
    iFilledRectangle(960, 90, 50, 30);
    // iFilledRectangle(1020, 90, 50, 30);
    setThemeColor();
    iRectangle(900, 130, 320, 30);
    iRectangle(900, 90, 50, 30);
    iRectangle(960, 90, 50, 30);
    iFilledRectangle(1150, 90, 70, 30);

    iSetColor(0, 0, 0);
    iText(953, 100, ":", DEF_FONT);
    // iText(1013, 100, ":", DEF_FONT);
    iSetColor(255, 255, 255);
    iText(1170, 100, "ADD", DEF_FONT_SM);

    if (inputState == 1)
        iSetColor(0, 0, 0);
    else
        iSetColor(100, 100, 100);
    iText(915, 138, reminderInputString, DEF_FONT);

    sprintf(displayString, "%02d", reminderInputTM.hour);
    if (inputState == 2)
        iSetColor(0, 0, 0);
    else
        iSetColor(100, 100, 100);
    iText(915, 98, displayString, DEF_FONT);

    sprintf(displayString, "%02d", reminderInputTM.min);
    if (inputState == 3)
        iSetColor(0, 0, 0);
    else
        iSetColor(100, 100, 100);
    iText(975, 98, displayString, DEF_FONT);
    // sprintf(displayString, "%02d", reminderInputTM.sec);
    // iText(1035, 98, displayString, DEF_FONT);
}

void renderReminders()
{
    for (int i = 0; i < reminderCount; i++)
    {
        // Reminder Text and Time
        iSetColor(0, 0, 0);
        iText(915, 655 - 130 * i, savedReminders[i].text, DEF_FONT);

        iText(915, 622 - 130 * i, "At :", DEF_FONT_SM);

        timeString(displayString, &savedReminders[i].time);
        iText(940, 620 - 130 * i, displayString, DEF_FONT);

        // Reminder Countdown
        iText(1095, 622 - 130 * i, "Remaining :", DEF_FONT_SM);

        timeUntil(&counter, &savedReminders[i].time);
        timeStringSec(displayString, &counter);
        iText(1165, 620 - 130 * i, displayString, DEF_FONT);

        if (timeCompare(&counter, &zero) == 0)
        {
            mciSendString("seek reminder_tone to start", NULL, 0, NULL);
            mciSendString("play reminder_tone", NULL, 0, NULL);
        }

        // X Button
        iSetColor(255, 255, 255);
        iFilledRectangle(1220, 655 - 130 * i, 14, 14);
        iSetColor(255, 0, 0);
        iLine(1220 + 3, 655 - 130 * i + 3, 1220 + 11, 655 - 130 * i + 11);
        iLine(1220 + 11, 655 - 130 * i + 3, 1220 + 3, 655 - 130 * i + 11);
    }
}

void renderReminderBlock()
{

    renderReminders();

    if (reminderCount < 5)
    {
        renderInput();
    }
}

void renderSurahList()
{
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
            if (surahHover[c][i] == 1)
                setThemeColor();
            else
                iSetColor(0, 0, 0);
            iText(x, 675 - 16 * i, surahNames[c * 38 + i], DEF_FONT_SM);
        }
    }
}

void renderCalender()
{
    int day = 1;
    int x = 400;
    int dist = 70;
    int y = 420;
    while (day <= monthDays[calDisplayDate.month - 1])
    {
        setThemeColor();
        if (calSetDate.day == day && calSetDate.month == calDisplayDate.month && calSetDate.year == calDisplayDate.year)
        {
            iRectangle(x, y, 60, 60);
        }
        else
        {
            iFilledRectangle(x, y, 60, 60);
            iSetColor(255, 255, 255);
        }

        sprintf(displayString, "%d", day);
        iText(x + 5, y + 10, displayString, DEF_FONT);

        day++;
        x += dist;
        if (x >= 400 + dist * 7)
        {
            x = 400;
            y -= dist;
        }
    }

    setThemeColor();
    iFilledRectangle(400, 550, 60, 60);
    iFilledRectangle(470, 550, 340, 60);
    iFilledRectangle(820, 550, 60, 60);

    iSetColor(255, 255, 255);
    iFilledPolygon(arrowLeft[0], arrowLeft[1], 3);
    iFilledPolygon(arrowRight[0], arrowRight[1], 3);
    sprintf(displayString, "%s - %d", monthEn[calDisplayDate.month - 1], calDisplayDate.year);
    iText(580, 572, displayString, DEF_FONT);
}
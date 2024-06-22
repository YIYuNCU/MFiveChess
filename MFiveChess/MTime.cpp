#include "pch.h"
#include "MTime.h"

MTime::MTime(int hours, int minutes, int seconds)
{
    this->hours = hours;
    this->minutes = minutes;
    this->seconds = seconds;
}

void MTime::SetNowTime(int seconds, int minutes, int hours)
{
    this->hours = hours;
    this->minutes = minutes;
    this->seconds = seconds;
}

bool MTime::SubSeconds(int seconds)
{
    bgn:
    if (this->seconds > 0)
    {
        this->seconds -= seconds;
        return true;
    }
    else if (this->minutes > 0)
    {
        this->minutes -= 1;
        this->seconds += 60;
        goto bgn;
    }
    else if (this->hours > 0)
    {
        this->hours -= 1;
        this->minutes += 60;
        goto bgn;
    }
    return false;
}

int MTime::GetNowMinutes() const
{
    return minutes;
}

int MTime::GetNowSeconds() const
{
    return seconds;
}

int MTime::GetNowHours() const
{
    return hours;
}

int64_t MTime::GetAllSeconds() const
{
    return (seconds + static_cast<int64_t>(60) * minutes + static_cast<long long>(3600) * hours);
}

double MTime::GetAllMinutes() const
{
    return (((double)seconds) / 60.0 + minutes + 60 * hours);
}

double MTime::GetAllHours() const
{
    return (((double)seconds) / 3600 + ((double)minutes) / 60 + hours);
}

CString MTime::GetNowTime() const
{
    CString NowTime, temp;
    temp.Format(_T("%d"), hours);
    NowTime += temp + CString(":");
    temp.Format(_T("%d"), minutes);
    NowTime += temp + CString(":");
    temp.Format(_T("%d"), seconds);
    NowTime += temp;
    return temp;
}

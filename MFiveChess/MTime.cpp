#include "pch.h"
#include "MTime.h"

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

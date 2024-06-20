#pragma once
class MTime
{
private:
	int minutes;
	int seconds;
	int hours;
public:
	int GetNowMinutes() const;
	int GetNowSeconds() const;
	int GetNowHours() const;
	int64_t GetAllSeconds() const;
	double GetAllMinutes() const;
	double GetAllHours() const;
	CString GetNowTime() const;
};


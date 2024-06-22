#pragma once
class MTime
{
private:
	int minutes;
	int seconds;
	int hours;
public:
    MTime(int hours = 0, int minutes = 0, int seconds = 0);
    void SetNowTime(int seconds = 0, int minutes = 0, int hours = 0);
    bool SubSeconds(int seconds);
	int GetNowMinutes() const;
	int GetNowSeconds() const;
	int GetNowHours() const;
	int64_t GetAllSeconds() const;
	double GetAllMinutes() const;
	double GetAllHours() const;
	CString GetNowTime() const;
};



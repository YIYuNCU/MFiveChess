#pragma once
class MTime
{
private:
	int minutes;
	int seconds;
	int hours;
public:
    /// <summary>
    /// 初始化MTime类
    /// </summary>
    /// <param name="hours">小时数(默认为0)</param>
    /// <param name="minutes">分钟数(默认为0)</param>
    /// <param name="seconds">秒数(默认为0)</param>
    MTime(int hours = 0, int minutes = 0, int seconds = 0);
    /// <summary>
    /// 设置当前Time
    /// </summary>
    /// <param name="seconds">秒数(默认为0)</param>
    /// <param name="minutes">分钟数(默认为0)</param>
    /// <param name="hours">小时数(默认为0)</param>
    void SetNowTime(int seconds = 0, int minutes = 0, int hours = 0);
    /// <summary>
    /// 减少秒数(秒数不够的话分钟数和小时数会自行变化)
    /// </summary>
    /// <param name="seconds">要减少的秒数</param>
    /// <returns>剩余的时间是否够减少</returns>
    bool SubSeconds(int seconds);
	/// <summary>
	/// 获取当前分钟数(仅分钟数)
	/// </summary>
	int GetNowMinutes() const;
	/// <summary>
	/// 获取当前秒数(仅秒数)
	/// </summary>
	int GetNowSeconds() const;
	/// <summary>
	/// 获取当前小时数(仅小时数)
	/// </summary>
	int GetNowHours() const;
	/// <summary>
	/// 获取将时间换算成秒后全部数量
	/// </summary>
	int64_t GetAllSeconds() const;
	/// <summary>
	/// 获取将时间换算为分钟后全部数量
	/// </summary>
	double GetAllMinutes() const;
	/// <summary>
	/// 获取将时间换算为小时后全部数量
	/// </summary>
	double GetAllHours() const;
	/// <summary>
	/// 获取转换为字符串后的时间
	/// </summary>
	CString GetNowTime() const;
};



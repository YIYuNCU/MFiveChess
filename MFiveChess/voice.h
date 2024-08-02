#pragma once
class Voice
{
public:
	/// <summary>
	/// 声音类初始化函数，用于初始化资源路径
	/// </summary>
	Voice();
	/// <summary>
	/// 播放声音函数（不阻塞线程）
	/// </summary>
	/// <param name="">.mp3路径</param>
	static void PlayVoice(const std::string&);
	/// <summary>
	/// 重复播放声音
	/// </summary>
	void Voice::PlayVoiceRepeat(const std::string& filename);
	/// <summary>
	/// 结束播放声音（搭配PlayVoice)
	/// </summary>
	static void EndVoice();
	/// <summary>
	/// 播放全部声音（阻塞线程）
	/// </summary>
	/// <param name="filename">路径</param>
	static void PlayAllMP3(const std::string& filename);
	//以下四个用于播放对应声音
	void PlayBlack();
	void PlayWhite();
	void PlayHuman();
	void PlayAI();
	void PlayBac();
	void PlayRep();
	void PlayBacByThread();
private:
	/// <summary>
	/// 黑棋落子资源保存路径
	/// </summary>
	std::string Black = "Resources\\Voice\\BlackVoice.mp3";
	/// <summary>
	/// 白棋落子资源保存路径
	/// </summary>
	std::string White = "Resources\\Voice\\WhiteVoice.mp3";
	/// <summary>
	/// 人输的音效音效资源
	/// </summary>
	std::string Human = "Resources\\Voice\\HumanLose.mp3";
	/// <summary>
	/// AI输的音效资源
	/// </summary>
	std::string AI = "Resources\\Voice\\AILose.mp3";
	/// <summary>
	/// 背景乐
	/// </summary>
	std::string Background = "Resources\\Voice\\Background.mp3";
	/// <summary>
	/// 悔棋音乐
	/// </summary>
	std::string Rep = "Resources\\Voice\\Rep.mp3";
	std::thread VoiceThread;
};


#include "pch.h"
#include "voice.h"
#include "ImageShow.h"

#pragma comment(lib, "winmm.lib")

Voice::Voice()
{
    ImageShow::GetPath(Black);
    ImageShow::GetPath(White);
    ImageShow::GetPath(Human);
    ImageShow::GetPath(AI);
}

void Voice::PlayVoice(const std::string& filename)
{
    // 构建播放命令字符串
    std::string command = "open \"" + filename + "\" type mpegvideo alias mp3";
    mciSendStringA(command.c_str(), NULL, 0, NULL);

    // 播放音频
    mciSendStringA("play mp3", NULL, 0, NULL);
}
void Voice::EndVoice()
{
    // 关闭音频
    mciSendStringA("close mp3", NULL, 0, NULL);
}

void Voice::PlayAllMP3(const std::string& filename) 
{
    // 构建播放命令字符串
    std::string command = "open \"" + filename + "\" type mpegvideo alias mp3";
    mciSendStringA(command.c_str(), NULL, 0, NULL);

    // 播放音频，并等待播放完毕
    mciSendStringA("play mp3 wait", NULL, 0, NULL);

    // 关闭音频
    mciSendStringA("close mp3", NULL, 0, NULL);
}
// 函数声明，用于在新线程中播放MP3音乐
DWORD WINAPI PlayMP3Thread(LPVOID lpParam);

void Voice::_PlayMP3Thread(const std::string& filename)
{
    // 创建新线程，用于播放音乐
    HANDLE hThread = CreateThread(NULL, 0, PlayMP3Thread, (LPVOID)filename.c_str(), 0, NULL);
    if (hThread == NULL) 
    {
        std::cerr << "Error creating thread." << std::endl;
        return;
    }
    // 关闭线程句柄，避免资源泄漏
    CloseHandle(hThread);
}

// 新线程函数，用于播放MP3音乐
DWORD WINAPI PlayMP3Thread(LPVOID lpParam) {
    std::string filename = (const char*)lpParam;

    // 构建打开命令字符串
    std::string openCommand = "open \"" + filename + "\" type mpegvideo alias mp3";
    mciSendStringA(openCommand.c_str(), NULL, 0, NULL);

    // 设置音频的音量
    std::string setVolumeCommand = "setaudio mp3 volume to 50";
    mciSendStringA(setVolumeCommand.c_str(), NULL, 0, NULL);

    // 播放音频，并等待播放完毕
    mciSendStringA("play mp3 wait", NULL, 0, NULL);

    // 关闭音频
    mciSendStringA("close mp3", NULL, 0, NULL);

    return 0;
}

void Voice::PlayBlack()
{
    PlayAllMP3(Black);
}

void Voice::PlayWhite()
{
    PlayAllMP3(White);
}

void Voice::PlayHuman()
{
    PlayVoice(Human);
}

void Voice::PlayAI()
{
    PlayVoice(AI);
}

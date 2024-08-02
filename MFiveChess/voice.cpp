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
    ImageShow::GetPath(Background);
    ImageShow::GetPath(Rep);
}

void Voice::PlayVoiceRepeat(const std::string& filename)
{
    // 构建播放命令字符串
    std::string command = "open \"" + filename + "\" type mpegvideo alias mp3";
    mciSendStringA(command.c_str(), NULL, 0, NULL);

    // 播放音频
    mciSendStringA("play mp3 repeat", NULL, 0, NULL);
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
void Voice::PlayBacByThread()
{
    VoiceThread = std::thread(&Voice::PlayBac, this);
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

void Voice::PlayBac()
{
    PlayVoiceRepeat(Background);
}

void Voice::PlayRep()
{
    PlayAllMP3(Rep);
}
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
    // �������������ַ���
    std::string command = "open \"" + filename + "\" type mpegvideo alias mp3";
    mciSendStringA(command.c_str(), NULL, 0, NULL);

    // ������Ƶ
    mciSendStringA("play mp3", NULL, 0, NULL);
}
void Voice::EndVoice()
{
    // �ر���Ƶ
    mciSendStringA("close mp3", NULL, 0, NULL);
}

void Voice::PlayAllMP3(const std::string& filename) 
{
    // �������������ַ���
    std::string command = "open \"" + filename + "\" type mpegvideo alias mp3";
    mciSendStringA(command.c_str(), NULL, 0, NULL);

    // ������Ƶ�����ȴ��������
    mciSendStringA("play mp3 wait", NULL, 0, NULL);

    // �ر���Ƶ
    mciSendStringA("close mp3", NULL, 0, NULL);
}
// �������������������߳��в���MP3����
DWORD WINAPI PlayMP3Thread(LPVOID lpParam);

void Voice::_PlayMP3Thread(const std::string& filename)
{
    // �������̣߳����ڲ�������
    HANDLE hThread = CreateThread(NULL, 0, PlayMP3Thread, (LPVOID)filename.c_str(), 0, NULL);
    if (hThread == NULL) 
    {
        std::cerr << "Error creating thread." << std::endl;
        return;
    }
    // �ر��߳̾����������Դй©
    CloseHandle(hThread);
}

// ���̺߳��������ڲ���MP3����
DWORD WINAPI PlayMP3Thread(LPVOID lpParam) {
    std::string filename = (const char*)lpParam;

    // �����������ַ���
    std::string openCommand = "open \"" + filename + "\" type mpegvideo alias mp3";
    mciSendStringA(openCommand.c_str(), NULL, 0, NULL);

    // ������Ƶ������
    std::string setVolumeCommand = "setaudio mp3 volume to 50";
    mciSendStringA(setVolumeCommand.c_str(), NULL, 0, NULL);

    // ������Ƶ�����ȴ��������
    mciSendStringA("play mp3 wait", NULL, 0, NULL);

    // �ر���Ƶ
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

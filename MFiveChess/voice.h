#pragma once
class Voice
{
public:
	Voice();
	static void PlayVoice(const std::string&);
	static void EndVoice();
	static void PlayAllMP3(const std::string& filename);
	void PlayBlack();
	void PlayWhite();
	void PlayHuman();
	void PlayAI();
	void _PlayMP3Thread(const std::string& filename);
private:
	std::string Black = "Voice\\BlackVoice.mp3";
	std::string White = "Voice\\WhiteVoice.mp3";
	std::string Human = "Voice\\HumanLose.mp3";
	std::string AI = "Voice\\AILose.mp3";
};


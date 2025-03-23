#pragma once

/*** Font�N���X ***/
class
{
public:
	int FH[30 + 1];

	void Read()
	{
		for (int i = 0; i < 30 + 1; i++)
		{
			FH[i] = CreateFontToHandle("�l�r�@�S�V�b�N", i, 6, DX_FONTTYPE_NORMAL);
		}
	}
private:

}Fon;

/*** Color�N���X ***/
class
{
public:
	int Black;
	int Red;
	int Green;
	int Blue;
	int White;

	void Read()
	{
		White = GetColor(255, 255, 255);
		Red = GetColor(255, 0, 0);
		Green = GetColor(0, 255, 0);
		Blue = GetColor(0, 0, 255);
		Black = GetColor(0, 0, 0);
	}
private:

}Col;

#ifdef DEF_SOUND_VALID
/*** Sound�N���X ***/
class
{
public:
	int BgmSound;
	int JumpSound;
	int BulletSound;
	int DamageSound;

	void Read()
	{
		BgmSound = LoadSoundMem("./sound/bgm.wav");
		JumpSound = LoadSoundMem("./sound/jump.wav");
		BulletSound = LoadSoundMem("./sound/bullet.wav");
		DamageSound = LoadSoundMem("./sound/damage.wav");
		ChangeVolumeSoundMem(255 * 20 / 100, BgmSound);		// ���ʂ�20%�ɒ���
		ChangeVolumeSoundMem(255 * 30 / 100, JumpSound);	// ���ʂ�20%�ɒ���
		ChangeVolumeSoundMem(255 * 30 / 100, BulletSound);	// ���ʂ�20%�ɒ���
		ChangeVolumeSoundMem(255 * 30 / 100, DamageSound);	// ���ʂ�20%�ɒ���
	}

	void PlayBGMSound()
	{
		PlaySoundMem(BgmSound, DX_PLAYTYPE_LOOP);
	}

	void PlayJumpSound()
	{
		PlaySoundMem(JumpSound, DX_PLAYTYPE_BACK);
	}
	void PlayBulletSound()
	{
		PlaySoundMem(BulletSound, DX_PLAYTYPE_BACK);
	}
	void PlayDamageSound()
	{
		PlaySoundMem(DamageSound, DX_PLAYTYPE_BACK);
	}
private:

}Snd;
#endif /* DEF_SOUND_VALID */

/*** FPS�N���X ***/
class
{
public:
	LONGLONG FirsttakeTime = 0;		// 1�t���[���ڂ̌v������
	LONGLONG NowtakeTime = 0;		// ���݂̌v������
	LONGLONG OldtakeTime = 0;		// �ȑO�̌v������

	float Deltatime = 0.000001f;	// �f���^�^�C���i�o�ߎ��ԁj
	int FrameCount = 1;				// ���݂̃t���[�����i1�t���[���ڂ���MAX�t���[���ڂ܂Łj
	float Average = 0.0f;			// ���ς�FPS�l

	void FPSInit() {
		FirsttakeTime = GetNowHiPerformanceCount();

		NowtakeTime = FirsttakeTime;
		OldtakeTime = FirsttakeTime;
		Deltatime = 0.000001f;
		FrameCount = 1;
		Average = 0.0f;

		return;
	}

	void FPSCheck() {
		NowtakeTime = GetNowHiPerformanceCount();
		Deltatime = (NowtakeTime - OldtakeTime) / MicroSecond;
		OldtakeTime = NowtakeTime;

		if (FrameCount == GameFPS)
		{
			LONGLONG TotalFrameTIme = NowtakeTime - FirsttakeTime;
			float CalcAverage = static_cast<float>(TotalFrameTIme) / GameFPS;
			Average = MicroSecond / CalcAverage;
			FirsttakeTime = GetNowHiPerformanceCount();
			FrameCount = 1;
		}
		else
		{
			FrameCount++;
		}
		return;
	}

	void FPSWait() {
		int wait = 0;
		wait = static_cast<int>(((MicroSecond / GameFPS * FrameCount) - (NowtakeTime - FirsttakeTime)) / MillSecond);	/* wait����(msec) = ���z�̎��� - ���ۂ̎��� */

		if (wait > 0 && wait <= WaitTimeMill) {
			WaitTimer(wait);
		}
		return;
	}
private:

}Fps;

/*** Key�N���X ***/
class
{
public:
	int input[256];		// �L�[�{�[�h���͏��

	int GetKey()
	{
		char allkey[256];
		GetHitKeyStateAll(allkey);
		for (int i = 0; i < 256; i++)
		{
			if (allkey[i] == 1) // ����̃L�[�͉�����Ă��邩
			{
				if (input[i] < DEF_KEY_PRESS_TIME) // ����������܂ŉ�����Ă��邩�ǂ���
				{
					input[i] = input[i] + 1; // �ۑ�
				}
			}
			else if (allkey[i] == 0) // ����̃L�[�͉�����Ă��Ȃ���
			{
				input[i] = 0;
			}
		}
		return 0;
	}
private:

}Key;

#ifdef DEF_JOYPAD_VALID
/*** Joypad�N���X ***/
class
{
public:
	DINPUT_JOYSTATE input;				// JoyPad���͏��
	unsigned char input_X_Z1 = 0;		// JoyPad�u�~�v�O��l

private:

}JPad;
#endif /* DEF_JOYPAD_VALID */

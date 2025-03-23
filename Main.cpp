/*** Header File ***/
#define _CRT_SECURE_NO_WARNINGS // C4996������

#include <algorithm>
#include<iostream>

#include "DxLib.h"
#include "ProjConf.h"
#include "Sub.h"
#include "Picture.h"
#include "Stage.h"
#include "Title.h"
#include "Action.h"
#include "Scene.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	
	ChangeWindowMode(TRUE);							// �E�B���h�E���[�h�ŋN��
	if (DxLib_Init() == -1)							// �c�w���C�u��������������
	{
		printf("DxLib�̏������Ɏ��s���܂����B\n");
		return -1;	// �G���[���N�����璼���ɏI��
	}

	/*** Window Init ***/
	SetWindowText("NinjaGame");					// �E�B���h�E�̃^�C�g��
	SetWindowInitPosition(WIN_POS_X, WIN_POS_Y);	// �E�B���h�E�̈ʒu
	SetGraphMode(WIN_MAX_X, WIN_MAX_Y, 32);			// �E�B���h�E�̃T�C�Y
	SetBackgroundColor(255, 255, 255);				// �E�B���h�E�̔w�i�F
	SetDrawScreen(DX_SCREEN_BACK);					// �`����ʂ𗠉�ʂɂ���
	SetAlwaysRunFlag(TRUE);							// �E�C���h�E��A�N�e�B�u��Ԃł������𑱍s����

	/*** FPS������ ***/
	Fps.FPSInit();

	/*** Read ***/
	Col.Read();
	Fon.Read();
	Pic.Read();
	Sta.Read();
#ifdef DEF_SOUND_VALID
	Snd.Read();

	/*** BGM�J�n ***/
	Snd.PlayBGMSound();
#endif /* DEF_SOUND_VALID */

	/*** ���[�v���� ***/
	while (ScreenFlip() == 0 &&		// ����ʂ̓��e��\��ʂɔ��f
		ClearDrawScreen() == 0 &&	// ��ʂ�������
		Key.GetKey() == 0 &&		// �L�[�{�[�h���͏��擾
#ifdef DEF_JOYPAD_VALID
		GetJoypadDirectInputState(DX_INPUT_KEY_PAD1, &JPad.input) == 0 && // JoyPad���͏��擾
#endif /* DEF_JOYPAD_VALID */
		ProcessMessage() == 0)		// �E�C���h�E�̃��b�Z�[�W������
	{
		/* FPS�v���J�n */
		Fps.FPSCheck();

		/* �V�[���o�� */
		Sce.Out();

		/* FPS�\�� */
		DrawFormatStringFToHandle(FPS_POS_X, FPS_POS_Y, Col.Black, Fon.FH[10], "FPS:%5.1f", Fps.Average);

#ifdef DEF_JOYPAD_VALID
		JPad.input_X_Z1 = JPad.input.Buttons[1]; // JoyPad�u�~�v�O��l�ۑ�
#endif /* DEF_JOYPAD_VALID */

		/* FPSWait */
		Fps.FPSWait();
	}

	WaitKey();						// �L�[���͑҂�

	DxLib_End();					// �c�w���C�u�����g�p�̏I������

	return 0;						// �\�t�g�̏I�� 
}

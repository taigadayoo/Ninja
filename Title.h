#pragma once

/*** Title�N���X ***/
class
{
public:
	int Out()
	{
		int ret = SCE::SCE_00_TIT;

		/*** �^�C�g����ʂ̕`�� ***/
		DrawGraph(0, 0, Pic.Title, TRUE);

		/*** Press Enter�̕`�� ***/
		DrawFormatStringFToHandle(TIT_ENTER_POS_X, TIT_ENTER_POS_Y, Col.Black, Fon.FH[30], "Press Enter");

		/*** �N���A�^�C���̕`�� ***/
		for (int i = 0; i < RANK_DISP_NUM; i++)
		{
			DrawFormatStringFToHandle(RANK_POS_X, static_cast<float>(RANK_POS_Y + i * 10), Col.Black, Fon.FH[10], "No.%d:%6.2f s", i + 1, Sta.Rank[i]);
		}

		/*** �A�N�V�����V�[���Ɉڍs ***/
		if ((Key.input[KEY_INPUT_RETURN] > 0)		// Enter����
#ifdef DEF_JOYPAD_VALID
			|| (JPad.input.Buttons[9] == 128)		// // JoyPad:�uoptions�v�{�^������
#endif /* DEF_JOYPAD_VALID */
			)
		{
			Sta.StartCount = GetNowCount(); // �J�n�J�E���g�ݒ�
			ret = SCE::SCE_01_ACT; // �A�N�V�����V�[���Ɉڍs
		}
		return ret;
	}

private:

}Tit;

#pragma once

/*** Scene�N���X ***/
class
{
public:
	/* �V�[���o�� */
	void Out()
	{
		switch (now)
		{
		case SCE::SCE_00_TIT:
			/* �^�C�g���V�[���o�� */
			now = Tit.Out();
			break;
		case SCE::SCE_01_ACT:
			/* �A�N�V�����V�[���o�� */
			now = Act.Out();
			break;
		default:
			break;
		}
	}

private:
	// ���݂̃V�[��
	int now = SCE::SCE_00_TIT;

}Sce;

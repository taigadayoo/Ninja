#pragma once

#define ON		1				// ON
#define OFF		0				// OFF

/*** �V�X�e���ݒ� ***/
#define DEF_SOUND_VALID			// �T�E���h�L��
//#define DEF_JOYPAD_VALID		// JoyPad�iDUALSHOCK4�j�L��

/*** �E�B���h�E�ݒ� ***/
#define WIN_MAX_X 660			// �E�B���h�E��X�ő�l
#define WIN_MAX_Y 450			// �E�B���h�E��Y�ő�l
#define WIN_MIN_X 0				// �E�B���h�E��X�ŏ��l
#define WIN_MIN_Y 0				// �E�B���h�E��Y�ŏ��l
#define WIN_POS_X 0				// �E�B���h�E�̏����ʒuX
#define WIN_POS_Y 0				// �E�B���h�E�̏����ʒuY

/*** �L�[�{�[�h�ݒ� ***/
#define DEF_KEY_PRESS_TIME 100	// �L�[�{�[�h��������

/*** FPS�ݒ� ***/
#define MicroSecond 1000000.0f	// 1�}�C�N���b
#define MillSecond 1000.0f		// 1�~���b
#define WaitTimeMill 3000		// �ő�ő҂Ă�~����
#define GameFPS 60				// �ݒ肵����FPS

/*** �V�[���ݒ� ***/
enum SCE						// �V�[�����
{
	SCE_00_TIT,					// �^�C�g���V�[��
	SCE_01_ACT,					// �A�N�V�����V�[��
	SCE_02_MAX
};

/*** �^�C�g���ݒ� ***/
#define TIT_ENTER_POS_X 220		// 'Press Enter' X�ʒu
#define TIT_ENTER_POS_Y 400		// 'Press Enter' Y�ʒu

/*** �X�e�[�W�ݒ� ***/
#define PIC_BACK 0								// Back
#define PIC_BLO1 1								// BLOCK1
#define PIC_BLO2 2								// BLOCK2
#define PIC_BLO3 3								// BLOCK3
#define PIC_BLO4 4								// BLOCK4
#define PIC_BLO5 5								// BLOCK5
#define PIC_MCHAR 6								// MChara
#define PIC_ENEM 7								// Enemy

#define CELL 30									// 1�u���b�N�̃T�C�Y[pix]
#define STG_X_MAX 6600							// �X�e�[�W�ő�X�ʒu[pix]
#define STG_Y_MAX 450							// �X�e�[�W�ő�Y�ʒu[pix]
#define STG_X_MIN 0								// �X�e�[�W�ŏ�X�ʒu[pix]
#define STG_Y_MIN 0								// �X�e�[�W�ŏ�Y�ʒu[pix]
#define STG_BLOCK_X_MAX (STG_X_MAX / CELL) 		// �X�e�[�W�ő�X�ʒu[�u���b�N]
#define STG_BLOCK_Y_MAX (STG_Y_MAX / CELL) 		// �X�e�[�W�ő�Y�ʒu[�u���b�N]
#define STG_FALL_Y (STG_Y_MAX - CELL)			// �X�e�[�W��������ʒu�i�X�e�[�W�ő�Y�ʒu-1CELL�j[pix]

#define CHA_POS_X_INI (STG_X_MIN + 2 * CELL)	// �L���������ʒuX
#define CHA_POS_Y_INI (STG_Y_MAX - 3 * CELL)	// �L���������ʒuY�i������3�u���b�N�ځj

#define ENEMY_HIT_RANGE	5						// �G�����蔻��臒l[pix]
#define ENE1_POS_X_INI	120						// �G1�����ʒuX
#define ENE1_POS_Y_INI	(STG_Y_MAX - 3 * CELL)	// �G1�����ʒuY�i������3�u���b�N�ځj
#define ENE2_POS_X_INI	360						// �G2�����ʒuX
#define ENE2_POS_Y_INI	(STG_Y_MAX - 3 * CELL)	// �G2�����ʒuY�i������3�u���b�N�ځj

#define DIR_NONE	0x00000000					// �ړ������Ȃ�
#define DIR_RI		0x00000001					// �E����
#define DIR_LE		0x00000002					// ������
#define DIR_UP		0x00000004					// �����
#define DIR_DO		0x00000008					// ������

#define JUMP_OFF		0						// �W�����v�Ȃ�
#define JUMP_UP			1						// �W�����v�㏸��
#define JUMP_GRAVITY	2						// ���R����

#define MOVEX		(CELL / 10)					// X�����ړ��ʁi�ʏ펞�j
#define MOVEX_D		(CELL / 5)					// X�����ړ��ʁi�_�b�V�����j
#define MOVEY_H_MAX	(4 * CELL)					// �W�����v���̍ő卂���i4�u���b�N�ځj

#define GOAL_POS_X (5400 - WIN_MAX_X / 2)		// GOAL X�ʒu

/*** �N���A�^�C�� ***/
enum RANK										// �V�[�����
{
	RANK_1ST,
	RANK_2ND,
	RANK_3RD,
	RANK_4TH,
	RANK_MAX
};
#define RANK_TIME_INI	100.0f					// �N���A�^�C�������l[s]
#define RANK_POS_X		(WIN_MAX_X - 90)		// �N���A�^�C���\���ʒuX
#define RANK_POS_Y		10						// �N���A�^�C���\���ʒuY
#define RANK_DISP_NUM	3						// �N���A�^�C���\����
#define TIME_POS_X		RANK_POS_X				// �^�C���\���ʒuX
#define TIME_POS_Y		(RANK_POS_Y + 30)		// �^�C���\���ʒuY

#define FPS_POS_X		10						// FPS�\���ʒuX
#define FPS_POS_Y		10						// FPS�\���ʒuY
#define COMD_POS_X		FPS_POS_X				// �R�}���h�����\���ʒuX
#define COMD_POS_Y		(FPS_POS_Y + 20)		// �R�}���h�����\���ʒuY

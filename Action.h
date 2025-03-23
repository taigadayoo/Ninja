#pragma once

/*** Action�N���X ***/
#pragma once
#include <cmath>
#include "DxLib.h"
#include <vector>
#include <random> 


/*** Action�N���X ***/
class ACTION
{
public:
    // ���˂���ʁi�e�j�̍ő吔
    static const int MAX_BULLETS = 10;

    // �e�̍\����
    struct BULLET {
        int X = 0;           // x���W
        int Y = 0;           // y���W
        int InitialX = 0;    // ���ˎ��̏���x���W
        int Dir = DIR_RI;    // �i�s�����iDIR_RI�܂���DIR_LE�j
        bool Active = false; // �g�p���t���O
    };

    // �G�L�����\����
    struct ENEMY {
        struct {
            int X = 0; // x���W
            int Y = 0; // y���W
        } Pos;
        int MinX = 0;    // �ړ��͈͂̍��[
        int MaxX = 0;    // �ړ��͈͂̉E�[
        int Dir = DIR_RI;    // �i�s�����iDIR_RI�܂���DIR_LE�j
        int Touch = DIR_NONE; // �ڐG����
        bool Alive = true;   // �������
        int Speed = 2;    // �G�̈ړ����x�i�Ǝ��̒l��ݒ�j
    };

    // ���C���L�����\����
    struct CHARACTER
    {
        struct {
            int X = CHA_POS_X_INI;
            int Y = CHA_POS_Y_INI;
            int Yin = CHA_POS_Y_INI;  // ���n�OY�����ʒu
        } Pos;
        struct {
            struct { int Ri = 0, Up = 0, Ce = 0; } RiUp;
            struct { int Le = 0, Up = 0, Ce = 0; } LeUp;
            struct { int Ri = 0, Do = 0, Ce = 0; } RiDo;
            struct { int Le = 0, Do = 0, Ce = 0; } LeDo;
        } Cor;
        int Dir = DIR_NONE;       // ���͂ɂ��ړ�����
        int PicDir = DIR_RI;      // �`��p����
        int Touch = DIR_NONE;     // �ڐG���Ă������
        int Fall = FALSE;         // �������
    };

    // �ړ��p�\����
    struct MOVEMENT
    {
        int X = MOVEX;          // X�����ړ���
        int Y = 0;              // Y�����ړ���
        int JumpState = JUMP_OFF;  // �W�����v���
        bool Dash = OFF;        // �_�b�V���t���O
    };

public:
    // �R���X�g���N�^
    ACTION() : jumpCount(0), maxJump(2)
    {
        Sta_PosX = STG_X_MIN;
        InitEnemies();
        MainChar.Pos.X = CHA_POS_X_INI;
        MainChar.Pos.Y = CHA_POS_Y_INI;
        MainChar.Pos.Yin = CHA_POS_Y_INI;
        MainChar.Dir = DIR_NONE;
        MainChar.PicDir = DIR_RI;
        MainChar.Touch = DIR_NONE;
        MainChar.Fall = FALSE;
        Mov.JumpState = JUMP_OFF;
        Mov.Y = 0;
        Cou = 0;



        Goal = FALSE;
        EndFlag = FALSE;

        for (int i = 0; i < MAX_BULLETS; i++) {
            Bullets[i].Active = false;
        }
    }

    // �X�V�����F���C���L�����̎l�p�`���W�����X�V
    void Update()
    {
        MainChar.Dir = DIR_NONE;
        MainChar.Touch = DIR_NONE;

        MainChar.Cor.RiUp.Ce = Sta.Cood.Pix[MainChar.Pos.X - Sta_PosX + CELL - 1][MainChar.Pos.Y];
        MainChar.Cor.RiDo.Ce = Sta.Cood.Pix[MainChar.Pos.X - Sta_PosX + CELL - 1][MainChar.Pos.Y + CELL - 1];
        MainChar.Cor.LeUp.Ce = Sta.Cood.Pix[MainChar.Pos.X - Sta_PosX][MainChar.Pos.Y];
        MainChar.Cor.LeDo.Ce = Sta.Cood.Pix[MainChar.Pos.X - Sta_PosX][MainChar.Pos.Y + CELL - 1];
    }

    // ���菈���F�ڐG�A���́A�e��Փ˔���i�W�����v�E���E�ړ��E�ʔ��˂Ȃǁj
    void Judge()
    {
        // �_�b�V������
        if ((Key.input[KEY_INPUT_LSHIFT] > 0)
#ifdef DEF_JOYPAD_VALID
            || (JPad.input.Buttons[0] == 128)
#endif
            )
        {
            Mov.Dash = ON;
            Mov.X = MOVEX_D;
        }
        else
        {
            Mov.Dash = OFF;
            Mov.X = MOVEX;
        }

        // ���E�̐ڐG�\���iX�����j
        MainChar.Cor.RiUp.Ri = Sta.Cood.Pix[MainChar.Pos.X - Sta_PosX + CELL - 1 + Mov.X][MainChar.Pos.Y];
        MainChar.Cor.RiDo.Ri = Sta.Cood.Pix[MainChar.Pos.X - Sta_PosX + CELL - 1 + Mov.X][MainChar.Pos.Y + CELL - 1];
        MainChar.Cor.LeUp.Le = Sta.Cood.Pix[MainChar.Pos.X - Sta_PosX - Mov.X][MainChar.Pos.Y];
        MainChar.Cor.LeDo.Le = Sta.Cood.Pix[MainChar.Pos.X - Sta_PosX - Mov.X][MainChar.Pos.Y + CELL - 1];

        // �㉺�̐ڐG�\���iY�����j
        MainChar.Cor.RiUp.Up = Sta.Cood.Pix[MainChar.Pos.X - Sta_PosX + CELL - 1][MainChar.Pos.Y - 1];
        MainChar.Cor.LeUp.Up = Sta.Cood.Pix[MainChar.Pos.X - Sta_PosX][MainChar.Pos.Y - 1];
        MainChar.Cor.RiDo.Do = Sta.Cood.Pix[MainChar.Pos.X - Sta_PosX + CELL - 1][MainChar.Pos.Y + CELL];
        MainChar.Cor.LeDo.Do = Sta.Cood.Pix[MainChar.Pos.X - Sta_PosX][MainChar.Pos.Y + CELL];

        if (MainChar.Cor.RiUp.Ri != PIC_BACK || MainChar.Cor.RiDo.Ri != PIC_BACK)
            MainChar.Touch |= DIR_RI;
        if (MainChar.Cor.LeUp.Le != PIC_BACK || MainChar.Cor.LeDo.Le != PIC_BACK)
            MainChar.Touch |= DIR_LE;
        if (MainChar.Cor.RiUp.Up != PIC_BACK || MainChar.Cor.LeUp.Up != PIC_BACK)
            MainChar.Touch |= DIR_UP;
        if (MainChar.Cor.RiDo.Do != PIC_BACK || MainChar.Cor.LeDo.Do != PIC_BACK)
            MainChar.Touch |= DIR_DO;

        // �ړ����͔���
        if ((Key.input[KEY_INPUT_SPACE] == 1 || Key.input[KEY_INPUT_UP] == 1)
#ifdef DEF_JOYPAD_VALID
            || ((JPad.input.Buttons[1] == 128) && (JPad.input_X_Z1 == 0))
#endif
            )
        {
            if ((Mov.JumpState == JUMP_OFF) && ((MainChar.Touch & DIR_DO) == DIR_DO))
            {
                Mov.JumpState = JUMP_UP;
                jumpCount = 1;
#ifdef DEF_SOUND_VALID
                Snd.PlayJumpSound();
#endif
            }
            // �󒆂ł̒ǉ��W�����v�i��F��i�W�����v�j
            else if (Mov.JumpState == JUMP_GRAVITY && jumpCount < maxJump)
            {
                Mov.JumpState = JUMP_UP;
                jumpCount++;
#ifdef DEF_SOUND_VALID
                Snd.PlayJumpSound();
#endif
            }
        }
        else if ((Key.input[KEY_INPUT_D] > 0 || Key.input[KEY_INPUT_RIGHT] > 0)
#ifdef DEF_JOYPAD_VALID
            || (JPad.input.POV[0] > 0 && JPad.input.POV[0] < 18000)
#endif
            )
        {
            MainChar.Dir = DIR_RI;
            MainChar.PicDir = DIR_RI;
        }
        else if ((Key.input[KEY_INPUT_A] > 0 || Key.input[KEY_INPUT_LEFT] > 0)
#ifdef DEF_JOYPAD_VALID
            || (JPad.input.POV[0] > 18000 && JPad.input.POV[0] < 36000)
#endif
            )
        {
            MainChar.Dir = DIR_LE;
            MainChar.PicDir = DIR_LE;
        }

        // �S�[������
        if (abs(Sta_PosX) > GOAL_POS_X)
        {
            Goal = TRUE;
        }

        // �I������iEsc�L�[�A�����A�G�Ƃ̐ڐG�A�S�[���Ȃǁj
        if ((Key.input[KEY_INPUT_ESCAPE] > 0) ||
            (MainChar.Fall == TRUE) ||

            (Goal == TRUE)
#ifdef DEF_JOYPAD_VALID
            || (JPad.input.Buttons[12] == 128)
#endif
            )
        {
            InitEnemies();
            EndFlag = TRUE;
        }

        // �ʁi�e�j���˔���i�X�y�[�X�L�[�j
        if (Key.input[KEY_INPUT_P] == 1) {
            for (int i = 0; i < MAX_BULLETS; i++) {
                if (!Bullets[i].Active) {
                    Snd.PlayBulletSound();
                    Bullets[i].Active = true;
                    Bullets[i].X = MainChar.Pos.X + CELL / 2;
                    Bullets[i].Y = MainChar.Pos.Y + CELL / 2;
                    Bullets[i].Dir = MainChar.PicDir; // �L�����̌����ɍ��킹�Ĕ���
                    Bullets[i].InitialX = Bullets[i].X; // ���ˎ��̏����ʒu���L�^
                    break;
                }
            }
        }
    }
    void CheckCollision() {
        // �v���C���[�ƓG�̓����蔻��
        for (auto& enemy : Enemies) {
            if (enemy.Alive) {
                if ((MainChar.Pos.X + CELL >= enemy.Pos.X) &&
                    (MainChar.Pos.X < enemy.Pos.X + CELL) &&
                    (MainChar.Pos.Y + CELL > enemy.Pos.Y) &&
                    (MainChar.Pos.Y < enemy.Pos.Y + CELL)) {
                    // �v���C���[���G�ɐڐG�����ꍇ�̏����i��: �_���[�W��Q�[���I�[�o�[�j
                    MainChar.Fall = TRUE; // �_���[�W��Ԃɂ���
                    InitEnemies();
                }
            }
        }

        // �ʁi�e�j�ƓG�L�����̓����蔻��
        for (auto& bullet : Bullets) {
            if (bullet.Active) {
                for (auto& enemy : Enemies) {
                    if (enemy.Alive &&
                        bullet.X >= enemy.Pos.X &&
                        bullet.X <= enemy.Pos.X + CELL &&
                        bullet.Y >= enemy.Pos.Y &&
                        bullet.Y <= enemy.Pos.Y + CELL) {
                        // �ʂƓG�̏Փˏ���
                        Snd.PlayDamageSound();
                        enemy.Alive = false;    // �G���A�N�e�B�u��
                        bullet.Active = false; // �ʂ��A�N�e�B�u��
                    }
                }
            }
        }
    }
    const int MAX_BULLET_DISTANCE = 250;
    /*** �ړ��v�Z ***/
    void Cal()
    {
        UpdateEnemies();   // �G�L�����̈ړ�����
        CheckCollision();
        SpawnEnemyNearPlayer();

        // X�����ړ��v�Z�i�L�����ړ�or�X�e�[�W�X�N���[���j
        if (MainChar.Dir == DIR_RI)
        {
            if ((MainChar.Touch & DIR_RI) != DIR_RI)
            {
                if (MainChar.Pos.X < WIN_MAX_X / 2)
                    MainChar.Pos.X += Mov.X;
                else if ((MainChar.Pos.X >= WIN_MAX_X / 2) && (abs(Sta_PosX) < STG_X_MAX - WIN_MAX_X))
                    Sta_PosX -= Mov.X;
            }
        }
        else if (MainChar.Dir == DIR_LE)
        {
            if ((MainChar.Touch & DIR_LE) != DIR_LE)
            {
                if (MainChar.Pos.X > STG_X_MIN)
                    MainChar.Pos.X -= Mov.X;
                else if ((MainChar.Pos.X <= STG_X_MIN) && (Sta_PosX < STG_X_MIN))
                    Sta_PosX += Mov.X;
            }
        }

        // Y�����ړ��v�Z�i�W�����v�㏸�^�d�͗����j
        if (Mov.JumpState == JUMP_UP)
        {
            Cou++;
            T = T_k * ((double)Cou / 60.0);
            Mov.Y = (int)(pow(T, 2.0));
            for (int y = 1; y <= Mov.Y; y++)
            {
                if ((MOVEY_H_MAX == MainChar.Pos.Yin - MainChar.Pos.Y) ||
                    (Sta.Cood.Pix[MainChar.Pos.X - Sta_PosX][MainChar.Pos.Y - 1] != PIC_BACK) ||
                    (Sta.Cood.Pix[MainChar.Pos.X - Sta_PosX + CELL - 1][MainChar.Pos.Y - 1] != PIC_BACK))
                {
                    Mov.JumpState = JUMP_GRAVITY;
                    Cou = 0;
                    break;
                }
                else
                {
                    MainChar.Pos.Y--;
                }
            }
        }
        else if ((Mov.JumpState == JUMP_GRAVITY) || ((MainChar.Touch & DIR_DO) != DIR_DO))
        {
            if (MainChar.Pos.Y < STG_FALL_Y)
            {
                Cou++;
                T = T_k * ((double)Cou / 60.0);
                Mov.Y = (int)(pow(T, 2.0));
                for (int y = 1; y <= Mov.Y; y++)
                {
                    if ((Sta.Cood.Pix[MainChar.Pos.X - Sta_PosX][MainChar.Pos.Y + CELL] != PIC_BACK) ||
                        (Sta.Cood.Pix[MainChar.Pos.X - Sta_PosX + CELL - 1][MainChar.Pos.Y + CELL] != PIC_BACK))
                    {
                        MainChar.Pos.Yin = MainChar.Pos.Y;
                        Mov.JumpState = JUMP_OFF;
                        jumpCount = 0;
                        Cou = 0;
                        break;
                    }
                    else
                    {
                        MainChar.Pos.Y++;
                    }
                }
            }
            else
            {
                MainChar.Fall = TRUE;
            }
        }

        // �ʁi�e�j�̍X�V����
        const int BULLET_SPEED = 8;
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (Bullets[i].Active) {
                // �e�̈ړ�
                if (Bullets[i].Dir == DIR_RI)
                    Bullets[i].X += BULLET_SPEED;
                else if (Bullets[i].Dir == DIR_LE)
                    Bullets[i].X -= BULLET_SPEED;

                // �򋗗����v�Z
                int distanceTravelled = abs(Bullets[i].X - Bullets[i].InitialX);

                // ��苗���𒴂������A�N�e�B�u��
                if (distanceTravelled > MAX_BULLET_DISTANCE) {
                    Bullets[i].Active = false;
                }
            }
        }
    }

    // ���C���L�����`��
    void Cha()
    {
        if (MainChar.PicDir == DIR_RI)
            DrawGraph(MainChar.Pos.X, MainChar.Pos.Y, Pic.MChara, TRUE);
        else if (MainChar.PicDir == DIR_LE)
            DrawTurnGraph(MainChar.Pos.X, MainChar.Pos.Y, Pic.MChara, TRUE);
    }

    void InitEnemies() {
        Enemies.clear();

        // �����_���G���W���̏���
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distSpeed(1, 3); // �G�̑��x�͈�

        for (int i = 0; i < 3; i++) { // 5�̂̓G�𐶐�
            ENEMY enemy;
            enemy.Pos.X = 300 + i * 150;   // �����ʒu
            enemy.Pos.Y = ENE1_POS_Y_INI; // ����Y���W
            enemy.MinX = 300 + i * 200 - 50; // �Œ�̈ړ��͈�
            enemy.MaxX = 300 + i * 200 + 50; // �Œ�̈ړ��͈�
            enemy.Dir = DIR_RI;
            enemy.Alive = true;
            enemy.Speed = distSpeed(gen); // �����_���ȑ��x��ݒ�
            Enemies.push_back(enemy);
        }
    }

    void UpdateEnemies() {
        // �����_���G���W���̏����i���������Ƒ��x�̃����_���͈͂�ݒ�j
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distRange(150, 500);  // ���������̃����_���͈�
        std::uniform_int_distribution<> distSpeed(1, 5);    // ���x�̃����_���͈�

        for (auto& enemy : Enemies) {
            if (enemy.Alive) {
                if (enemy.Dir == DIR_RI) { // �E�����ړ�
                    enemy.Pos.X += enemy.Speed;
                    if (enemy.Pos.X >= enemy.MaxX) {
                        // �������ɐ؂�ւ�
                        enemy.Dir = DIR_LE;

                        // �V�����ړ��͈͂𐶐�
                        int newRange = distRange(gen);
                        enemy.MinX = enemy.Pos.X - newRange;
                        enemy.MaxX = enemy.Pos.X;

                        // �V�������x�𐶐�
                        enemy.Speed = distSpeed(gen);
                    }
                }
                else if (enemy.Dir == DIR_LE) { // �������ړ�
                    enemy.Pos.X -= enemy.Speed;
                    if (enemy.Pos.X <= enemy.MinX) {
                        // �E�����ɐ؂�ւ�
                        enemy.Dir = DIR_RI;

                        // �V�����ړ��͈͂𐶐�
                        int newRange = distRange(gen);
                        enemy.MinX = enemy.Pos.X;
                        enemy.MaxX = enemy.Pos.X + newRange;

                        // �V�������x�𐶐�
                        enemy.Speed = distSpeed(gen);
                    }
                }
            }
        }
    }

    void SpawnEnemyNearPlayer() {
        // �����_���G���W���̏����iX���W�̃����_���͈͂�ݒ�j
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> xDist(MainChar.Pos.X - 150, MainChar.Pos.X + 150); // �v���C���[���͂̃����_���͈�

        if (spawnTimer >= SPAWN_INTERVAL) {
            ENEMY enemy;

            // ��苗�������܂Ń����_�����W���Đ���
            do {
                enemy.Pos.X = xDist(gen); // X���W�������_������
            } while (abs(enemy.Pos.X - MainChar.Pos.X) < 100); // �v���C���[����100px�ȏ㗣���

            enemy.Pos.Y = MainChar.Pos.Y;    // Y���W�̓v���C���[�Ɠ���
            enemy.MinX = enemy.Pos.X - 50;   // �ړ��͈́i�Œ�j
            enemy.MaxX = enemy.Pos.X + 50;   // �ړ��͈́i�Œ�j
            enemy.Dir = DIR_RI;              // �����������E
            enemy.Alive = true;

            // �����_���ȑ��x��ݒ�
            std::uniform_int_distribution<> distSpeed(1, 3);
            enemy.Speed = distSpeed(gen);

            // �G�����X�g�ɒǉ�
            Enemies.push_back(enemy);

            // �^�C�}�[�����Z�b�g
            spawnTimer = 0;
        }
        else {
            // �^�C�}�[��i�߂�
            spawnTimer++;
        }
    }






    // �G�L�����̕`��
    void DrawEnemies() {
        for (const auto& enemy : Enemies) {
            if (enemy.Alive) {
                DrawGraph(enemy.Pos.X, enemy.Pos.Y, Pic.Enemy1, TRUE); // �G�L�����摜��`��
            }
        }
    }
    // �ʁi�e�j�̕`��
    const int BULLET_WIDTH = 30;
    const int BULLET_HEIGHT = 30;

    void BulletDraw()
    {
        for (int i = 0; i < MAX_BULLETS; i++)
        {
            if (Bullets[i].Active)
            {
                DrawGraph(Bullets[i].X - BULLET_WIDTH / 2, Bullets[i].Y - BULLET_HEIGHT / 2, Pic.Bullet, TRUE);
            }
        }
    }

    // �\���n�`��i�^�C���A�R�}���h�������j
    void Disp()
    {
        for (int i = 0; i < RANK_DISP_NUM; i++)
        {
            DrawFormatStringFToHandle(RANK_POS_X, static_cast<float>(RANK_POS_Y + i * 10),
                Col.Black, Fon.FH[10], "No.%d:%6.2f s", i + 1, Sta.Rank[i]);
        }
        DrawFormatStringFToHandle(TIME_POS_X, TIME_POS_Y, Col.Black, Fon.FH[10],
            "Time:%6.2f s", static_cast<float>((GetNowCount() - Sta.StartCount) / MillSecond));

        DrawFormatStringFToHandle(COMD_POS_X, COMD_POS_Y, Col.Black, Fon.FH[10], "�E�ړ��F[D]");
        DrawFormatStringFToHandle(COMD_POS_X, COMD_POS_Y + 10, Col.Black, Fon.FH[10], "���ړ��F[A]");
        DrawFormatStringFToHandle(COMD_POS_X, COMD_POS_Y + 20, Col.Black, Fon.FH[10], "�W�����v�F[Space]");
        DrawFormatStringFToHandle(COMD_POS_X, COMD_POS_Y + 30, Col.Black, Fon.FH[10], "�_�b�V���F[��SHIFT]");
        DrawFormatStringFToHandle(COMD_POS_X, COMD_POS_Y + 40, Col.Black, Fon.FH[10], "Title�ɖ߂�F[Esc]");
        DrawFormatStringFToHandle(COMD_POS_X, COMD_POS_Y + 50, Col.Black, Fon.FH[10], "�藠���𓊂���F[P]");
    }

    // Act�V�[���I�����̏�����
    void EndInit()
    {
        Sta_PosX = STG_X_MIN;

        MainChar.Pos.X = CHA_POS_X_INI;
        MainChar.Pos.Y = CHA_POS_Y_INI;
        MainChar.Pos.Yin = CHA_POS_Y_INI;
        MainChar.Dir = DIR_NONE;
        MainChar.PicDir = DIR_RI;
        MainChar.Touch = DIR_NONE;
        MainChar.Fall = FALSE;
        Mov.JumpState = JUMP_OFF;
        Mov.Y = 0;
        Cou = 0;



        Goal = FALSE;
        EndFlag = FALSE;

        for (int i = 0; i < MAX_BULLETS; i++) {
            Bullets[i].Active = false;
        }
    }

    // Act�V�[���{����
    int Out()
    {
        int ret = SCE::SCE_01_ACT;

        Update();
        Judge();
        Cal();

        Sta.Out(&Sta_PosX);
        Cha();
        DrawEnemies();
        BulletDraw();
        Disp();

        if (EndFlag == TRUE)
        {
            if (Goal == TRUE)
                Sta.UpdateTime();
            EndInit();
            ret = SCE::SCE_00_TIT;
        }

        return ret;
    }

    // --- �����o�ϐ� ---

    CHARACTER MainChar;


    BULLET Bullets[MAX_BULLETS];

private:
    int Goal = FALSE;
    int EndFlag = FALSE;
    int Cou = 0;
    double T = 0.0;
    const double T_k = 20.0;
    std::vector<ENEMY> Enemies; // �����̓G���Ǘ����郊�X�g
    int Sta_PosX = STG_X_MIN;   // �X�e�[�W�ʒu�I�t�Z�b�g
    MOVEMENT Mov;
    int jumpCount;      // ���݂̃W�����v��
    const int maxJump;  // �ő�W�����v�񐔁i��F2�j
    int spawnTimer = 0; // �G�̃X�|�[���^�C�}�[
    const int SPAWN_INTERVAL = 150; // �X�|�[���Ԋu�i�t���[�����j
} Act;
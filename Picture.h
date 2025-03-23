#pragma once

/*** PictureƒNƒ‰ƒX ***/
class
{
public:
	int Back;
	int Block1;
	int Block2;
	int Block3;
	int Block4;
	int Block5;
	int MChara;
	int Enemy1;
	int Title;
	int Bullet;

	void Read()
	{
		Back = LoadGraph("./picture/Back.png");
		Block1 = LoadGraph("./picture/Block_1.png");
		Block2 = LoadGraph("./picture/Block_2.png");
		Block3 = LoadGraph("./picture/Block_3.png");
		Block4 = LoadGraph("./picture/Block_4.png");
		Block5 = LoadGraph("./picture/Block_5.png");
		MChara = LoadGraph("./picture/MainChara.png");
		Enemy1 = LoadGraph("./picture/Enemy_1.png");
		Title = LoadGraph("./picture/Title.png");
		Bullet = LoadGraph("./picture/Bullet.png");
	}

private:

}Pic;

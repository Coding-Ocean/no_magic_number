#include<fstream>
#include"libOne.h"
#include"DATA.h"

DATA d;

void create()
{
	//�f�[�^�ǂݍ���
	std::ifstream ifs("assets/data.bin");
	ifs.read((char*)&d, sizeof(DATA));
	ifs.close();
	
	//�^�C���Ɏ�������C���f�b�N�X�ǂݍ���
	d.numIndices = d.rows * d.cols;
	d.indices = new char[d.numIndices];
	ifs.open("assets/indices.bin");
	if (ifs)
	{
		ifs.read(d.indices, d.numIndices);
		ifs.close();
	}
	else 
	{
		for (int i = 0; i < d.numIndices; i++)
		{
			d.indices[i] = i;
		}
	}
}

void destroy()
{
	std::ofstream ofs("assets/indices.bin");
	ofs.write(d.indices, d.numIndices);
	ofs.close();

	delete[] d.indices;
}

void mouseOperation()
{
	//�}�E�X�|�C���^������^�C���̃C���f�b�N�X�B
	d.indexMouseOver = -1;

	//�}�E�X�|�C���^�����j���[�i�S�^�C���j�͈̔͊O�ɂ��鎞�A�֐��𔲂���
	if (mouseX<d.ofstX || mouseX > d.ofstX + d.tileW * d.cols ||
		mouseY<d.ofstY || mouseY > d.ofstY + d.tileH * d.rows)
	{
		d.indexMouseHolding = -1;
		return;
	}

	//�ȉ��}�E�X�|�C���^�����j���[�i�S�^�C���j�͈̔͂ɂ���---------------------
	
	//�}�E�X�|�C���^�̈ʒu�����ԍ��ƍs�ԍ��ɕϊ�
	int mouseCol = int((mouseX - d.ofstX) / d.tileW);
	int mouseRow = int((mouseY - d.ofstY) / d.tileH);
	//�P�����z��d.indices[]�̃C���f�b�N�X�ɕϊ�
	d.indexMouseOver = d.cols * mouseRow + mouseCol;
	
	//�N���b�N���ꂽ���̃C���f�b�N�X������Ă���
	if (isTrigger(MOUSE_LBUTTON))
	{
		d.indexMouseHolding = d.indexMouseOver;
	}

	//�}�E�X�|�C���^�{�^���𗣂����ʒu���󂯂Ă���A�Ƃ��Ă������C���f�b�N�X��}��
	if (d.indexMouseHolding != -1 &&
		isRelease(MOUSE_LBUTTON))
	{
		//�N���b�N���ꂽ�^�C�������C���f�b�N�X������Ă���
		int workIndex = d.indices[d.indexMouseHolding];
		//����̃C���f�b�N�X��O���Ɏ����Ă��������A���ɂ��炷
		if (d.indexMouseOver < d.indexMouseHolding)
		{
			for (int i = d.indexMouseHolding; i > d.indexMouseOver; i--)
			{
				d.indices[i] = d.indices[i - 1];
			}
		}
		//�O���̃C���f�b�N�X������Ɏ����Ă��������A�O�ɂ��炷
		else if (d.indexMouseOver > d.indexMouseHolding)
		{
			for (int i = d.indexMouseHolding; i < d.indexMouseOver; i++)
			{
				d.indices[i] = d.indices[i + 1];
			}
		}
		//��ł��炷���Ƃɂ��}���ꏊ���ł����̂œ����
		d.indices[d.indexMouseOver] = workIndex;
		
		//�����ɂ���
		d.indexMouseHolding = -1;
	}
}

void draw()
{
	clear();
	for (int r = 0; r < d.rows; r++)
	{
		for (int c = 0; c < d.cols; c++)
		{
			float px = d.ofstX + d.tileW * c;
			float py = d.ofstY + d.tileH * r;
			fill(d.backR, d.backG, d.backB);
			int i = d.cols * r + c;
			if (i == d.indexMouseOver) {
				fill(d.selectR, d.selectG, d.selectB);
			}
			rect(px, py, d.tileW, d.tileH);

			textSize(d.sizeText);
			fill(d.textR, d.textG, d.textB);
			textMode(TOP);
			text(d.indices[i], px, py);
		}
	}
	if (d.indexMouseHolding >= 0) {
		fill(d.holdTextR, d.holdTextG, d.holdTextB);
		textMode(BCENTER);
		text(d.indices[d.indexMouseHolding], mouseX, mouseY);
	}

	fill(255);
	print(d.indexMouseOver);
	print(d.indexMouseHolding);
}

void gmain()
{
	window(1920, 1080, full);
	create();
	while (notQuit)
	{
		mouseOperation();
		draw();
	}
	destroy();
}
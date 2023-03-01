#include<fstream>
#include"libOne.h"
#include"DATA.h"

DATA d;

void create()
{
	//データ読み込み
	std::ifstream ifs("assets/data.bin");
	ifs.read((char*)&d, sizeof(DATA));
	ifs.close();
	
	//タイルに持たせるインデックス読み込み
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
	//マウスポインタがあるタイルのインデックス。
	d.indexMouseOver = -1;

	//マウスポインタがメニュー（全タイル）の範囲外にある時、関数を抜ける
	if (mouseX<d.ofstX || mouseX > d.ofstX + d.tileW * d.cols ||
		mouseY<d.ofstY || mouseY > d.ofstY + d.tileH * d.rows)
	{
		d.indexMouseHolding = -1;
		return;
	}

	//以下マウスポインタがメニュー（全タイル）の範囲にある---------------------
	
	//マウスポインタの位置から列番号と行番号に変換
	int mouseCol = int((mouseX - d.ofstX) / d.tileW);
	int mouseRow = int((mouseY - d.ofstY) / d.tileH);
	//１次元配列d.indices[]のインデックスに変換
	d.indexMouseOver = d.cols * mouseRow + mouseCol;
	
	//クリックされた時のインデックスを取っておく
	if (isTrigger(MOUSE_LBUTTON))
	{
		d.indexMouseHolding = d.indexMouseOver;
	}

	//マウスポインタボタンを離した位置を空けてから、とっておいたインデックスを挿入
	if (d.indexMouseHolding != -1 &&
		isRelease(MOUSE_LBUTTON))
	{
		//クリックされたタイルが持つインデックスを取っておく
		int workIndex = d.indices[d.indexMouseHolding];
		//後方のインデックスを前方に持っていった時、後ろにずらす
		if (d.indexMouseOver < d.indexMouseHolding)
		{
			for (int i = d.indexMouseHolding; i > d.indexMouseOver; i--)
			{
				d.indices[i] = d.indices[i - 1];
			}
		}
		//前方のインデックスを後方に持っていった時、前にずらす
		else if (d.indexMouseOver > d.indexMouseHolding)
		{
			for (int i = d.indexMouseHolding; i < d.indexMouseOver; i++)
			{
				d.indices[i] = d.indices[i + 1];
			}
		}
		//上でずらすことにより挿入場所ができたので入れる
		d.indices[d.indexMouseOver] = workIndex;
		
		//無効にする
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
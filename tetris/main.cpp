#include <SFML/Graphics.hpp>

using namespace sf;

const int height = 20;
const int width = 10;

int map[height][width] = { 0 };

struct  Point
{
	int x, y;
}a[4], b[4];

int tetrominoes[7][4] =
{
	1,3,5,7, //I
	2,4,5,7, //Z
	3,5,4,6, //S
	3,5,4,7, //T
	2,3,5,7, //L
	3,5,7,6, //J
	2,3,4,5, //O
};

bool check()
{
	for (int i = 0; i < 4; i++)
		if(a[i].x < 0 || a[i].x >= width || a[i].y >= height) return 0;
		else if (map[a[i].y][a[i].x]) return 0;

	return 1;
}



int main()
{
	srand(time(0));

	RenderWindow window(VideoMode(480, 800), "*** TETRIS ***");

	Sprite colors[6];

	Texture t;
	t.loadFromFile("assets/blocks.png");
	Sprite s(t);
	s.setTextureRect(IntRect(0, 0, 32, 32));


	int dx = 0;
	bool rotate = false;
	int numOfColor = 1;
	float timer = 0;
	float delay = 0.3;

	int n = rand() % 7;
	for (int i = 0; i < 4; i++)
	{
		a[i].x = tetrominoes[n][i] % 2;
		a[i].y = tetrominoes[n][i] / 2;

	}

	Clock clock;


	while (window.isOpen())
	{
		
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == Event::KeyPressed)
			{
				if (event.key.code == Keyboard::Up) rotate = true;
				else if (event.key.code == Keyboard::Left) dx = -1;
				else if (event.key.code == Keyboard::Right) dx = 1;
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.05;

		//move

		for (int i = 0; i < 4; i++) { b[i] = a[i]; a[i].x += dx; }
		if (!check()) for (int i = 0; i < 4; i++) a[i] = b[i];

		//rotate

		if (rotate)
		{

			Point point = a[1]; //center of rotation
			for (int i = 0; i < 4; i++)
			{
				int x = a[i].y - point.y;
				int y = a[i].x - point.x;
				a[i].x = point.x - x;
				a[i].y = point.y + y;
			}

			if(!check()) for (int i = 0; i < 4; i++) a[i] = b[i];

		}

		if (timer > delay)
		{
			for (int i = 0; i < 4; i++) { b[i] = a[i]; a[i].y += 1; }

			if (!check()) {
				for (int i = 0; i < 4; i++) map[b[i].y][b[i].x] = numOfColor;

				numOfColor = 1 + rand() % 7;
				int n = rand() % 7;
				for (int i = 0; i < 4; i++)
				{
					a[i].x = tetrominoes[n][i] % 2;
					a[i].y = tetrominoes[n][i] / 2;

				}
			}

			timer = 0;
		}

		// check lines

		int k = height - 1;
		for (int i = height - 1; i > 0; i--)
		{
			int count = 0;
			for (int j = 0; j < width; j++)
			{
				if (map[i][j]) count++;
				map[k][j] = map[i][j];
			}
			if (count < width) k--;
		}




		

		//DRAW
		window.clear(Color::White);

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				if (map[i][j] == 0) continue;
				s.setTextureRect(IntRect(map[i][j]*32,0,32,32));
				s.setPosition(j * 32, i * 32);
				window.draw(s);
			}
				

		}

		for (int i = 0; i < 4; i++)
		{
			s.setTextureRect(IntRect(numOfColor * 32, 0, 32, 32));
			s.setPosition(a[i].x * 32, a[i].y * 32);
			window.draw(s);

		}


		window.display();

		//restart move and rotate
		dx = 0;
		rotate = 0;
		delay = 0.3;
	}

	return 0;
}
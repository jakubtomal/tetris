#include <SFML/Graphics.hpp>
#include <string>

using namespace sf ;

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


	//Graphic
	RenderWindow window(VideoMode(480, 700), "*** TETRIS ***");

	Font font;
	font.loadFromFile("assets/RacingNumbers.ttf");
	Text textPoints;
	textPoints.setFont(font);
	textPoints.setCharacterSize(50);
	textPoints.setFillColor(Color::Blue);
	textPoints.setOutlineColor(Color::Red);
	textPoints.setOutlineThickness(2);
	textPoints.setPosition(30, 635);
	Texture t , t2;
	t.loadFromFile("assets/blocks.png");
	t2.loadFromFile("assets/background.png");
	Sprite s(t),background(t2);
	s.setTextureRect(IntRect(0, 0, 32, 32));



	//Game Config
	int dx = 0;
	int points = 0;
	bool rotate = false;
	bool running = true;
	int numOfColor = 1;
	float timer = 0;
	float startDelay = 0.3;
	float currentDelay = startDelay;
	float delay = startDelay;

	int n = rand() % 7;
	for (int i = 0; i < 4; i++)
	{
		a[i].x = tetrominoes[n][i] % 2;
		a[i].y = tetrominoes[n][i] / 2;

	}

	Clock clock;


	while (window.isOpen())
	{

		textPoints.setString("Score "+std::to_string(points));
		
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
				else if (event.key.code == Keyboard::R)
				{
					for (int i = 0; i < height; i++)
					{
						for (int j = 0; j < width; j++)
						{
							
							map[i][j] = 0;
							for (int i = 0; i < 4; i++)
							{
								a[i].x = tetrominoes[n][i] % 2;
								a[i].y = tetrominoes[n][i] / 2;

							}
							points = 0;
							currentDelay = startDelay;
							running = true;
						}
					};
				}
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.05;

		//check for lose

		if (!check() && (a[0].y  = tetrominoes[n][0] / 2)) running = false;


		if (running)
		{
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

				if (!check()) for (int i = 0; i < 4; i++) a[i] = b[i];

			}

			if (timer > delay)
			{
				for (int i = 0; i < 4; i++) { b[i] = a[i]; a[i].y += 1; }

				if (!check()) {
					for (int i = 0; i < 4; i++) map[b[i].y][b[i].x] = numOfColor;

					numOfColor = 1 + rand() % 6;
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

			int k = height-1 ;
			for (int i = height-1 ; i > 0; i--)
			{
				int count = 0;
				for (int j = 0; j < width; j++)
				{
					if (map[i][j]) count++;
					map[k][j] = map[i][j];
				}
				if (count < width) k--;
				else
				{
					points++;
					if (currentDelay <= 0.1) currentDelay -= 0.01;
					
				}
			}
		}




		

		//DRAW
		window.clear(Color::White);
		window.draw(background);
		window.draw(textPoints);
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
		delay = currentDelay;
	}

	return 0;
}
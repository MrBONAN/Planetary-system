#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

#define G 0.66

using namespace sf; // подключаем пространство имен sf

class Planet
{
public:

	static int idCount; // для генерации id
	static int count; // количество планет
	static Planet* map; // общий список планет (для их взаимодействия)
	static RenderWindow* window;
	Planet()
	{
		this->circle = new CircleShape;
	}
	~Planet()
	{
		delete this->circle;
	}
	void Init(int x, int y, int rad, double mass, const Color& color, double dx = 0, double dy = 0)
	{
		this->x = x;
		this->y = y;
		this->mass = mass;
		this->dx = dx;
		this->dy = dy;

		// создание планеты
		this->circle->setRadius(rad);
		this->circle->setPosition(Vector2f(x - rad, y - rad));
		this->circle->setFillColor(color);
		this->id = idCount++;
	}

	void Show()
	{
		this->circle->move(Vector2f(this->dx, this->dy));
		this->window->draw(*(this->circle));
	}

	void Calc()
	{
		double F = 0, R2, a1, a2, sqR, Sin, Cos;
		for (int i = id + 1; i < count; i++) // Рассчитываем для всемирный закон тяготения для всех последующих в списке планет
		{
			Planet& other = map[i];

			this->x = this->circle->getPosition().x + this->circle->getRadius();
			this->y = this->circle->getPosition().y + this->circle->getRadius();
			other.x = other.circle->getPosition().x + other.circle->getRadius();
			other.y = other.circle->getPosition().y + other.circle->getRadius();

			double dx1 = other.x - this->x;
			double dy1 = other.y - this->y;
			R2 = pow(dx1, 2.0) + pow(dy1, 2.0);
			F = G * this->mass * other.mass / R2;
			// Оба этих ускорения по сути равны
			a1 = F / this->mass; // ускорение для текущего объекта
			a2 = F / other.mass; // ускорение для other объекта
			sqR = sqrt(R2);
			Sin = dy1 / sqR;
			Cos = dx1 / sqR;
			this->dx += Cos * a1;
			this->dy += Sin * a1;
			other.dx -= Cos * a2;
			other.dy -= Sin * a2;
			//std::cout << a1 << std::endl;
			//std::cout << a2 << std::endl;
		}
	}
	double x, y, dx, dy, mass;
private:
	//double x, y, dx, dy, mass;
	int id;
	CircleShape* circle;
};

int Planet::idCount = 0;
int Planet::count = 2;
Planet* Planet::map = new Planet[Planet::count];
RenderWindow* Planet::window = new RenderWindow;

int main()
{
	// Устанавливаем 8-й уровень сглаживания

	ContextSettings settings;
	settings.antialiasingLevel = 0;

	// Объект, который, собственно, является главным окном приложения
	RenderWindow window(VideoMode(800, 450), "Planet system", Style::Default, settings);
	Planet::window = &window;
	window.setFramerateLimit(120);
	//Planet m;
	//m.Init(100, 100, 50, 100.0, Color::Red);
	//m.Show();
	//Planet p1;
	//p1.Init(300.0, 150.0, 2, 600.0, Color::Blue);
	Planet p2;
	//p2.Init(390.0, 150.0, 2, 1.0, Color::Blue);
	double k = 1.5;
	double m = 100;
	/*Planet::map[0].Init(300.0, 150.0, 5, m, Color::Red, k, 0);
	Planet::map[1].Init(390.0, 150.0, 5, m, Color::Red, 0, k);
	Planet::map[2].Init(390.0, 240.0, 5, m, Color::Red, -k, 0);
	Planet::map[3].Init(300.0, 240.0, 5, m, Color::Red, 0, -k);*/
	if (1) {
		Planet::map[0].Init(300.0, 150.0, 50, 600, Color::Red);
		Planet::map[1].Init(390.0, 150.0, 5, 1, Color::Blue, 0, 2.1);
	}
	else {
		Planet::map[0].Init(300.0, 150.0, 30, 600, Color::Red, 0, -k);
		Planet::map[1].Init(390.0, 150.0, 30, 600, Color::Blue, 0, k);
	}
	/*Planet::map[0].Init(300.0, 150.0, 50, 600000, Color::Red);
	Planet::map[1].Init(380.0, 150.0, 5, 7, Color::Blue, 0, 0.005);*/

	// Главный цикл приложения: выполняется, пока открыто окно
	while (window.isOpen())
	{
		// Обрабатываем очередь событий в цикле
		Event event;
		while (window.pollEvent(event))
		{
			// Пользователь нажал на «крестик» и хочет закрыть окно?
			if (event.type == Event::Closed)
				window.close(); // тогда закрываем его
		}
		// Установка цвета фона
		window.clear(Color(250, 220, 100, 0));

		//Цикл обработки

		for (int i = 0; i < Planet::count; i++)
		{
			Planet::map[i].Show();
		}
		//p1.Show();
		//p2.Show();

		for (int i = 0; i < Planet::count; i++)
		{
			Planet::map[i].Calc();
		}


		// Отрисовка окна
		window.display();
	}
	delete[] Planet::map;

	return 0;
}
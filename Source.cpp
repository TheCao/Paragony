#include "ClassObraz.hpp" // klasa obraz
#include "MainFunc.hpp"
using namespace std;
using namespace cv;

//string input = "D:/test/paragony/test5.jpg";
//string input = "D:/test/test.png";
string input = "D:/test/obrobione2/Lokalna2/Lokalna2-31-8-240.jpg";

//string input = "D:/test/obrot.jpg";
extern obraz obrazek;

int main()
{
	obrazek.czy_zaznaczac = true;
	if (obrazek.inputWczytaj(input)) // Sprawdzenie poprawnoœci wczytania inputu
	{
		namedWindow("Obraz wczytany", CV_WINDOW_AUTOSIZE);
		//obrazek.obrot(obrazek.image_input, 20); // funkcja do obracania obrazu dziala okej!
		obrazek.inputResize(obrazek.image_input);


	}

	imshow("Obraz wczytany", obrazek.image_input_resized);
	if (obrazek.czy_zaznaczac == true)
	{
		Mat image_oznaczone;
		setMouseCallback("Obraz wczytany",mouse_callback , NULL);
		while (true)
		{
			while (obrazek.b_zaznaczenie == false)
			{
				waitKey(10);
			}
			image_oznaczone = obrazek.oznaczObszar(obrazek.image_input_resized, 
				obrazek.callback_box.x,
				obrazek.callback_box.y, 
				obrazek.callback_box.width, 
				obrazek.callback_box.height);
			imshow("Obraz wyciety", image_oznaczone);
			waitKey(1000);
			resize(image_oznaczone, image_oznaczone, 
				Size(0, 0), 
				1.6, 1.6, 
				CV_INTER_LINEAR); // jesli size 0 wtedy ustawiamy skalowanie przez fx i fy, inter linear najlepszy do skalowania w gore ;)
			obrazek.b_zaznaczenie = false;
			obrazek.tesseractSprawdzWszystko(image_oznaczone);
		}

	}


	cout << "Proces skonczony" << endl;

	waitKey(0);
	destroyAllWindows();
	return 0;

}

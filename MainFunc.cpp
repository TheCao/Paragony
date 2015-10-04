#include "MainFunc.hpp"
#include "ClassObraz.hpp"

#include <iostream>
#include <opencv2/highgui/highgui.hpp> 
#include "opencv2/imgproc/imgproc.hpp" 

using namespace std;
using namespace cv;
obraz obrazek;

//obs³uga myszy
void mouse_callback(int event, int x, int y, int flags, void* param){
	string zaznaczenie = "Zaznaczenie";
	switch (event){
	case CV_EVENT_MOUSEMOVE:
		if (obrazek.drawing_box)
		{
			obrazek.callback_box.width = x - obrazek.callback_box.x;
			obrazek.callback_box.height = y - obrazek.callback_box.y;
		}
		break;

	case CV_EVENT_LBUTTONDOWN:
		obrazek.drawing_box = true;
		obrazek.b_zaznaczenie = false;
		cout << "Zaznaczanie" << endl;
		obrazek.callback_box.x = x;
		obrazek.callback_box.y = y;
		break;

	case CV_EVENT_LBUTTONUP:
		obrazek.drawing_box = false;
		if (obrazek.callback_box.width < 0){
			obrazek.callback_box.x += obrazek.callback_box.width;
			obrazek.callback_box.width *= -1;
		}
		if (obrazek.callback_box.height < 0){
			obrazek.callback_box.y += obrazek.callback_box.height;
			obrazek.callback_box.height *= -1;
		}
		if (obrazek.callback_box.width > 0 && obrazek.callback_box.height > 0)
		{
			obrazek.licznik_zaznaczenia++;
			obrazek.b_zaznaczenie = true;
			zaznaczenie += " " + to_string(obrazek.licznik_zaznaczenia);
		}
		rectangle(obrazek.image_input_resized, obrazek.callback_box, Scalar(0, 255, 255), 3);
		putText(obrazek.image_input_resized, zaznaczenie, Point(obrazek.callback_box.x + 15 + obrazek.callback_box.width, obrazek.callback_box.y + obrazek.callback_box.height), FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 255, 255));
		cout << "Wymiary: " << endl << "X: " << obrazek.callback_box.x << endl << "Y: " << obrazek.callback_box.y << endl << "Width :" << obrazek.callback_box.width << endl << "Height : " << obrazek.callback_box.height << endl;
		imshow("Obraz wczytany", obrazek.image_input_resized);
		break;
	} // switch end


}
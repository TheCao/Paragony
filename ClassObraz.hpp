#pragma once
#include <iostream>
#include <opencv2/highgui/highgui.hpp> 
#include "opencv2/imgproc/imgproc.hpp" 


class obraz
{
private:
	

public:
	bool drawing_box; // czy rysujemy prostokat po znalezieniu wyrazu
	cv::Mat image_input_resized;
	cv::Mat image_input;
	bool czy_zaznaczac;
	bool b_zaznaczenie = false;
	CvRect callback_box = cvRect(0, 0, 0, 0); // wykorzystanie w callback 
	CvRect tesseract_box = cvRect(0, 0, 0, 0); // tymczasowa zmienna do testu tesseract
	std::string filepath; // zmienna wykorzystywana podczas funkcji binaryzacji 
	int licznik_zaznaczenia = 0;
	//FUNKCJE
	obraz();
	~obraz();

	//tesseract
	int tesseract_start(std::string input);
	int tesseractSprawdzWszystko(cv::Mat &sub);

	//obsluga obrazu
	int inputWczytaj(const std::string);
	cv::Mat inputResize(cv::Mat &image);
	double obliczObrot(cv::Mat &image);
	int obrot(cv::Mat &input, double angle);
	cv::Mat oznaczObszar(cv::Mat &image, int x, int y, int width, int height);
	int oznaczWyraz();
	int oznaczLitere();

	// metody binaryzacji 
	int binaryzacja_otsu(int border1 = 0, int border2 = 255);
	void histogram(const cv::Mat &img);
	int binaryzacja_histogram(int border1 = 0, int border2 = 255);
	int binaryzacja_lokalna1(int block_size = 11, double offset = 0.1, int border2 = 255);
	int binaryzacja_lokalna2(int block_size = 11, int iterator = 20, int border2 = 255);
	int binaryzacja_semi(int border1 = 0, int border2 = 255);

	
};


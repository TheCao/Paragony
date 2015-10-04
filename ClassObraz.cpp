#include "ClassObraz.hpp"
#include<baseapi.h>
#include <leptonica\allheaders.h>
#include <iostream>
#include <opencv2/highgui/highgui.hpp> 
#include "opencv2/imgproc/imgproc.hpp" 
#include <string>
#include <Windows.h>

using namespace std;
using namespace cv;
//testuje gita
obraz::obraz()
{
}


obraz::~obraz()
{
}

//TODO: tworzyæ obiekty typu CvRect za pomoca new.
int obraz::tesseract_start(string input)
{ // ta funkcja sprawdza calosc zaladowanego w argumencie pliku 
	// wywolanie np. tesseract_start("D:/test/obrobione2/Lokalna2/Lokalna2-33-8-240.jpg");
	Pix *image1 = pixRead(input.c_str());
	if (image1->data == NULL)
	{
		cout << "Blad: Bledna sciezka do pliku!" << endl;
		return -1;
	}
	//inicjalizacja tesseract
	tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
	if (api->Init(NULL, "eng")) {
		fprintf(stderr, "Could not initialize tesseract.\n");
		exit(1);
	}
	// start
	api->SetImage(image1);

	Boxa* boxes = api->GetComponentImages(tesseract::RIL_WORD, true, NULL, NULL);
	printf("Found %d textline image components.\n", boxes->n);
	for (int i = 0; i < boxes->n; i++) {
		Box* Tesseract_box_GetBox = boxaGetBox(boxes, i, L_CLONE);
		//api->SetRectangle(callback_box.x, callback_box.y, callback_box.width, callback_box.width);
		api->SetRectangle(Tesseract_box_GetBox->x, Tesseract_box_GetBox->y, Tesseract_box_GetBox->w, Tesseract_box_GetBox->h); // wybiera callback_box tekstu
		char* ocrResult = api->GetUTF8Text();
		int conf = api->MeanTextConf();
		// tymczasowe rysowanie boxa obrabianego przez tesseract
		tesseract_box.x = Tesseract_box_GetBox->x;
		tesseract_box.y = Tesseract_box_GetBox->y;
		tesseract_box.width = Tesseract_box_GetBox->w;
		tesseract_box.height = Tesseract_box_GetBox->h;
		rectangle(image_input_resized, tesseract_box, Scalar(0, 255, 255), 3);
		imshow("Obrazek", image_input_resized);
		fprintf(stdout, "callback_box[%d]: x=%d, y=%d, w=%d, h=%d, confidence: %d, text: %s",
			i, Tesseract_box_GetBox->x, Tesseract_box_GetBox->y, Tesseract_box_GetBox->w, Tesseract_box_GetBox->h, conf, ocrResult);
		waitKey(200);
	}
	cout << endl;
	api->End();
	pixDestroy(&image1);
	return 1;
}
int obraz::tesseractSprawdzWszystko(Mat &sub)
{ // ta funkcja sprawdza plik podany wewnetrznie 
	string zaznaczenie;
	tesseract::TessBaseAPI tess;
	tess.Init(NULL, "pol");
	tess.SetImage((uchar*)sub.data, sub.size().width, sub.size().height, sub.channels(), sub.step1()); // linijka do przekazywania Mat do tesseract 
	Boxa* boxes = tess.GetComponentImages(tesseract::RIL_WORD, true, NULL, NULL);
	printf("Found %d textline image components.\n", boxes->n);
	for (int i = 0; i < boxes->n; i++) {
		Box* Tesseract_box_GetBox = boxaGetBox(boxes, i, L_CLONE);
		//api->SetRectangle(callback_box.x, callback_box.y, callback_box.width, callback_box.width);
		tess.SetRectangle(Tesseract_box_GetBox->x, Tesseract_box_GetBox->y, Tesseract_box_GetBox->w, Tesseract_box_GetBox->h); // wybiera callback_box tekstu
		char* ocrResult = tess.GetUTF8Text();
		int conf = tess.MeanTextConf();
		// tymczasowe rysowanie boxa obrabianego przez tesseract
		tesseract_box.x = Tesseract_box_GetBox->x;
		tesseract_box.y = Tesseract_box_GetBox->y;
		tesseract_box.width = Tesseract_box_GetBox->w;
		tesseract_box.height = Tesseract_box_GetBox->h;
		rectangle(sub, tesseract_box, Scalar(255, 0, 5), 2);
		zaznaczenie = to_string(i);
		putText(sub, zaznaczenie, Point(tesseract_box.x + 2 + tesseract_box.width, tesseract_box.y + tesseract_box.height), FONT_HERSHEY_COMPLEX, 0.5, Scalar(255, 0, 0));
		imshow("Obraz wyciety", sub);
		fprintf(stdout, "callback_box[%d]: x=%d, y=%d, w=%d, h=%d, confidence: %d, text: %s",
			i, Tesseract_box_GetBox->x, Tesseract_box_GetBox->y, Tesseract_box_GetBox->w, Tesseract_box_GetBox->h, conf, ocrResult);
		waitKey(1000);
	}
	return 1;
}
int obraz::inputWczytaj(const string input)
{

	image_input = imread(input, 1);
	if (!image_input.data)                              // Check for invalid input
	{
		cout << "Blad: Bledna sciezka do pliku!" << endl;
		return -1;
	}

	return 1;
}
Mat obraz::inputResize(Mat &image)
{
	// input resize and display 
	resize(image, image_input_resized, cvSize(600, 600));
	//imshow("Obraz wczytany", image_input_resized);
	return image_input_resized;
}
Mat obraz::oznaczObszar(Mat &image, int x, int y, int width, int height)
{
	Mat image_cropped;
	// skalowanie zaznaczenia ( rozmiar, rozdzielczoœæ, binaryzacja )
	Rect zakres(x + 1, y + 1, width - 1, height - 1);
	image_cropped = image(zakres);
	return image_cropped;

}
double obraz::obliczObrot(Mat &image) // musze ustawic typ image na 8, jest 16. ( czyli CV_8UC3 ) a chcê CV_8UC2
{

	//cvCvtColor
	cout << "Typ:" << image.type() << endl;
	cvtColor(image, image, CV_RGB2GRAY, 1);
	cout << "Typ2:" << image.type() << endl;
	image.convertTo(image, CV_8UC4);
	cout << "Typ3:" << image.type() << endl;
	bitwise_not(image, image);
	Size size = image.size();
	std::vector<cv::Vec4i> lines;
	double angle = 0;
	HoughLines(image, lines, 1, CV_PI, 0);
	/*cv::Mat disp_lines(size, CV_8UC1, cv::Scalar(0, 0, 0));

	unsigned nb_lines = lines.size();
	for (unsigned i = 0; i < nb_lines; ++i)
	{
	cv::line(disp_lines, cv::Point(lines[i][0], lines[i][1]),
	cv::Point(lines[i][2], lines[i][3]), cv::Scalar(255, 0, 0));
	angle += atan2((double)lines[i][3] - lines[i][1],
	(double)lines[i][2] - lines[i][0]);
	cout << "Angle: " << angle << endl;
	}
	angle /= nb_lines; // mean angle, in radians.*/

	//	return angle;
	return 0;
}
int obraz::obrot(Mat &input, double angle)
{
	// Obrót ca³ego obrazu o zadany k¹t


	// wyznaczanie œrodka 
	cv::Point2f center(input.cols / 2.0, input.rows / 2.0);
	cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1.0);
	// determine bounding rectangle
	cv::Rect bbox = cv::RotatedRect(center, input.size(), angle).boundingRect();
	// ustawianie obrazu w œrodku 
	rot.at<double>(0, 2) += bbox.width / 2.0 - center.x;
	rot.at<double>(1, 2) += bbox.height / 2.0 - center.y;

	//cv::Mat dst;
	cv::warpAffine(input, input, rot, bbox.size());
	//cv::imshow("rotated_im.png", dst);
	return 1;

	// zrobic jeszcze na koniec wyciecie samego ( cropp ) 
}

// obróbka binaryzacyjna
int obraz::binaryzacja_otsu(int border1, int border2)
{
	// test binaryzacji otsu
	Mat image_otsu, image_otsu_binary;
	cvtColor(image_input, image_otsu, CV_BGR2GRAY);
	threshold(image_otsu, image_otsu_binary, border1, border2, THRESH_BINARY | THRESH_OTSU);
	cout << "Binaryzacja Otsu wykonana" << endl;
	string fileend = "/Otsu/Otsu-" + to_string(border1) + "-" + to_string(border2) + ".jpg";
	string filepath_local = filepath + fileend;
	std::string FilePath = "D:\\test\\obrobione2\\Otsu";
	CreateDirectoryA(FilePath.c_str(), NULL);
	imwrite(filepath_local, image_otsu_binary);
	cout << "Plik zapisany w : " << filepath_local << endl;
	return 1;
}
void obraz::histogram(const Mat &img){
	int histSize = 256;
	/// Set the ranges )
	float range[] = { 0, 256 };
	const float* histRange = { range };
	bool uniform = true;
	bool accumulate = false;
	Mat grey_hist;
	/// Compute the histograms:
	calcHist(&img, 1, 0, Mat(), grey_hist, 1, &histSize, &histRange, uniform, accumulate);
	// Draw the histograms 
	int hist_w = 256;
	int hist_h = 200;
	int bin_w = cvRound((double)hist_w / histSize);
	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
	/// Normalize the result to [ 0, histImage.rows ]
	normalize(grey_hist, grey_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	// Draw for each channel
	for (int i = 1; i < histSize; i++){
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(grey_hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(grey_hist.at<float>(i))),
			Scalar(120, 120, 120), 2, 8, 0);
	}
}
int obraz::binaryzacja_histogram(int border1, int border2)
{
	Mat image_histogram, image_histogram_binary;
	cvtColor(image_input, image_histogram, CV_BGR2GRAY);
	histogram(image_histogram);
	threshold(image_histogram, image_histogram_binary, border1, border2, THRESH_BINARY);
	string fileend = "/Histogram/Histogram-" + to_string(border1) + "-" + to_string(border2) + ".jpg";
	string filepath_local = filepath + fileend;
	std::string FilePath = "D:\\test\\obrobione2\\Histogram";
	CreateDirectoryA(FilePath.c_str(), NULL);
	imwrite(filepath_local, image_histogram_binary);
	cout << "Plik zapisany w : " << filepath_local << endl;
	return 1;
}
int obraz::binaryzacja_lokalna2(int block_size, int offset, int border2)
{
	Mat image_lokalna2, image_lokalna2_binary;
	cvtColor(image_input, image_lokalna2, CV_BGR2GRAY);
	adaptiveThreshold(image_lokalna2, image_lokalna2_binary, border2, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, block_size, offset);
	cout << "Binaryzacja Lokalna v2 wykonana" << endl;
	string fileend = "/Lokalna2/Lokalna2-" + to_string(block_size) + "-" + to_string(offset) + "-" + to_string(border2) + ".jpg";
	string filepath_local = filepath + fileend;
	std::string FilePath = "D:\\test\\obrobione2\\Lokalna2";
	CreateDirectoryA(FilePath.c_str(), NULL);
	imwrite(filepath_local, image_lokalna2_binary);
	cout << "Plik zapisany w : " << filepath_local << endl;
	return 1;
}
int obraz::binaryzacja_lokalna1(int block_size, double offset, int border2)
{
	Mat image_lokalna1, image_lokalna1_binary, image_lokalna1_gaussian;
	cvtColor(image_input, image_lokalna1, CV_BGR2GRAY);
	adaptiveThreshold(image_lokalna1, image_lokalna1_binary, border2, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, block_size, offset);
	adaptiveThreshold(image_lokalna1, image_lokalna1_gaussian, border2, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, block_size, offset);
	cout << "Binaryzacja lokalna1 wykonana" << endl;
	string fileend = "/Lokalna1/Lokalna1-Mean-" + to_string(block_size) + "-" + to_string(offset) + "-" + to_string(border2) + ".jpg";
	string filepath_local = filepath + fileend;
	std::string FilePath = "D:\\test\\obrobione2\\Lokalna1";
	CreateDirectoryA(FilePath.c_str(), NULL);
	imwrite(filepath_local, image_lokalna1_binary);
	cout << "Plik zapisany w : " << filepath_local << endl;
	fileend = "/Lokalna1/Lokalna1-Gauss-" + to_string(block_size) + "-" + to_string(offset) + "-" + to_string(border2) + ".jpg";
	filepath_local = filepath + fileend;
	imwrite(filepath_local, image_lokalna1_gaussian);
	cout << "Plik zapisany w : " << filepath_local << endl;
	return 1;
}
int obraz::binaryzacja_semi(int border1, int border2)
{
	Mat image_semi, image_semi_binary;
	cvtColor(image_input, image_semi, CV_BGR2GRAY);
	threshold(image_semi, image_semi_binary, border1, border2, THRESH_BINARY_INV);
	bitwise_and(image_semi, image_semi_binary, image_semi_binary);
	cout << "Binaryzacja Semi wykonana" << endl;
	string fileend = "/Semi/Semi-" + to_string(border1) + "-" + to_string(border2) + ".jpg";
	string filepath_local = filepath + fileend;
	std::string FilePath = "D:\\test\\obrobione2\\Semi";
	CreateDirectoryA(FilePath.c_str(), NULL);
	imwrite(filepath_local, image_semi_binary);
	cout << "Plik zapisany w : " << filepath_local << endl;
	return 1;
}


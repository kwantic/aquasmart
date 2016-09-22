#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>

using namespace cv;
using namespace std;

static void help()
{
	cout << "\nThis program demonstrates circle finding with the Hough transform.\n"
		"Usage:\n"
		"./houghcircles <image_name>, Default is ../data/board.jpg\n" << endl;
}

int main(int argc, char** argv)
{
	cv::CommandLineParser parser(argc, argv,
		"{help h ||}{@image|image.jpg|}"
	);
	if (parser.has("help"))
	{
		help();
		return 0;
	}
	//![load]
	string filename = parser.get<string>("@image");
	Mat img = imread(filename, IMREAD_COLOR);
	if (img.empty())
	{
		help();
		cout << "can not open " << filename << endl;
		return -1;
	}
	//![load]

	// Convert input image to HSV
	cv::Mat hsv_image;
	cv::cvtColor(img, hsv_image, cv::COLOR_BGR2HSV);

	//![reduce_noise]
	medianBlur(hsv_image, hsv_image, 3);
	//![reduce_noise]

	// Threshold the HSV image, keep only the white pixels
	cv::Mat lower_white_hue_range;
	//cv::Mat upper_red_hue_range;
	cv::inRange(hsv_image, cv::Scalar(0, 0, 200, 0), cv::Scalar(180, 255, 255, 0), lower_white_hue_range);
	//cv::inRange(hsv_image, cv::Scalar(0, 0, 200, 0), cv::Scalar(180, 255, 255, 0), upper_red_hue_range);

	// Combine the above two images
	//cv::Mat red_hue_image;
	//cv::addWeighted(lower_red_hue_range, 1.0, upper_red_hue_range, 1.0, 0.0, red_hue_image);

	//cv::GaussianBlur(lower_white_hue_range, lower_white_hue_range, cv::Size(9, 9), 2, 2);

	// Use the Hough transform to detect circles in the combined threshold image
	std::vector<cv::Vec3f> circles;
	cv::HoughCircles(lower_white_hue_range, circles, CV_HOUGH_GRADIENT, 2, lower_white_hue_range.rows / 256, 100, 8, 0, 4);


	// Loop over all detected circles and outline them on the original image
	if (circles.size() == 0) std::exit(-1);
	for (size_t current_circle = 0; current_circle < circles.size(); ++current_circle) {
		cv::Point center(std::round(circles[current_circle][0]), std::round(circles[current_circle][1]));
		int radius = std::round(circles[current_circle][2]);

		cv::circle(img, center, radius, cv::Scalar(0, 0, 255), 3, 8, 0);
	}

	// Show images
	cv::namedWindow("Threshold lower image", cv::WINDOW_AUTOSIZE);
	cv::imshow("Threshold lower image", lower_white_hue_range);
	//cv::namedWindow("Threshold upper image", cv::WINDOW_AUTOSIZE);
	//cv::imshow("Threshold upper image", upper_red_hue_range);
	//cv::namedWindow("Combined threshold images", cv::WINDOW_AUTOSIZE);
	//cv::imshow("Combined threshold images", red_hue_image);
	cv::namedWindow("Detected white circles on the input image", cv::WINDOW_AUTOSIZE);
	cv::imshow("Detected white circles on the input image", img);

	cv::waitKey(0);


	return 0;
}
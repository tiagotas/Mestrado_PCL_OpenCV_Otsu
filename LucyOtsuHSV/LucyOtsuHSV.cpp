// LucyOtsuHSV.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


pcl::PointCloud<pcl::PointXYZI>::Ptr cloudRGB2GRAY(pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud) {
	
	pcl::PointCloud<pcl::PointXYZI>::Ptr cloud_gray(new pcl::PointCloud<pcl::PointXYZI>);
	cloud_gray->height = cloud->height;
	cloud_gray->width = cloud->width;

	for (pcl::PointCloud<pcl::PointXYZRGB>::iterator it = cloud->begin(); it != cloud->end(); it++) {
		// Color conversion
		cv::Mat pixel(1, 1, CV_8UC3, cv::Scalar(it->r, it->g, it->b));
		cv::Mat temp;
		cv::cvtColor(pixel, temp, CV_RGB2GRAY);

		pcl::PointXYZI pointI;
		pointI.x = it->x;
		pointI.y = it->y;
		pointI.z = it->z;
		pointI.intensity = temp.at<uchar>(0, 0);

		cloud_gray->push_back(pointI);

	}
	return cloud_gray;
}

	pcl::PointCloud<pcl::PointXYZI>::Ptr cloudRGB2S(pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud) {
	pcl::PointCloud<pcl::PointXYZI>::Ptr cloud_gray(new pcl::PointCloud<pcl::PointXYZI>);
	cloud_gray->height = cloud->height;
	cloud_gray->width = cloud->width;

	for (pcl::PointCloud<pcl::PointXYZRGB>::iterator it = cloud->begin(); it != cloud->end(); it++) {
		// Color conversion
		cv::Mat pixel(1, 1, CV_8UC3, cv::Scalar(it->r, it->g, it->b));
		cv::Mat temp;
		cv::cvtColor(pixel, temp, CV_RGB2HSV);

		pcl::PointXYZI pointI;
		pointI.x = it->x;
		pointI.y = it->y;
		pointI.z = it->z;
		pointI.intensity = temp.at<uchar>(0, 1);

		cloud_gray->push_back(pointI);

	}
	return cloud_gray;
}

int main(int argc, char** argv) {

	std::cout << "Cloud reading failed." << std::endl;

	pcl::PointCloud <pcl::PointXYZRGB>::Ptr cloud_orig(new pcl::PointCloud <pcl::PointXYZRGB>);
	pcl::PLYReader reader;
	if (reader.read("lucy.ply", *cloud_orig) == -1) {
		std::cout << "Cloud reading failed." << std::endl;
		return (-1);
	}

	pcl::PointCloud <pcl::PointXYZI>::Ptr cloud = cloudRGB2S(cloud_orig);

	cv::Mat gray_values(1, cloud->size(), CV_8U);
	cv::Mat temp;

	int counter = 0;
	for (pcl::PointCloud<pcl::PointXYZI>::iterator it = cloud->begin(); it != cloud->end(); it++) {
		gray_values.at<uchar>(0, counter) = it->intensity;
		counter++;
	}

	double thres_v = cv::threshold(gray_values, temp, 0, 255, CV_THRESH_OTSU);
	std::cout << "Otsu threshold value = " << thres_v << std::endl;

	for (pcl::PointCloud<pcl::PointXYZI>::iterator it = cloud->begin(); it != cloud->end(); it++) {
		float v = it->intensity;
		if (v < thres_v) { it->intensity = 0; }
		else { it->intensity = 255; }
	}

	pcl::io::savePLYFile("lucy_otsu.ply", *cloud);
	
	//pcl::visualization::CloudViewer viewer("Visualizer");
	//viewer.showCloud(cloud);
	//while (!viewer.wasStopped()) {}
	
	return (0);
}















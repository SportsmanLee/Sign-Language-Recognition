#include "stdafx.h"
#include "MySVM.h"
#include <fstream>
#include <iterator>

using namespace cv;

void MySVM::concatenateGt(vector< vector<float> > features)
{
	vector<float> insertVector;
	for (unsigned int i = 0; i < features.size(); ++i) {
		for (unsigned int j = 0; j < features[i].size(); ++j) {
			insertVector.push_back(features[i][j]);
		}
	}

	gtVectors.push_back(insertVector);
}

void MySVM::concatenateAll(vector< vector<float> > features)
{
	vector<float> insertVector;
	for (unsigned int i = 0; i < features.size(); ++i) {
		for (unsigned int j = 0; j < features[i].size(); ++j) {
			insertVector.push_back(features[i][j]);
		}
	}

	Allvectors.push_back(insertVector);
}

void MySVM::concatenateAllclasses(int classes)
{
	Allclasses.push_back(classes);
}

int MySVM::update_class()
{
	if(Allvectors.size()==0)
		now_class = 1;
	else
		now_class++;
	return now_class;
}

void MySVM::concatenateOther(vector< vector<float> > features)
{
	vector<float> insertVector;
	for (unsigned int i = 0; i < features.size(); ++i) {
		for (unsigned int j = 0; j < features[i].size(); ++j) {
			insertVector.push_back(features[i][j]);
		}
	}

	otherVectors.push_back(insertVector);
}

void MySVM::concatenateTest(vector< vector<float> > features)
{
	for (unsigned int i = 0; i < features.size(); ++i) {
		for (unsigned int j = 0; j < features[i].size(); ++j) {
			testVector.push_back(features[i][j]);
		}
	}
}

vector<float> MySVM::getTestVector()
{
	return testVector;
}

void MySVM::clearVectors()
{
	gtVectors.clear();
	otherVectors.clear();
}

void MySVM::trainSVM()
{
	// Set up training data
	Mat labels(gtVectors.size() + otherVectors.size(), 1, CV_32FC1, Scalar(0));
	for (int i = 0; i < labels.rows; ++i) {
		labels.at<float>(i, 0) = (i < (int)gtVectors.size()) ? 1.0 : -1.0;
	}

	Mat trainingData(gtVectors.size() + otherVectors.size(), gtVectors[0].size(), CV_32FC1);
	for (int i = 0; i < trainingData.rows; ++i) {
		for (int j = 0; j < trainingData.cols; ++j) {
			trainingData.at<float>(i, j) = (i < (int)gtVectors.size()) ? gtVectors[i][j] : otherVectors[i - gtVectors.size()][j];
		}
	}

	// Set up SVM's parameters
	CvSVMParams params;
	params.svm_type    = CvSVM::C_SVC;
	params.kernel_type = CvSVM::LINEAR;
	//params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);

	// Train the SVM
	CvSVM SVM;
	if (SVM.train_auto(trainingData, labels, Mat(), Mat(), params)) {
		SVM.save("svm_data_auto.xml");
	}
}

void MySVM::trainSVM_lda()
{
	vector<int> classes;
	// Set up training data
	Mat labels(Allvectors.size(), 1, CV_32FC1, Scalar(0));
	for (int i = 0; i < labels.rows; ++i) {
		labels.at<float>(i, 0) = Allclasses[i];

	}

	Mat trainingData(Allvectors.size(),  Allvectors[0].size(), CV_32FC1);
	//Mat trainingData(gtVectors.size() + otherVectors.size(),  7, CV_32FC1);
	for (int i = 0; i < trainingData.rows; ++i) {
		for (int j = 0; j < trainingData.cols; ++j) {
			trainingData.at<float>(i, j) = Allvectors[i][j];
		}
	}

	//read pca & lda
	string pca_file = "pca.yaml";
	FileStorage fs(pca_file, FileStorage::READ);

	PCA pca;

	fs["pca mean"] >> pca.mean;   // Read entire cv::Mat
	fs["pca eigenvalues"] >> pca.eigenvalues;   // Read entire cv::Mat
	fs["pca eigenvectors"] >> pca.eigenvectors;   // Read entire cv::Mat

	LDA lda;
	lda.load("lda.yaml");

	//test data
	transform = lda.project(pca.project(trainingData));

	transform.convertTo(transform, CV_32FC1);

	//===========debug=========
	/*
	//write transform to txt
	char filename[]="transform.txt";
	fstream fp;
	fp.open(filename, ios::out);//開啟檔案
	if(!fp){//如果開啟檔案失敗，fp為0；成功，fp為非0
	cout<<"Fail to open file: "<<filename<<endl;
	}
	cout<<"File Descriptor: "<<fp<<endl;
	for(int i = 0 ; i < transform.rows ; i++)
	{
	for(int j = 0 ; j < transform.cols ; j++)
	{
	fp<<transform.ptr<float>(i)[j] << '\t';//寫入字串
	}
	fp<<endl;
	}
	fp.close();//關閉檔案
	//write transform to txt
	char filename2[]="trainingdata.txt";
	fstream fp2;
	fp2.open(filename2, ios::out);//開啟檔案
	if(!fp2){//如果開啟檔案失敗，fp為0；成功，fp為非0
	cout<<"Fail to open file: "<<filename2<<endl;
	}
	cout<<"File Descriptor: "<<fp2<<endl;
	for(int i = 0 ; i < trainingData.rows ; i++)
	{
	for(int j = 0 ; j < trainingData.cols ; j++)
	{
	fp2<<trainingData.ptr<float>(i)[j] << '\t';//寫入字串
	}
	fp2<<endl;
	}


	fp2.close();//關閉檔案
	*/

	// Set up SVM's parameters
	CvSVMParams params;
	params.svm_type    = CvSVM::C_SVC;
	params.kernel_type = CvSVM::LINEAR;
	params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);

	// Train the SVM

	CvSVM SVM;
	if (SVM.train_auto(transform, labels, Mat(), Mat(), params)) {
		SVM.save("svm_lda.xml");
	}
	fs.release();
}

void MySVM::get_pca_lda()
{
	//Allvectors to mat

	Mat trainingData(Allvectors.size() ,  Allvectors[0].size(), CV_32FC1);
	for (int i = 0; i < trainingData.rows; ++i) {
		for (int j = 0; j < trainingData.cols; ++j) {
			trainingData.at<float>(i, j) = Allvectors[i][j];
		}
	}

	int class_num = now_class + 1;

	int N = trainingData.rows;
	int D = trainingData.cols;

	PCA pca(trainingData , Mat(), CV_PCA_DATA_AS_ROW ,0.95);
	//get lda transform matrix 
	LDA lda=LDA(pca.project(trainingData),Allclasses); 
	transform =lda.project( pca.project(trainingData));

	FileStorage fo("pca.yaml", FileStorage::WRITE);
	if(fo.isOpened()) {
		fo << "pca mean" << pca.mean;
		fo << "pca eigenvalues" << pca.eigenvalues;
		fo << "pca eigenvectors" << pca.eigenvectors;
	}
	fo.release();
	lda.save("lda.yaml");


	//===========debug=========
	if(Allclasses.size() == Allvectors.size())
	{
		//write transform to txt
		char filename[]="transform.txt";
		fstream fp;
		fp.open(filename, ios::out);//開啟檔案
		if(!fp){//如果開啟檔案失敗，fp為0；成功，fp為非0
			cout<<"Fail to open file: "<<filename<<endl;
		}
		cout<<"File Descriptor: "<<fp<<endl;
		for(int i = 0 ; i < transform.rows ; i++)
		{
			for(int j = 0 ; j < transform.cols ; j++)
			{
				fp<<transform.ptr<float>(i)[j] << '\t';//寫入字串
			}
			fp<<endl;
		}
		fp.close();//關閉檔案
		//write trainingData to txt
		char filename2[]="trainingdata.txt";
		fstream fp2;
		fp2.open(filename2, ios::out);//開啟檔案
		if(!fp2){//如果開啟檔案失敗，fp為0；成功，fp為非0
			cout<<"Fail to open file: "<<filename2<<endl;
		}
		cout<<"File Descriptor: "<<fp2<<endl;
		for(int i = 0 ; i < trainingData.rows ; i++)
		{
			for(int j = 0 ; j < trainingData.cols ; j++)
			{
				fp2<<trainingData.ptr<float>(i)[j] << '\t';//寫入字串
			}
			fp2<<endl;
		}


		fp2.close();//關閉檔案
	}
	k_means_lda(transform , 5);

	//===========debug end===============
}

float MySVM::testSVM()
{
	Mat testImage(1, testVector.size(), CV_32FC1);
	for (unsigned int i = 0; i < testVector.size(); ++i) {
		testImage.at<float>(0, i) = testVector[i];
	}
	CvSVM SVM;
	SVM.load(modelFile.c_str());

	return SVM.predict(testImage, false); // test result 
}

float MySVM::testSVM_lda()
{
	Mat testImage(1, testVector.size(), CV_32FC1);
	for (unsigned int i = 0; i < testVector.size(); ++i) {
		testImage.at<float>(0, i) = testVector[i];
	}




	


	//read pca & lda
	string pca_file = "pca.yaml";
	FileStorage fs(pca_file, FileStorage::READ);

	PCA pca;

	fs["pca mean"] >> pca.mean;   // Read entire cv::Mat
	fs["pca eigenvalues"] >> pca.eigenvalues;   // Read entire cv::Mat
	fs["pca eigenvectors"] >> pca.eigenvectors;   // Read entire cv::Mat

	LDA lda;
	lda.load("lda.yaml");

	//test data
	transform = lda.project(pca.project(testImage));

	transform.convertTo(transform, CV_32FC1);
	//k means
	//load 
	string kmeans_file = "kmeans.yaml";
	FileStorage fs_kmeans(kmeans_file, FileStorage::READ);
	Mat k_mean_centers;
	double min_distance=0.0;
	int min_index;
	fs_kmeans["means"] >> k_mean_centers;   // Read entire cv::Mat

	for(int i = 0 ; i < k_mean_centers.rows ; i++)
	{

		if(min_distance > distance(k_mean_centers , transform ,i) || min_distance == 0.0)
		{
			min_distance = distance(k_mean_centers , transform ,i);
			min_index = i;
		}
	}
	string kmeans_path = "group";
	string add ;
	stringstream ss(add);
	ss << min_index;
	kmeans_path.append(ss.str());
	kmeans_path.append(".xml");


	//============debug kmeans===========
	////write transform to txt
	//char filename[]="kmean_res.txt";
	//fstream fp;
	//fp.open(filename, ios::out);//開啟檔案
	//if(!fp){//如果開啟檔案失敗，fp為0；成功，fp為非0
	//	cout<<"Fail to open file: "<<filename<<endl;
	//}
	//cout<<"File Descriptor: "<<fp<<endl;
	//fp<<min_index << '\t';//寫入字串
	//fp.close();//關閉檔案
	//=====================================
	//write
	//	char filename[]="test.txt";
	//	fstream fp;
	//	fp.open(filename, ios::out);//開啟檔案
	//	for(int i = 0 ; i < transform.rows ; i++)
	//	{
	//		for(int j = 0 ; j < transform.cols ; j++)
	//		{
	//			fp<<transform.ptr<float>(i)[j] << '\t';//寫入字串
	//		}
	//		fp<<endl;
	//	}


	//	fp.close();//關閉檔案
	CvSVM SVM;
	SVM.load(kmeans_path.c_str());
	return SVM.predict(transform, false); // test result 

}
void MySVM::k_means(int k)
{
	//Allvectors to mat
	Mat trainingData(Allvectors.size() ,  Allvectors[0].size(), CV_32FC1);
	for (int i = 0; i < trainingData.rows; ++i) {
		for (int j = 0; j < trainingData.cols; ++j) {
			trainingData.at<float>(i, j) = Allvectors[i][j];
		}
	}
	//k means
	Mat clusters , centers;
	cv::kmeans(trainingData,k,clusters,TermCriteria(CV_TERMCRIT_ITER,10,5.0),10,KMEANS_PP_CENTERS , centers);  


	//==============debug=====================
	//write center to txt
	char filename[]="kmeans_center.txt";
	fstream fp;
	fp.open(filename, ios::out);//開啟檔案
	if(!fp){//如果開啟檔案失敗，fp為0；成功，fp為非0
		cout<<"Fail to open file: "<<filename<<endl;
	}
	cout<<"File Descriptor: "<<fp<<endl;
	for(int i = 0 ; i < centers.rows ; i++)
	{
		for(int j = 0 ; j < centers.cols ; j++)
		{
			fp<<centers.ptr<float>(i)[j] << '\t';//寫入字串
		}
		fp<<endl;
	}
	fp.close();//關閉檔案
	//write trainingData to txt
	char filename2[]="trainingdata.txt";
	fstream fp2;
	fp2.open(filename2, ios::out);//開啟檔案
	if(!fp2){//如果開啟檔案失敗，fp為0；成功，fp為非0
		cout<<"Fail to open file: "<<filename2<<endl;
	}
	cout<<"File Descriptor: "<<fp2<<endl;
	for(int i = 0 ; i < trainingData.rows ; i++)
	{
		for(int j = 0 ; j < trainingData.cols ; j++)
		{
			fp2<<trainingData.ptr<float>(i)[j] << '\t';//寫入字串
		}
		fp2<<endl;
	}


	fp2.close();//關閉檔案

	//write label to txt
	string file_name = "label";
	string add ;
	stringstream ss(add);
	ss << k;
	file_name.append(ss.str());
	file_name.append("_org.yaml");
	FileStorage fo(file_name, FileStorage::WRITE);
	if(fo.isOpened()) {
		fo << "label" << clusters;
	}
	fo.release();
}


void MySVM::k_means_lda(Mat trainingData , int k){
	//k means
	Mat clusters , centers;
	trainingData.convertTo(trainingData, CV_32FC1);
	cv::kmeans(trainingData,k,clusters,TermCriteria(CV_TERMCRIT_ITER,10,1.0),10,KMEANS_PP_CENTERS , centers);  


	//==============debug=====================
	//write center to txt
	FileStorage fo_kmean("kmeans.yaml", FileStorage::WRITE);
	if(fo_kmean.isOpened()) {
		fo_kmean << "means" << centers;
	}
	fo_kmean.release();


	//write trainingData to txt
	char filename2[]="trainingdata.txt";
	fstream fp2;
	fp2.open(filename2, ios::out);//開啟檔案
	if(!fp2){//如果開啟檔案失敗，fp為0；成功，fp為非0
		cout<<"Fail to open file: "<<filename2<<endl;
	}
	cout<<"File Descriptor: "<<fp2<<endl;
	for(int i = 0 ; i < trainingData.rows ; i++)
	{
		for(int j = 0 ; j < trainingData.cols ; j++)
		{
			fp2<<trainingData.ptr<float>(i)[j] << '\t';//寫入字串
		}
		fp2<<endl;
	}


	fp2.close();//關閉檔案

	//write label to txt
	string file_name = "label";
	string add ;
	stringstream ss(add);
	ss << k;
	file_name.append(ss.str());
	file_name.append("_lda.yaml");
	FileStorage fo(file_name, FileStorage::WRITE);
	if(fo.isOpened()) {
		fo << "label" << clusters;
	}
	fo.release();
}

double MySVM::distance(Mat center , Mat data , int index)
{
	center.convertTo(center,CV_64FC1);
	data.convertTo(data,CV_64FC1);
	Mat a = data.clone();
	Mat buff = center.row(index).clone();
	Mat result = abs(a-buff);

	double dis ;
	for(int i = 0 ; i < result.cols ; i++)
	{
		dis+=abs(result.ptr<double>(0)[i]);
	}
	//dis= dis *dis;
	return dis;
}
void MySVM::setModel(string filename)
{
	modelFile = filename;
}

void MySVM::clear_testVector()
{
	testVector.clear();
}


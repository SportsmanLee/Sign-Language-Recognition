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
	Mat labels(gtVectors.size() + otherVectors.size(), 1, CV_32FC1, Scalar(0));
	for (int i = 0; i < labels.rows; ++i) {
		labels.at<float>(i, 0) = (i < (int)gtVectors.size()) ? 0 : 1;
		if(labels.at<float>(i, 0)==0)
			classes.push_back(0);
		else
			classes.push_back(1);
		//labels.at<float>(i, 0) = (float)i;

	}

	Mat trainingData(gtVectors.size() + otherVectors.size(),  gtVectors[0].size(), CV_32FC1);
	//Mat trainingData(gtVectors.size() + otherVectors.size(),  7, CV_32FC1);
	for (int i = 0; i < trainingData.rows; ++i) {
		for (int j = 0; j < trainingData.cols; ++j) {
			trainingData.at<float>(i, j) = (i < (int)gtVectors.size()) ? gtVectors[i][j] : otherVectors[i - gtVectors.size()][j];
		}
	}
	
	int class_num = 2;

	int N = trainingData.rows;
    int D = trainingData.cols;
	PCA pca(trainingData , Mat(), CV_PCA_DATA_AS_ROW, N-class_num);
	//get lda transform matrix 
	LDA lda=LDA(pca.project(trainingData),classes , 1); 
	transform = lda.project(pca.project(trainingData));

	transform.convertTo(transform , CV_32FC1);

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

	//save pca & lda
	FileStorage fo("pca.yaml", FileStorage::WRITE);
				 if(fo.isOpened()) {
					 fo << "pca mean" << pca.mean;
					 fo << "pca eigenvalues" << pca.eigenvalues;
					 fo << "pca eigenvectors" << pca.eigenvectors;
				 }
	fo.release();
	lda.save("lda.yaml");
    // Set up SVM's parameters
    CvSVMParams params;
    params.svm_type    = CvSVM::C_SVC;
    params.kernel_type = CvSVM::LINEAR;
    params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);



    // Train the SVM

    CvSVM SVM;
    if (SVM.train_auto(transform, labels, Mat(), Mat(), params)) {
		SVM.save("svm_data_auto.xml");
	}
	
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

void MySVM::setModel(string filename)
{
	modelFile = filename;
}

void MySVM::clear_testVector()
{
	testVector.clear();
}
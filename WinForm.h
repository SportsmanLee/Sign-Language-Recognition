#pragma once

#include "MyCV.h"
#include "fourier.h"
#include "MySVM.h"
#include "dirent.h"
#include <fstream>
#include <msclr/marshal_cppstd.h>

extern MyCV w_opencv;
extern fourier w_fourier;
extern MySVM w_svm;

namespace CWinFormOpenCV {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace cv;

	/// <summary>
	/// WinForm 的摘要
	/// </summary>
	public ref class WinForm : public System::Windows::Forms::Form
	{
	public:
		WinForm(void)
		{
			InitializeComponent();
			//
			//TODO: 在此加入建構函式程式碼
			//
		}

	protected:
		/// <summary>
		/// 清除任何使用中的資源。
		/// </summary>
		~WinForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private: System::Windows::Forms::PictureBox^  originPictureBox;
	private: System::Windows::Forms::OpenFileDialog^  modelChooser;
	private: System::Windows::Forms::TextBox^  fileTextBox;
	private: System::Windows::Forms::TextBox^  modelTextBox;
	protected: 


	private: System::Windows::Forms::Button^  truthButton;
	private: System::Windows::Forms::Button^  falseButton;
	private: System::Windows::Forms::Button^  trainButton;
	private: System::Windows::Forms::Button^  testVideoButton;


	private: System::Windows::Forms::Button^  modelButton;
	private: System::Windows::Forms::Button^  txtButton;
	private: System::Windows::Forms::Button^  testImageButton;


	private:
		/// <summary>
		/// 設計工具所需的變數。
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// 此為設計工具支援所需的方法 - 請勿使用程式碼編輯器
		/// 修改這個方法的內容。
		/// </summary>
		void InitializeComponent(void)
		{
			this->originPictureBox = (gcnew System::Windows::Forms::PictureBox());
			this->modelChooser = (gcnew System::Windows::Forms::OpenFileDialog());
			this->fileTextBox = (gcnew System::Windows::Forms::TextBox());
			this->modelTextBox = (gcnew System::Windows::Forms::TextBox());
			this->truthButton = (gcnew System::Windows::Forms::Button());
			this->falseButton = (gcnew System::Windows::Forms::Button());
			this->trainButton = (gcnew System::Windows::Forms::Button());
			this->testVideoButton = (gcnew System::Windows::Forms::Button());
			this->modelButton = (gcnew System::Windows::Forms::Button());
			this->txtButton = (gcnew System::Windows::Forms::Button());
			this->testImageButton = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->originPictureBox))->BeginInit();
			this->SuspendLayout();
			// 
			// originPictureBox
			// 
			this->originPictureBox->Location = System::Drawing::Point(291, 65);
			this->originPictureBox->Name = L"originPictureBox";
			this->originPictureBox->Size = System::Drawing::Size(640, 360);
			this->originPictureBox->SizeMode = System::Windows::Forms::PictureBoxSizeMode::CenterImage;
			this->originPictureBox->TabIndex = 1;
			this->originPictureBox->TabStop = false;
			// 
			// modelChooser
			// 
			this->modelChooser->FileName = L"svm_data_auto.xml";
			this->modelChooser->Filter = L"XML檔案 (*.xml)|*.xml|所有檔案 (*.*)|*.*";
			this->modelChooser->FilterIndex = 0;
			this->modelChooser->Title = L"選取分類器";
			// 
			// fileTextBox
			// 
			this->fileTextBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(255)), 
				static_cast<System::Int32>(static_cast<System::Byte>(192)));
			this->fileTextBox->Location = System::Drawing::Point(12, 358);
			this->fileTextBox->Multiline = true;
			this->fileTextBox->Name = L"fileTextBox";
			this->fileTextBox->Size = System::Drawing::Size(273, 39);
			this->fileTextBox->TabIndex = 5;
			// 
			// modelTextBox
			// 
			this->modelTextBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(192)), static_cast<System::Int32>(static_cast<System::Byte>(255)), 
				static_cast<System::Int32>(static_cast<System::Byte>(192)));
			this->modelTextBox->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->modelTextBox->Font = (gcnew System::Drawing::Font(L"Consolas", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->modelTextBox->Location = System::Drawing::Point(291, 13);
			this->modelTextBox->Multiline = true;
			this->modelTextBox->Name = L"modelTextBox";
			this->modelTextBox->Size = System::Drawing::Size(639, 50);
			this->modelTextBox->TabIndex = 14;
			// 
			// truthButton
			// 
			this->truthButton->Font = (gcnew System::Drawing::Font(L"Consolas", 14.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->truthButton->Location = System::Drawing::Point(41, 23);
			this->truthButton->Name = L"truthButton";
			this->truthButton->Size = System::Drawing::Size(213, 33);
			this->truthButton->TabIndex = 9;
			this->truthButton->Text = L"Ground Truth";
			this->truthButton->UseVisualStyleBackColor = true;
			this->truthButton->Click += gcnew System::EventHandler(this, &WinForm::truthButton_Click);
			// 
			// falseButton
			// 
			this->falseButton->Font = (gcnew System::Drawing::Font(L"Consolas", 14.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->falseButton->Location = System::Drawing::Point(41, 65);
			this->falseButton->Name = L"falseButton";
			this->falseButton->Size = System::Drawing::Size(213, 33);
			this->falseButton->TabIndex = 10;
			this->falseButton->Text = L"Ground False";
			this->falseButton->UseVisualStyleBackColor = true;
			this->falseButton->Click += gcnew System::EventHandler(this, &WinForm::falseButton_Click);
			// 
			// trainButton
			// 
			this->trainButton->Enabled = false;
			this->trainButton->Font = (gcnew System::Drawing::Font(L"Consolas", 14.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->trainButton->Location = System::Drawing::Point(41, 116);
			this->trainButton->Name = L"trainButton";
			this->trainButton->Size = System::Drawing::Size(213, 33);
			this->trainButton->TabIndex = 11;
			this->trainButton->Text = L"Train";
			this->trainButton->UseVisualStyleBackColor = true;
			this->trainButton->Click += gcnew System::EventHandler(this, &WinForm::trainButton_Click);
			// 
			// testVideoButton
			// 
			this->testVideoButton->Enabled = false;
			this->testVideoButton->Font = (gcnew System::Drawing::Font(L"Consolas", 14.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->testVideoButton->Location = System::Drawing::Point(41, 267);
			this->testVideoButton->Name = L"testVideoButton";
			this->testVideoButton->Size = System::Drawing::Size(213, 33);
			this->testVideoButton->TabIndex = 12;
			this->testVideoButton->Text = L"Test Video";
			this->testVideoButton->UseVisualStyleBackColor = true;
			this->testVideoButton->Click += gcnew System::EventHandler(this, &WinForm::testVideoButton_Click);
			// 
			// modelButton
			// 
			this->modelButton->Font = (gcnew System::Drawing::Font(L"Consolas", 14.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->modelButton->Location = System::Drawing::Point(41, 215);
			this->modelButton->Name = L"modelButton";
			this->modelButton->Size = System::Drawing::Size(213, 33);
			this->modelButton->TabIndex = 13;
			this->modelButton->Text = L"Choose Model";
			this->modelButton->UseVisualStyleBackColor = true;
			this->modelButton->Click += gcnew System::EventHandler(this, &WinForm::modelButton_Click);
			// 
			// txtButton
			// 
			this->txtButton->Font = (gcnew System::Drawing::Font(L"Consolas", 14.25F, System::Drawing::FontStyle::Bold));
			this->txtButton->Location = System::Drawing::Point(41, 165);
			this->txtButton->Name = L"txtButton";
			this->txtButton->Size = System::Drawing::Size(213, 33);
			this->txtButton->TabIndex = 15;
			this->txtButton->Text = L"to .txt";
			this->txtButton->UseVisualStyleBackColor = true;
			this->txtButton->Click += gcnew System::EventHandler(this, &WinForm::txtButton_Click);
			// 
			// testImageButton
			// 
			this->testImageButton->Enabled = false;
			this->testImageButton->Font = (gcnew System::Drawing::Font(L"Consolas", 14.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->testImageButton->Location = System::Drawing::Point(41, 319);
			this->testImageButton->Name = L"testImageButton";
			this->testImageButton->Size = System::Drawing::Size(213, 33);
			this->testImageButton->TabIndex = 16;
			this->testImageButton->Text = L"Test Image";
			this->testImageButton->UseVisualStyleBackColor = true;
			this->testImageButton->Click += gcnew System::EventHandler(this, &WinForm::testVideoButton_Click);
			// 
			// WinForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(942, 437);
			this->Controls->Add(this->testImageButton);
			this->Controls->Add(this->txtButton);
			this->Controls->Add(this->modelButton);
			this->Controls->Add(this->testVideoButton);
			this->Controls->Add(this->trainButton);
			this->Controls->Add(this->falseButton);
			this->Controls->Add(this->truthButton);
			this->Controls->Add(this->fileTextBox);
			this->Controls->Add(this->modelTextBox);
			this->Controls->Add(this->originPictureBox);
			this->Name = L"WinForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"WinForm";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->originPictureBox))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: int getdir(std::string dir, vector<std::string> &files) {
				 DIR *dp;//創立資料夾指標
				 struct dirent *dirp;
				 if((dp = opendir(dir.c_str())) == NULL){
					 cout << "Error(" << errno << ") opening " << dir << endl;
					 return errno;
				 }
				 while((dirp = readdir(dp)) != NULL)
				 {//如果dirent指標非空
					 files.push_back(std::string(dirp->d_name));//將資料夾和檔案名放入vector
				 }
				 closedir(dp);//關閉資料夾指標
				 return 0;
			 }
	private: System::Void truthButton_Click(System::Object^  sender, System::EventArgs^  e) {
				 FolderBrowserDialog^ folderBrowserDiaglog = gcnew FolderBrowserDialog();
				 if (folderBrowserDiaglog->ShowDialog() == System::Windows::Forms::DialogResult::Cancel) {
					 return;
				 }
				 std::string path = msclr::interop::marshal_as<std::string>(folderBrowserDiaglog->SelectedPath);

				 vector<std::string> filenames = vector<std::string>();
				 getdir(path,filenames);

				 vector<std::string> all_files = vector<std::string>();
				 std::string message;
				 for(unsigned int i = 0; i < filenames.size(); i++)
				 {
					 if (filenames[i] == "." || filenames[i] == "..")
						 continue;
					 message = path + "\\" + filenames[i];
					 all_files.push_back(message);
				 }
				 
				 for (unsigned int i = 0; i < all_files.size(); ++i) {
					 w_opencv.readImage(all_files[i]);

					 /*int histSize = 16;
					 float range[] = { 0, 256 } ;
					 const float* histRange = { range };
					 w_opencv.calHistogram(histSize, histRange);*/

					 w_opencv.detectSIFT();

					 w_opencv.HuMoment();
					 std::vector<float> huVector = w_opencv.getHuVector();

					 w_fourier.image_process(w_opencv.getImage());

					 vector< vector<float> > features;

					 //features.push_back(w_opencv.getHistVector());
					 features.push_back(w_opencv.getHuVector());
					 features.push_back(w_opencv.getSiftVector());
					 features.push_back(w_fourier.get_vector());

					 w_svm.concatenateGt(features);

					 features.clear();
					 w_opencv.clear();
					 w_fourier.clear_vector();

					 System::String^ string = gcnew System::String(all_files[i].c_str());
					 fileTextBox->Text = string;
					 fileTextBox->Refresh();
				 }
				 w_svm.outputVector(0);
				 MessageBoxA(0, "跑完了!", "Ground Truth", MB_OK);
			 }
	private: System::Void falseButton_Click(System::Object^  sender, System::EventArgs^  e) {
				 FolderBrowserDialog^ folderBrowserDiaglog = gcnew FolderBrowserDialog();
				 if (folderBrowserDiaglog->ShowDialog() == System::Windows::Forms::DialogResult::Cancel) {
					 return;
				 }
				 std::string path = msclr::interop::marshal_as<std::string>(folderBrowserDiaglog->SelectedPath);

				 vector<std::string> filenames = vector<std::string>();
				 getdir(path,filenames);

				 vector<std::string> all_files = vector<std::string>();
				 std::string message;
				 for(unsigned int i = 0; i < filenames.size(); i++)
				 {
					 if (filenames[i] == "." || filenames[i] == "..")
						 continue;
					 message = path + "\\" + filenames[i];
					 all_files.push_back(message);
				 }

				 for (unsigned int i = 0; i < all_files.size(); ++i) {
					 w_opencv.readImage(all_files[i]);

					 /*int histSize = 16;
					 float range[] = { 0, 256 } ;
					 const float* histRange = { range };
					 w_opencv.calHistogram(histSize, histRange);*/

					 w_opencv.detectSIFT();

					 w_opencv.HuMoment();
					 std::vector<float> huVector = w_opencv.getHuVector();

					 w_fourier.image_process(w_opencv.getImage());

					 vector< vector<float> > features;

					 //features.push_back(w_opencv.getHistVector());
					 features.push_back(w_opencv.getHuVector());
					 features.push_back(w_opencv.getSiftVector());
					 features.push_back(w_fourier.get_vector());

					 w_svm.concatenateOther(features);

					 features.clear();
					 w_opencv.clear();
					 w_fourier.clear_vector();

					 System::String^ string = gcnew System::String(all_files[i].c_str());
					 fileTextBox->Text = string;
					 fileTextBox->Refresh();
				 }
				 w_svm.outputVector(1);
				 MessageBoxA(0, "跑完了!", "Ground False", MB_OK);

				 trainButton->Enabled = true;
			 }
	private: System::Void trainButton_Click(System::Object^  sender, System::EventArgs^  e) {
				 w_svm.trainSVM();
				 MessageBoxA(0, "跑完了!", "SVM", MB_OK);
			 }
	private: System::Void modelButton_Click(System::Object^  sender, System::EventArgs^  e) {
				 if (modelChooser->ShowDialog() == System::Windows::Forms::DialogResult::Cancel) {
					 return;
				 }
				 
				 std::string modelFile = msclr::interop::marshal_as<std::string>(modelChooser->FileName);

				 w_svm.setModel(modelFile);

				 modelTextBox->Text = gcnew System::String(modelFile.c_str());

				 testVideoButton->Enabled = true;
			 }
	private: System::Void testVideoButton_Click(System::Object^  sender, System::EventArgs^  e) {

				 //w_svm.setModel("svm_data_auto_a_v2.xml");

				 OpenFileDialog ^ openFileDialog1 = gcnew OpenFileDialog();
				 openFileDialog1->Filter = "Video Files (*.MTS,*.wmv,*.avi)|*.MTS*.wmv;*.avi;*.*";
				 openFileDialog1->Title = "開啟影片檔";

				 if (openFileDialog1->ShowDialog(this) == System::Windows::Forms::DialogResult::Cancel)   // 使用者沒有選檔案
					 return;

				 std::string file;	
				 file = msclr::interop::marshal_as<std::string>(openFileDialog1->FileName);
				 //MessageBoxA(0, file.c_str(), "File name", MB_OK);
				 VideoCapture cap(file);
				 if(!cap.isOpened()) {
					 MessageBoxA(0, "Cannot open the video!!", "Open file failed", MB_OK);
					 return ;
				 }

				 ofstream output("output.txt", ios::out);
				 int fn = 0;
				 Mat frame;

				 while(fn!=5) {
				     cap >> frame;
					 fn++;				 
				 }
				 w_opencv.readFrame(frame);
				 w_opencv.set_bg_frame();

				 while(1) {
					 
					 cap >> frame;
					 fn++;
 
					 // 每10個frame取1張
					 if(fn%10 != 0) {
						 continue;
					 }
					 if(frame.empty() || fn > 300)
						 break;

					 w_opencv.readFrame(frame);
					 w_opencv.img_preproc();

					 //===========display on window================
					 Bitmap^ testImage = w_opencv.getBitmap();
					 if (testImage->Width > originPictureBox->Width || testImage->Height > originPictureBox->Height) {
						 Bitmap^ resizeImage = gcnew Bitmap(testImage, originPictureBox->Size);
						 originPictureBox->Image = resizeImage;
					 }
					 else {
						 originPictureBox->Image = testImage;
					 }
					 originPictureBox->Refresh();
					 //================================
					 w_opencv.detectSIFT();

					 w_opencv.HuMoment();
					 std::vector<float> huVector = w_opencv.getHuVector();

					 w_fourier.image_process(w_opencv.getImage());

					 vector< vector<float> > features;

					 //features.push_back(w_opencv.getHistVector());
					 features.push_back(w_opencv.getHuVector());
					 features.push_back(w_opencv.getSiftVector());
					 features.push_back(w_fourier.get_vector());

					 w_svm.concatenateTest(features);
					 /*
					 std::string output_file = "feature_" + to_string(fn) + ".txt";
					 ofstream output(output_file, ios::out);

					 std::vector<float> all_feature = w_svm.getTestVector();
					 ostream_iterator<float> output_iterator(output, "\n");
					 copy(all_feature.begin(), all_feature.end(), output_iterator);
					 */
					 System::String^ string = gcnew System::String(std::to_string(fn).c_str());
					 fileTextBox->Text = string;
					 fileTextBox->Refresh();

					 float res = w_svm.testSVM();
					 //MessageBoxA(0, std::to_string(res).c_str(), "SVM", MB_OK);

					 char result[128];
					 sprintf(result, "%s\t%f\r\n", std::to_string(fn).c_str(), res);  

					 output << result;

					 features.clear();
					 w_opencv.clear();
					 w_fourier.clear_vector();
					 w_svm.clear_testVector();
				 } // end while(1)
				 output.close();
				 MessageBoxA(0, "跑完了!", "TEST", MB_OK);
			 }
	private: System::Void txtButton_Click(System::Object^  sender, System::EventArgs^  e) {
				 OpenFileDialog ^ openFileDialog1 = gcnew OpenFileDialog();
				 openFileDialog1->Filter = "Video Files (*.MTS,*.wmv,*.avi)|*.MTS*.wmv;*.avi;*.*";
				 openFileDialog1->Title = "開啟影片檔";

				 if (openFileDialog1->ShowDialog(this) == System::Windows::Forms::DialogResult::Cancel)   // 使用者沒有選檔案
					 return;

				 std::string file;	
				 file = msclr::interop::marshal_as<std::string>(openFileDialog1->FileName);
				 VideoCapture cap(file);
				 if(!cap.isOpened()) {
					 MessageBoxA(0, "Cannot open the video!!", "Open file failed", MB_OK);
					 return ;
				 }
				 //========================
				 int fn = 0;
				 Mat frame;
				 vector<float> outputVector;

				 fstream output("output.txt", ios::out);

				 while(fn != 3) {
				     cap >> frame;
					 fn++;
				 }
				 cv::resize(frame, frame, cv::Size(cvRound(frame.cols / 2.0), cvRound(frame.rows / 2.0)));
				 w_opencv.set_bg_frame();

				 while(1) {
					 
					 if(!cap.read(frame)) {
						return;
					 }
					 
					 // 每10個frame取1張
					 if(fn%10 != 0) {
						 continue;
					 }

					 w_opencv.readFrame(frame);
					 w_opencv.img_preproc();

					 //===========display on window================
					 Bitmap^ testImage = w_opencv.getBitmap();
					 if (testImage->Width > originPictureBox->Width || testImage->Height > originPictureBox->Height) {
						 Bitmap^ resizeImage = gcnew Bitmap(testImage, originPictureBox->Size);
						 originPictureBox->Image = resizeImage;
					 }
					 else {
						 originPictureBox->Image = testImage;
					 }
					 originPictureBox->Refresh();

					 System::String^ string = gcnew System::String(std::to_string(fn).c_str());
					 fileTextBox->Text = string;
					 fileTextBox->Refresh();
					 //==============================================
					 w_opencv.detectSIFT();

					 w_opencv.HuMoment();
					 std::vector<float> huVector = w_opencv.getHuVector();

					 w_fourier.image_process(w_opencv.getImage());

					 vector< vector<float> > features;

					 //features.push_back(w_opencv.getHistVector());
					 features.push_back(w_opencv.getHuVector());
					 features.push_back(w_opencv.getSiftVector());
					 features.push_back(w_fourier.get_vector());

					 w_svm.concatenateTest(features);

					 outputVector = w_svm.getTestVector();

					 for (unsigned int i = 0; i < outputVector.size(); ++i) {
						 output << outputVector[i] << " ";
					 }
					 output << endl;

					 features.clear();
					 w_opencv.clear();
					 w_fourier.clear_vector();
					 w_svm.clear_testVector();
					 outputVector.clear();
				 } // end while(1)

				 output.close();
				 MessageBoxA(0, "跑完了!", "TXT", MB_OK);
			 }
	private: System::Void testImageButton_Click(System::Object^  sender, System::EventArgs^  e) {
				 OpenFileDialog ^ openFileDialog1 = gcnew OpenFileDialog();
				 openFileDialog1->Filter = "Image File (*.jpg,*.bmp)|*.jpg;*.bmp;*.*";
				 openFileDialog1->Title = "開啟影像";

				 if (openFileDialog1->ShowDialog(this) == System::Windows::Forms::DialogResult::Cancel)   // 使用者沒有選檔案
					 return;

				 std::string file;	
				 file = msclr::interop::marshal_as<std::string>(openFileDialog1->FileName);
				 //MessageBoxA(0, file.c_str(), "File name", MB_OK);
				 w_opencv.readImage(file);

				 Bitmap^ testImage = w_opencv.getBitmap();
				 if (testImage->Width > originPictureBox->Width || testImage->Height > originPictureBox->Height) {
					 Bitmap^ resizeImage = gcnew Bitmap(testImage, originPictureBox->Size);
					 originPictureBox->Image = resizeImage;
				 }
				 else {
					 originPictureBox->Image = testImage;
				 }
				 originPictureBox->Refresh();

				 w_opencv.detectSIFT();

				 w_opencv.HuMoment();
				 std::vector<float> huVector = w_opencv.getHuVector();

				 w_fourier.image_process(w_opencv.getImage());

				 vector< vector<float> > features;

				 //features.push_back(w_opencv.getHistVector());
				 features.push_back(w_opencv.getHuVector());
				 features.push_back(w_opencv.getSiftVector());
				 features.push_back(w_fourier.get_vector());

				 w_svm.concatenateTest(features);

				 System::String^ string = gcnew System::String(file.c_str());
				 fileTextBox->Text = string;
				 fileTextBox->Refresh();

				 float res = w_svm.testSVM();
				 MessageBoxA(0, std::to_string(res).c_str(), "SVM", MB_OK);

				 features.clear();
				 w_opencv.clear();
				 w_fourier.clear_vector();
				 w_svm.clear_testVector();
			 }

};
}
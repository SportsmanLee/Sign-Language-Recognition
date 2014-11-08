#pragma once

#include "MyCV.h"
#include "fourier.h"
#include "MySVM.h"
#include "include\dirent.h"
#include <msclr/marshal_cppstd.h>
#include "include\opencv2\core\core.hpp"
#include "include\opencv2\highgui\highgui.hpp"

MyCV w_opencv;
fourier w_fourier;
MySVM w_svm;

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
	private: System::Windows::Forms::Button^  loadImageButton;
	private: System::Windows::Forms::PictureBox^  originPictureBox;
	protected: 

	private: System::Windows::Forms::OpenFileDialog^  fileChooser;



	private: System::Windows::Forms::TextBox^  huTextBox;



	private: System::Windows::Forms::Button^  featureButton;




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
			this->loadImageButton = (gcnew System::Windows::Forms::Button());
			this->originPictureBox = (gcnew System::Windows::Forms::PictureBox());
			this->fileChooser = (gcnew System::Windows::Forms::OpenFileDialog());
			this->huTextBox = (gcnew System::Windows::Forms::TextBox());
			this->featureButton = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->originPictureBox))->BeginInit();
			this->SuspendLayout();
			// 
			// loadImageButton
			// 
			this->loadImageButton->Font = (gcnew System::Drawing::Font(L"Consolas", 14.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->loadImageButton->Location = System::Drawing::Point(41, 63);
			this->loadImageButton->Name = L"loadImageButton";
			this->loadImageButton->Size = System::Drawing::Size(213, 37);
			this->loadImageButton->TabIndex = 0;
			this->loadImageButton->Text = L"Choose Folder";
			this->loadImageButton->UseVisualStyleBackColor = true;
			this->loadImageButton->Click += gcnew System::EventHandler(this, &WinForm::loadImageButton_Click);
			// 
			// originPictureBox
			// 
			this->originPictureBox->Location = System::Drawing::Point(291, 12);
			this->originPictureBox->Name = L"originPictureBox";
			this->originPictureBox->Size = System::Drawing::Size(640, 360);
			this->originPictureBox->SizeMode = System::Windows::Forms::PictureBoxSizeMode::CenterImage;
			this->originPictureBox->TabIndex = 1;
			this->originPictureBox->TabStop = false;
			// 
			// fileChooser
			// 
			this->fileChooser->FileName = L"fileChooser";
			this->fileChooser->Filter = L"點陣圖檔案 (*.bmp;*.dib)|*.bmp|JPEG (*.jpg;*.jpeg;*.jpe;*.jfif)|*.jpg|GIF (*.gif)|*.gi" 
				L"f|TIFF (*.tiff;*.tif)|*.tiff|PNG (*.png)|*.png|ICO (*.ico)|*.ico|所有檔案 (*.*)|*.*";
			this->fileChooser->FilterIndex = 7;
			this->fileChooser->Multiselect = true;
			this->fileChooser->Title = L"選取影像";
			// 
			// huTextBox
			// 
			this->huTextBox->Location = System::Drawing::Point(12, 205);
			this->huTextBox->Name = L"huTextBox";
			this->huTextBox->Size = System::Drawing::Size(273, 22);
			this->huTextBox->TabIndex = 5;
			this->huTextBox->Text = L"0";
			// 
			// featureButton
			// 
			this->featureButton->Font = (gcnew System::Drawing::Font(L"Consolas", 14.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->featureButton->Location = System::Drawing::Point(41, 131);
			this->featureButton->Name = L"featureButton";
			this->featureButton->Size = System::Drawing::Size(213, 33);
			this->featureButton->TabIndex = 9;
			this->featureButton->Text = L"Feature Extraction";
			this->featureButton->UseVisualStyleBackColor = true;
			this->featureButton->Click += gcnew System::EventHandler(this, &WinForm::featureButton_Click);
			// 
			// WinForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(942, 385);
			this->Controls->Add(this->featureButton);
			this->Controls->Add(this->huTextBox);
			this->Controls->Add(this->originPictureBox);
			this->Controls->Add(this->loadImageButton);
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
	private: System::Void loadImageButton_Click(System::Object^  sender, System::EventArgs^  e) {
				 vector<std::string> all_files = vector<std::string>();

				 FolderBrowserDialog^ folderBrowserDiaglog = gcnew FolderBrowserDialog();

				 folderBrowserDiaglog->ShowDialog();
				 std::string path = msclr::interop::marshal_as<std::string>(folderBrowserDiaglog->SelectedPath);

				 getdir(path,all_files);

				 string message;
				 for(unsigned int i = 0; i < all_files.size(); i++)
				 {
					 if (all_files[i] == "." || all_files[i] == "..")
						 continue;
					 message = path + "\\" + all_files[i];
					 w_opencv.setFiles(message);
				 }
			 }
	private: System::Void featureButton_Click(System::Object^  sender, System::EventArgs^  e) {
				 vector<std::string> all_files = w_opencv.getFiles();

				 for (unsigned int i = 0; i < all_files.size(); ++i) {
					 w_opencv.readImage(all_files[i]);

					 int histSize = 16;
					 float range[] = { 0, 256 } ;
					 const float* histRange = { range };
					 w_opencv.calHistogram(histSize, histRange);

					 w_opencv.detectSIFT();

					 w_opencv.HuMoment();
					 std::vector<double> huVector = w_opencv.getHuVector();
					 
					 /*huTextBox->Text = "";
					 for(int i=0; i<huVector.size(); i++)
					 {
						 huTextBox->Text += L"Hu Moment hu" + (i+1) + ": " + ToString()->Format("{0:0.000000000000}",huVector[i]) + "\r\n";
					 }*/

					 w_fourier.image_process(w_opencv.getImage());

					 vector< vector<double> > features;

					 features.push_back(w_opencv.getHistVector());
					 features.push_back(w_opencv.getHuVector());
					 features.push_back(w_opencv.getSiftVector());
					 features.push_back(w_fourier.get_vector());

					 /*int size = features[0].size() + features[1].size() + features[2].size() + features[3].size();
					 MessageBoxA(0, std::to_string(size).c_str(), "feature size", MB_OK);*/

					 w_svm.concatenateFeature(features);
					 
					 features.clear();
					 w_opencv.clearVectors();
					 w_fourier.clear_vector();
				 }
			 }
};
}
#pragma once

#include "MyCV.h"
#include "fourier.h"
#include "MySVM.h"
#include "dirent.h"
#include <fstream>
#include <sstream>
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
	private: System::Windows::Forms::Button^  videoTxtButton;

	private: System::Windows::Forms::Button^  testImageButton;
	private: System::Windows::Forms::Button^  txtTrainButton;
	private: System::Windows::Forms::Button^  imgTxtButton;
	private: System::Windows::Forms::Button^  bagWordButton;
	private: System::Windows::Forms::Button^  testBOWButton;
	private: System::Windows::Forms::Button^  chooseVocButton;
	private: System::Windows::Forms::GroupBox^  trainingGroupBox;
	private: System::Windows::Forms::GroupBox^  processingGroupBox;
	private: System::Windows::Forms::GroupBox^  testingGroupBox;



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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(WinForm::typeid));
			this->originPictureBox = (gcnew System::Windows::Forms::PictureBox());
			this->modelChooser = (gcnew System::Windows::Forms::OpenFileDialog());
			this->fileTextBox = (gcnew System::Windows::Forms::TextBox());
			this->modelTextBox = (gcnew System::Windows::Forms::TextBox());
			this->truthButton = (gcnew System::Windows::Forms::Button());
			this->falseButton = (gcnew System::Windows::Forms::Button());
			this->trainButton = (gcnew System::Windows::Forms::Button());
			this->testVideoButton = (gcnew System::Windows::Forms::Button());
			this->modelButton = (gcnew System::Windows::Forms::Button());
			this->videoTxtButton = (gcnew System::Windows::Forms::Button());
			this->testImageButton = (gcnew System::Windows::Forms::Button());
			this->txtTrainButton = (gcnew System::Windows::Forms::Button());
			this->imgTxtButton = (gcnew System::Windows::Forms::Button());
			this->bagWordButton = (gcnew System::Windows::Forms::Button());
			this->testBOWButton = (gcnew System::Windows::Forms::Button());
			this->chooseVocButton = (gcnew System::Windows::Forms::Button());
			this->trainingGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->processingGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->testingGroupBox = (gcnew System::Windows::Forms::GroupBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->originPictureBox))->BeginInit();
			this->trainingGroupBox->SuspendLayout();
			this->processingGroupBox->SuspendLayout();
			this->testingGroupBox->SuspendLayout();
			this->SuspendLayout();
			// 
			// originPictureBox
			// 
			this->originPictureBox->Enabled = false;
			this->originPictureBox->Location = System::Drawing::Point(7, 65);
			this->originPictureBox->Name = L"originPictureBox";
			this->originPictureBox->Size = System::Drawing::Size(739, 457);
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
			this->fileTextBox->Location = System::Drawing::Point(7, 9);
			this->fileTextBox->Multiline = true;
			this->fileTextBox->Name = L"fileTextBox";
			this->fileTextBox->Size = System::Drawing::Size(367, 50);
			this->fileTextBox->TabIndex = 5;
			// 
			// modelTextBox
			// 
			this->modelTextBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(192)), static_cast<System::Int32>(static_cast<System::Byte>(255)), 
				static_cast<System::Int32>(static_cast<System::Byte>(192)));
			this->modelTextBox->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->modelTextBox->Font = (gcnew System::Drawing::Font(L"Consolas", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->modelTextBox->Location = System::Drawing::Point(379, 9);
			this->modelTextBox->Multiline = true;
			this->modelTextBox->Name = L"modelTextBox";
			this->modelTextBox->Size = System::Drawing::Size(366, 50);
			this->modelTextBox->TabIndex = 14;
			// 
			// truthButton
			// 
			this->truthButton->BackColor = System::Drawing::Color::Salmon;
			this->truthButton->Cursor = System::Windows::Forms::Cursors::Hand;
			this->truthButton->Enabled = false;
			this->truthButton->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->truthButton->Font = (gcnew System::Drawing::Font(L"Consolas", 14.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->truthButton->Location = System::Drawing::Point(6, 122);
			this->truthButton->Name = L"truthButton";
			this->truthButton->Size = System::Drawing::Size(213, 33);
			this->truthButton->TabIndex = 9;
			this->truthButton->Text = L"Ground Truth";
			this->truthButton->UseVisualStyleBackColor = false;
			this->truthButton->Click += gcnew System::EventHandler(this, &WinForm::truthButton_Click);
			// 
			// falseButton
			// 
			this->falseButton->BackColor = System::Drawing::Color::Salmon;
			this->falseButton->Cursor = System::Windows::Forms::Cursors::Hand;
			this->falseButton->Enabled = false;
			this->falseButton->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->falseButton->Font = (gcnew System::Drawing::Font(L"Consolas", 14.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->falseButton->Location = System::Drawing::Point(6, 163);
			this->falseButton->Name = L"falseButton";
			this->falseButton->Size = System::Drawing::Size(213, 33);
			this->falseButton->TabIndex = 10;
			this->falseButton->Text = L"Ground False";
			this->falseButton->UseVisualStyleBackColor = false;
			this->falseButton->Click += gcnew System::EventHandler(this, &WinForm::falseButton_Click);
			// 
			// trainButton
			// 
			this->trainButton->BackColor = System::Drawing::Color::Salmon;
			this->trainButton->Cursor = System::Windows::Forms::Cursors::Hand;
			this->trainButton->Enabled = false;
			this->trainButton->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->trainButton->Font = (gcnew System::Drawing::Font(L"Consolas", 14.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->trainButton->Location = System::Drawing::Point(6, 204);
			this->trainButton->Name = L"trainButton";
			this->trainButton->Size = System::Drawing::Size(213, 33);
			this->trainButton->TabIndex = 11;
			this->trainButton->Text = L"Train";
			this->trainButton->UseVisualStyleBackColor = false;
			this->trainButton->Click += gcnew System::EventHandler(this, &WinForm::trainButton_Click);
			// 
			// testVideoButton
			// 
			this->testVideoButton->BackColor = System::Drawing::Color::LightSkyBlue;
			this->testVideoButton->Cursor = System::Windows::Forms::Cursors::Hand;
			this->testVideoButton->Enabled = false;
			this->testVideoButton->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->testVideoButton->Font = (gcnew System::Drawing::Font(L"Consolas", 14.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->testVideoButton->Location = System::Drawing::Point(6, 121);
			this->testVideoButton->Name = L"testVideoButton";
			this->testVideoButton->Size = System::Drawing::Size(213, 33);
			this->testVideoButton->TabIndex = 12;
			this->testVideoButton->Text = L"Test Video";
			this->testVideoButton->UseVisualStyleBackColor = false;
			this->testVideoButton->Click += gcnew System::EventHandler(this, &WinForm::testVideoButton_Click);
			// 
			// modelButton
			// 
			this->modelButton->BackColor = System::Drawing::Color::LightSkyBlue;
			this->modelButton->Cursor = System::Windows::Forms::Cursors::Hand;
			this->modelButton->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->modelButton->Font = (gcnew System::Drawing::Font(L"Consolas", 14.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->modelButton->Location = System::Drawing::Point(6, 39);
			this->modelButton->Name = L"modelButton";
			this->modelButton->Size = System::Drawing::Size(213, 33);
			this->modelButton->TabIndex = 13;
			this->modelButton->Text = L"Choose Model";
			this->modelButton->UseVisualStyleBackColor = false;
			this->modelButton->Click += gcnew System::EventHandler(this, &WinForm::modelButton_Click);
			// 
			// videoTxtButton
			// 
			this->videoTxtButton->BackColor = System::Drawing::Color::PaleGreen;
			this->videoTxtButton->Cursor = System::Windows::Forms::Cursors::Hand;
			this->videoTxtButton->Enabled = false;
			this->videoTxtButton->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->videoTxtButton->Font = (gcnew System::Drawing::Font(L"Consolas", 14.25F, System::Drawing::FontStyle::Bold));
			this->videoTxtButton->Location = System::Drawing::Point(7, 150);
			this->videoTxtButton->Name = L"videoTxtButton";
			this->videoTxtButton->Size = System::Drawing::Size(213, 33);
			this->videoTxtButton->TabIndex = 15;
			this->videoTxtButton->Text = L"Video to Text";
			this->videoTxtButton->UseVisualStyleBackColor = false;
			this->videoTxtButton->Click += gcnew System::EventHandler(this, &WinForm::videoTxtButton_Click);
			// 
			// testImageButton
			// 
			this->testImageButton->BackColor = System::Drawing::Color::LightSkyBlue;
			this->testImageButton->Cursor = System::Windows::Forms::Cursors::Hand;
			this->testImageButton->Enabled = false;
			this->testImageButton->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->testImageButton->Font = (gcnew System::Drawing::Font(L"Consolas", 14.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->testImageButton->Location = System::Drawing::Point(6, 203);
			this->testImageButton->Name = L"testImageButton";
			this->testImageButton->Size = System::Drawing::Size(213, 33);
			this->testImageButton->TabIndex = 16;
			this->testImageButton->Text = L"Test Image";
			this->testImageButton->UseVisualStyleBackColor = false;
			this->testImageButton->Click += gcnew System::EventHandler(this, &WinForm::testImageButton_Click);
			// 
			// txtTrainButton
			// 
			this->txtTrainButton->BackColor = System::Drawing::Color::Salmon;
			this->txtTrainButton->Cursor = System::Windows::Forms::Cursors::Hand;
			this->txtTrainButton->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->txtTrainButton->Font = (gcnew System::Drawing::Font(L"Consolas", 14.25F, System::Drawing::FontStyle::Bold));
			this->txtTrainButton->Location = System::Drawing::Point(6, 81);
			this->txtTrainButton->Name = L"txtTrainButton";
			this->txtTrainButton->Size = System::Drawing::Size(213, 33);
			this->txtTrainButton->TabIndex = 17;
			this->txtTrainButton->Text = L"txt to train";
			this->txtTrainButton->UseVisualStyleBackColor = false;
			this->txtTrainButton->Click += gcnew System::EventHandler(this, &WinForm::txtTrainButton_Click);
			// 
			// imgTxtButton
			// 
			this->imgTxtButton->BackColor = System::Drawing::Color::PaleGreen;
			this->imgTxtButton->Cursor = System::Windows::Forms::Cursors::Hand;
			this->imgTxtButton->Enabled = false;
			this->imgTxtButton->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->imgTxtButton->Font = (gcnew System::Drawing::Font(L"Consolas", 14.25F, System::Drawing::FontStyle::Bold));
			this->imgTxtButton->Location = System::Drawing::Point(6, 95);
			this->imgTxtButton->Name = L"imgTxtButton";
			this->imgTxtButton->Size = System::Drawing::Size(213, 33);
			this->imgTxtButton->TabIndex = 18;
			this->imgTxtButton->Text = L"get PCA , LDA";
			this->imgTxtButton->UseVisualStyleBackColor = false;
			this->imgTxtButton->Click += gcnew System::EventHandler(this, &WinForm::imgTxtButton_Click);
			// 
			// bagWordButton
			// 
			this->bagWordButton->BackColor = System::Drawing::Color::Salmon;
			this->bagWordButton->Cursor = System::Windows::Forms::Cursors::Hand;
			this->bagWordButton->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->bagWordButton->Font = (gcnew System::Drawing::Font(L"Consolas", 14.25F, System::Drawing::FontStyle::Bold));
			this->bagWordButton->Location = System::Drawing::Point(6, 40);
			this->bagWordButton->Name = L"bagWordButton";
			this->bagWordButton->Size = System::Drawing::Size(213, 33);
			this->bagWordButton->TabIndex = 19;
			this->bagWordButton->Text = L"Train Bag of Words";
			this->bagWordButton->UseVisualStyleBackColor = false;
			this->bagWordButton->Click += gcnew System::EventHandler(this, &WinForm::bagWordButton_Click);
			// 
			// testBOWButton
			// 
			this->testBOWButton->BackColor = System::Drawing::Color::PaleGreen;
			this->testBOWButton->Cursor = System::Windows::Forms::Cursors::Hand;
			this->testBOWButton->Enabled = false;
			this->testBOWButton->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->testBOWButton->Font = (gcnew System::Drawing::Font(L"Consolas", 14.25F, System::Drawing::FontStyle::Bold));
			this->testBOWButton->Location = System::Drawing::Point(6, 205);
			this->testBOWButton->Name = L"testBOWButton";
			this->testBOWButton->Size = System::Drawing::Size(213, 33);
			this->testBOWButton->TabIndex = 20;
			this->testBOWButton->Text = L"Test BOW";
			this->testBOWButton->UseVisualStyleBackColor = false;
			this->testBOWButton->Click += gcnew System::EventHandler(this, &WinForm::testBOWButton_Click);
			// 
			// chooseVocButton
			// 
			this->chooseVocButton->BackColor = System::Drawing::Color::PaleGreen;
			this->chooseVocButton->Cursor = System::Windows::Forms::Cursors::Hand;
			this->chooseVocButton->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->chooseVocButton->Font = (gcnew System::Drawing::Font(L"Consolas", 14.25F, System::Drawing::FontStyle::Bold));
			this->chooseVocButton->Location = System::Drawing::Point(6, 40);
			this->chooseVocButton->Name = L"chooseVocButton";
			this->chooseVocButton->Size = System::Drawing::Size(213, 33);
			this->chooseVocButton->TabIndex = 21;
			this->chooseVocButton->Text = L"Choose Vocabulary";
			this->chooseVocButton->UseVisualStyleBackColor = false;
			this->chooseVocButton->Click += gcnew System::EventHandler(this, &WinForm::chooseVocButton_Click);
			// 
			// trainingGroupBox
			// 
			this->trainingGroupBox->Controls->Add(this->truthButton);
			this->trainingGroupBox->Controls->Add(this->falseButton);
			this->trainingGroupBox->Controls->Add(this->trainButton);
			this->trainingGroupBox->Controls->Add(this->txtTrainButton);
			this->trainingGroupBox->Controls->Add(this->bagWordButton);
			this->trainingGroupBox->Font = (gcnew System::Drawing::Font(L"Consolas", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->trainingGroupBox->Location = System::Drawing::Point(7, 528);
			this->trainingGroupBox->Name = L"trainingGroupBox";
			this->trainingGroupBox->Size = System::Drawing::Size(225, 256);
			this->trainingGroupBox->TabIndex = 22;
			this->trainingGroupBox->TabStop = false;
			this->trainingGroupBox->Text = L"Training";
			// 
			// processingGroupBox
			// 
			this->processingGroupBox->Controls->Add(this->chooseVocButton);
			this->processingGroupBox->Controls->Add(this->imgTxtButton);
			this->processingGroupBox->Controls->Add(this->testBOWButton);
			this->processingGroupBox->Controls->Add(this->videoTxtButton);
			this->processingGroupBox->Font = (gcnew System::Drawing::Font(L"Consolas", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->processingGroupBox->Location = System::Drawing::Point(263, 528);
			this->processingGroupBox->Name = L"processingGroupBox";
			this->processingGroupBox->Size = System::Drawing::Size(226, 256);
			this->processingGroupBox->TabIndex = 23;
			this->processingGroupBox->TabStop = false;
			this->processingGroupBox->Text = L"Processing";
			// 
			// testingGroupBox
			// 
			this->testingGroupBox->Controls->Add(this->modelButton);
			this->testingGroupBox->Controls->Add(this->testVideoButton);
			this->testingGroupBox->Controls->Add(this->testImageButton);
			this->testingGroupBox->Font = (gcnew System::Drawing::Font(L"Consolas", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->testingGroupBox->Location = System::Drawing::Point(520, 529);
			this->testingGroupBox->Name = L"testingGroupBox";
			this->testingGroupBox->Size = System::Drawing::Size(225, 255);
			this->testingGroupBox->TabIndex = 24;
			this->testingGroupBox->TabStop = false;
			this->testingGroupBox->Text = L"Testing";
			// 
			// WinForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(753, 796);
			this->Controls->Add(this->testingGroupBox);
			this->Controls->Add(this->processingGroupBox);
			this->Controls->Add(this->trainingGroupBox);
			this->Controls->Add(this->fileTextBox);
			this->Controls->Add(this->modelTextBox);
			this->Controls->Add(this->originPictureBox);
			this->Name = L"WinForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"WinForm";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->originPictureBox))->EndInit();
			this->trainingGroupBox->ResumeLayout(false);
			this->processingGroupBox->ResumeLayout(false);
			this->testingGroupBox->ResumeLayout(false);
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
			 vector<std::string> loadImgsFromFolder() {
				 vector<std::string> all_files = vector<std::string>();

				 FolderBrowserDialog^ folderBrowserDiaglog = gcnew FolderBrowserDialog();
				 if (folderBrowserDiaglog->ShowDialog() == System::Windows::Forms::DialogResult::Cancel) {
					 return all_files;
				 }
				 std::string path = msclr::interop::marshal_as<std::string>(folderBrowserDiaglog->SelectedPath);

				 vector<std::string> filenames = vector<std::string>();
				 getdir(path,filenames);

				 std::string message;
				 for(unsigned int i = 0; i < filenames.size(); i++)
				 {
					 if (filenames[i] == "." || filenames[i] == "..")
						 continue;
					 if (filenames[i].find("jpg") == std::string::npos)
						 continue;
					 message = path + "\\" + filenames[i];
					 all_files.push_back(message);
				 }

				 return all_files;
			 }
	private: System::Void truthButton_Click(System::Object^  sender, System::EventArgs^  e) {
				 vector<std::string> all_files = loadImgsFromFolder();
				 if (all_files.empty())	return;
				 
				 RNG& rng = theRNG();

				 while (all_files.size() > 0) {
					 int randImgIdx = rng((unsigned)all_files.size());
					 w_opencv.readImage(all_files[randImgIdx]);

					 /*int histSize = 16;
					 float range[] = { 0, 256 } ;
					 const float* histRange = { range };
					 w_opencv.calHistogram(histSize, histRange);*/

					 w_opencv.detectSkin();
					 w_opencv.regionCut();

					 w_opencv.extractBOW();

					 w_opencv.HuMoment();

					 w_fourier.image_process(w_opencv.getImage());

					 vector< vector<float> > features;

					 //features.push_back(w_opencv.getHistVector());
					 features.push_back(w_opencv.getHuVector());
					 features.push_back(w_opencv.getSiftVector());
					 features.push_back(w_fourier.get_vector());

					 w_svm.concatenateGt(features);

					 //===========display on window==============
					 // To avoid memory leakage
					 delete originPictureBox->Image;
					 Bitmap^ testImage;
					 Bitmap^ resizeImage;
					 try {
						 testImage = w_opencv.getBitmap();
						 if (testImage->Width > originPictureBox->Width || testImage->Height > originPictureBox->Height) {
							 resizeImage = gcnew Bitmap(testImage, originPictureBox->Size);
							 originPictureBox->Image = resizeImage;
						 }
						 else {
							 originPictureBox->Image = testImage;
						 }
						 originPictureBox->Refresh();
					 }
					 finally {
						 delete testImage;
						 delete resizeImage;
					 }

					 string message = std::to_string(all_files.size()) + " images left\t" + all_files[randImgIdx];
					 System::String^ string = gcnew System::String(message.c_str());
					 fileTextBox->Text = string;
					 fileTextBox->Refresh();
					 //==========================================

					 features.clear();
					 w_opencv.clear();
					 w_fourier.clear_vector();
					 all_files.erase(all_files.begin() + randImgIdx);
				 }
				 MessageBoxA(0, "跑完了!", "Ground Truth", MB_OK);
				 delete originPictureBox->Image;		originPictureBox->Image = nullptr;
			 }
	private: System::Void falseButton_Click(System::Object^  sender, System::EventArgs^  e) {
				 vector<std::string> all_files = loadImgsFromFolder();
				 if (all_files.empty())	return;

				 RNG& rng = theRNG();

				 while (all_files.size() > 0) {
					 int randImgIdx = rng((unsigned)all_files.size());
					 w_opencv.readImage(all_files[randImgIdx]);

					 /*int histSize = 16;
					 float range[] = { 0, 256 } ;
					 const float* histRange = { range };
					 w_opencv.calHistogram(histSize, histRange);*/

					 w_opencv.detectSkin();
					 w_opencv.regionCut();

					 w_opencv.extractBOW();

					 w_opencv.HuMoment();

					 w_fourier.image_process(w_opencv.getImage());

					 vector< vector<float> > features;
					 
					 //features.push_back(w_opencv.getHistVector());
					 features.push_back(w_opencv.getHuVector());
					 features.push_back(w_opencv.getSiftVector());
					 features.push_back(w_fourier.get_vector());

					 w_svm.concatenateOther(features);

					 //===========display on window==============
					 // To avoid memory leakage
					 delete originPictureBox->Image;
					 Bitmap^ testImage;
					 Bitmap^ resizeImage;
					 try {
						 testImage = w_opencv.getBitmap();
						 if (testImage->Width > originPictureBox->Width || testImage->Height > originPictureBox->Height) {
							 resizeImage = gcnew Bitmap(testImage, originPictureBox->Size);
							 originPictureBox->Image = resizeImage;
						 }
						 else {
							 originPictureBox->Image = testImage;
						 }
						 originPictureBox->Refresh();
					 }
					 finally {
						 delete testImage;
						 delete resizeImage;
					 }

					 string message = std::to_string(all_files.size()) + " images left\t" + all_files[randImgIdx];
					 System::String^ string = gcnew System::String(message.c_str());
					 fileTextBox->Text = string;
					 fileTextBox->Refresh();
					 //==========================================

					 features.clear();
					 w_opencv.clear();
					 w_fourier.clear_vector();
					 all_files.erase(all_files.begin() + randImgIdx);
				 }
				 MessageBoxA(0, "跑完了!", "Ground False", MB_OK);
				 delete originPictureBox->Image;		originPictureBox->Image = nullptr;

				 trainButton->Enabled = true;
			 }
	private: System::Void trainButton_Click(System::Object^  sender, System::EventArgs^  e) {
				 w_svm.trainSVM();
				 w_svm.clearVectors();
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
				 testImageButton->Enabled = true;
			 }
	private: System::Void testVideoButton_Click(System::Object^  sender, System::EventArgs^  e) {
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

				 ofstream output(file.substr(0, file.find_last_of('\\')) + "\\output.txt", ios::out);
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

					 w_opencv.detectSkin();
					 w_opencv.regionCut();

					 //===========display on window==============
					 Bitmap^ testImage = w_opencv.getBitmap();
					 if (testImage->Width > originPictureBox->Width || testImage->Height > originPictureBox->Height) {
						 Bitmap^ resizeImage = gcnew Bitmap(testImage, originPictureBox->Size);
						 originPictureBox->Image = resizeImage;
					 }
					 else {
						 originPictureBox->Image = testImage;
					 }
					 originPictureBox->Refresh();
					 //==========================================

					 w_opencv.detectSIFT();

					 w_opencv.HuMoment();
					 std::vector<float> huVector = w_opencv.getHuVector();

					 w_fourier.image_process(w_opencv.getImage());

					 vector< vector<float> > features;

					 features.push_back(w_opencv.getHuVector());
					 features.push_back(w_opencv.getSiftVector());
					 features.push_back(w_fourier.get_vector());

					 w_svm.concatenateTest(features);

					 System::String^ string = gcnew System::String(std::to_string(fn).c_str());
					 fileTextBox->Text = string;
					 fileTextBox->Refresh();

					 float res = w_svm.testSVM();

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
			 // Read images from a folder & construct a bag of words with these images
	private: System::Void bagWordButton_Click(System::Object^  sender, System::EventArgs^  e) {
				 vector<std::string> all_files = loadImgsFromFolder();
				 if (all_files.empty())	return;

				 std::string path = all_files[0].substr(0, all_files[0].find_last_of('\\'));
				 Mat allDescriptors;
				 RNG& rng = theRNG();

				 while (all_files.size() > 0) {
					 int randImgIdx = rng((unsigned)all_files.size());
					 w_opencv.readImage(all_files[randImgIdx]);

					 w_opencv.detectSkin();
					 w_opencv.regionCut();

					 // debug output
					 //imwrite("skin\\" + all_files[randImgIdx].substr(all_files[randImgIdx].find_last_of('\\') + 1) + "_region.jpg", w_opencv.getSkinImage());

					 //===========display on window==============
					 // To avoid memory leakage
					 delete originPictureBox->Image;
					 Bitmap^ testImage;
					 Bitmap^ resizeImage;
					 try {
						 testImage = w_opencv.getBitmap();
						 if (testImage->Width > originPictureBox->Width || testImage->Height > originPictureBox->Height) {
							 resizeImage = gcnew Bitmap(testImage, originPictureBox->Size);
							 originPictureBox->Image = resizeImage;
						 }
						 else {
							 originPictureBox->Image = testImage;
						 }
						 originPictureBox->Refresh();
					 }
					 finally {
						 delete testImage;
						 delete resizeImage;
					 }

					 string message = std::to_string(all_files.size()) + " images left";
					 System::String^ string = gcnew System::String(message.c_str());
					 fileTextBox->Text = string;
					 fileTextBox->Refresh();
					 modelTextBox->Text = gcnew System::String(all_files[randImgIdx].c_str());
					 modelTextBox->Refresh();
					 //==========================================

					 w_opencv.detectSIFT();

					 allDescriptors.push_back(w_opencv.getSiftDescriptor());
					 
					 w_opencv.clear();
					 all_files.erase(all_files.begin() + randImgIdx);
				 }

				 // K Means & Training
				 BOWKMeansTrainer bowTrainer(100);
				 Mat vocabulary = bowTrainer.cluster(allDescriptors);

				 FileStorage fs(path + "\\vocabulary_region_100.yaml", FileStorage::WRITE);
				 if(fs.isOpened()) {
					 fs << "vocabulary" << vocabulary;
				 }
				 fs.release();

				 w_opencv.clear();
			 }
	private: System::Void chooseVocButton_Click(System::Object^  sender, System::EventArgs^  e) {
				 OpenFileDialog ^ openFileDialog1 = gcnew OpenFileDialog();
				 openFileDialog1->Filter = "Vocabulary File (*.yaml,*.*)|*.yaml;*.*";
				 openFileDialog1->Title = "選擇Vocabulary";

				 if (openFileDialog1->ShowDialog(this) == System::Windows::Forms::DialogResult::Cancel)   // 使用者沒有選檔案
					 return;

				 Mat vocabulary;

				 std::string file;	
				 file = msclr::interop::marshal_as<std::string>(openFileDialog1->FileName);
				 FileStorage fs( file, FileStorage::READ );
				 if( fs.isOpened() )
				 {
					 fs["vocabulary"] >> vocabulary;
				 }
				 fs.release();

				 /**********DEBUG************
				 FileStorage fo(file.substr(0, file.find_last_of('\\')) + "\\debug.yaml", FileStorage::WRITE);
				 if(fo.isOpened()) {
					 fo << "vocabulary" << vocabulary;
				 }
				 fo.release();
				 ***************************/

				 w_opencv.setBOWExtractor(vocabulary);

				 truthButton->Enabled = true;
				 falseButton->Enabled = true;
				 testBOWButton->Enabled = true;
				 imgTxtButton->Enabled = true;
				 videoTxtButton->Enabled = true;
				 if (modelChooser->CheckFileExists == true) {
					 testImageButton->Enabled = true;
					 testVideoButton->Enabled = true;
				 }
			 }
	private: System::Void testBOWButton_Click(System::Object^  sender, System::EventArgs^  e) {
				 OpenFileDialog ^ openFileDialog1 = gcnew OpenFileDialog();
				 openFileDialog1->Filter = "Image File (*.jpg,*.bmp)|*.jpg;*.bmp;*.*";
				 openFileDialog1->Title = "開啟影像";

				 if (openFileDialog1->ShowDialog(this) == System::Windows::Forms::DialogResult::Cancel)   // 使用者沒有選檔案
					 return;

				 std::string file;
				 file = msclr::interop::marshal_as<std::string>(openFileDialog1->FileName);
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
				 
				 // Extract Bag of Words
				 w_opencv.extractBOW();

				 w_opencv.clear();
			 }
			 // Read images from a folder & output feature vectors to text file.
	private: System::Void imgTxtButton_Click(System::Object^  sender, System::EventArgs^  e) {
				 //==========img to text code========
				 //vector<std::string> all_files = loadImgsFromFolder();
				 //if (all_files.empty())	return;
				 //
				 //// output feature vector
				 //vector<float> outputVector;
				 //std::string string = all_files[0].substr(0, all_files[0].find_last_of('\\'));
				 //fstream output(string + "\\output.txt", ios::out);
				 //
				 //// for each image, extract its features & combine them together, then output
				 //for (unsigned int i = 0; i < all_files.size(); ++i) {
					// w_opencv.readImage(all_files[i]);
					// 
					// w_opencv.extractBOW();

					// w_opencv.HuMoment();

					// w_fourier.image_process(w_opencv.getImage());

					// vector< vector<float> > features;

					// features.push_back(w_opencv.getHuVector());
					// features.push_back(w_opencv.getSiftVector());
					// features.push_back(w_fourier.get_vector());

					// w_svm.concatenateTest(features);

					// outputVector = w_svm.getTestVector();

					// for (unsigned int j = 0; j < outputVector.size(); ++j) {
					//	 output << outputVector[j] << " ";
					// }
					// output << endl;

					// //=============display on window================
					// delete originPictureBox->Image;		// To avoid memory leakage
					// Bitmap^ testImage = w_opencv.getBitmap();
					// if (testImage->Width > originPictureBox->Width || testImage->Height > originPictureBox->Height) {
					//	 Bitmap^ resizeImage = gcnew Bitmap(testImage, originPictureBox->Size);
					//	 originPictureBox->Image = resizeImage;
					// }
					// else {
					//	 originPictureBox->Image = testImage;
					// }
					// originPictureBox->Refresh();
					// 
					// System::String^ string = gcnew System::String(all_files[i].c_str());
					// fileTextBox->Text = string;
					// fileTextBox->Refresh();
					// //==============================================

					// features.clear();
					// w_opencv.clear();
					// w_fourier.clear_vector();
					// w_svm.clear_testVector();
					// outputVector.clear();
				 //}
				 //
				 //output.close();

				int classes = w_svm.update_class();
				vector<std::string> all_files = loadImgsFromFolder();
				 if (all_files.empty())	return;
				 
				 RNG& rng = theRNG();

				 while (all_files.size() > 0) {
					 int randImgIdx = rng((unsigned)all_files.size());
					 w_opencv.readImage(all_files[randImgIdx]);

					 w_opencv.extractBOW();

					 w_opencv.HuMoment();

					 w_fourier.image_process(w_opencv.getImage());

					 vector< vector<float> > features;

					 features.push_back(w_opencv.getHuVector());
					 features.push_back(w_opencv.getSiftVector());
					 features.push_back(w_fourier.get_vector());

					 w_svm.concatenateAll(features);
					 w_svm.concatenateAllclasses(classes);
					 //===========display on window==============
					 // To avoid memory leakage
					 delete originPictureBox->Image;
					 Bitmap^ testImage;
					 Bitmap^ resizeImage;
					 try {
						 testImage = w_opencv.getBitmap();
						 if (testImage->Width > originPictureBox->Width || testImage->Height > originPictureBox->Height) {
							 resizeImage = gcnew Bitmap(testImage, originPictureBox->Size);
							 originPictureBox->Image = resizeImage;
						 }
						 else {
							 originPictureBox->Image = testImage;
						 }
						 originPictureBox->Refresh();
					 }
					 finally {
						 delete testImage;
						 delete resizeImage;
					 }

					 string message = std::to_string(all_files.size()) + " images left";
					 System::String^ string = gcnew System::String(message.c_str());
					 fileTextBox->Text = string;
					 fileTextBox->Refresh();
					 //==========================================

					 features.clear();
					 w_opencv.clear();
					 w_fourier.clear_vector();
					 all_files.erase(all_files.begin() + randImgIdx);
				 }
				 std::string test1 = std::to_string(classes);
				 MessageBoxA(0, test1.c_str(), "Ground Truth", MB_OK);
		 }
	private: System::Void videoTxtButton_Click(System::Object^  sender, System::EventArgs^  e) {
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

				 int fn = 0;
				 Mat frame;
				 vector<float> outputVector;

				 std::string string = file.substr(0, file.find_last_of('\\'));
				 fstream output(string + "\\output.txt", ios::out);

				 cap >> frame;
				 w_opencv.readFrame(frame);
				 fn++;
				 w_opencv.set_bg_frame();

				 while(1) {
					 cap >> frame;
					 fn++;

					 if(frame.empty()) {
						break;
					 }
					 
					 // 每10個frame取1張
					 if(fn%10 != 0) {
						 continue;
					 }

					 w_opencv.readFrame(frame);
					 w_opencv.img_preproc();
					 w_opencv.detectSIFT();

					 w_opencv.HuMoment();

					 w_fourier.image_process(w_opencv.getImage());

					 vector< vector<float> > features;

					 features.push_back(w_opencv.getHuVector());
					 features.push_back(w_opencv.getSiftVector());
					 features.push_back(w_fourier.get_vector());

					 w_svm.concatenateTest(features);

					 outputVector = w_svm.getTestVector();

					 for (unsigned int i = 0; i < outputVector.size(); ++i) {
						 output << outputVector[i] << " ";
					 }
					 output << endl;

					 //=============display on window================
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

					 features.clear();
					 w_opencv.clear();
					 w_fourier.clear_vector();
					 w_svm.clear_testVector();
					 outputVector.clear();
				 } // end while(1)

				 output.close();
				 MessageBoxA(0, "跑完了!", "TXT", MB_OK);
			 }
	private: System::Void txtTrainButton_Click(System::Object^  sender, System::EventArgs^  e) {
				 OpenFileDialog ^ openFileDialog1 = gcnew OpenFileDialog();
				 openFileDialog1->Filter = "Text Files (*.txt)|*.txt";
				 // Read Ground Truth text file
				 openFileDialog1->Title = "選擇GroundTruth.txt";

				 if (openFileDialog1->ShowDialog(this) == System::Windows::Forms::DialogResult::Cancel)   // 使用者沒有選檔案
					 return;

				 std::string textFile;
				 textFile = msclr::interop::marshal_as<std::string>(openFileDialog1->FileName);

				 ifstream gtText(textFile, ios::in);

				 vector< vector<float> > inputVectors;
				 vector<float> inputVector;
				 istringstream istr;
				 string strLine;
				 
				 while (std::getline(gtText, strLine)) {
					 istr.str(strLine);
					 float feature;
					 istr >> feature;
					 while (!istr.eof()) {
						 inputVector.push_back(feature);
						 istr >> feature;
					 }

					 inputVectors.push_back(inputVector);

					 System::String^ string = gcnew System::String(std::to_string(inputVectors.size()).c_str());
					 fileTextBox->Text = string;
					 fileTextBox->Refresh();
					 
					 inputVector.clear();
					 istr.clear();
				 }

				 w_svm.concatenateGt(inputVectors);

				 inputVectors.clear();
				 gtText.close();

				 // Read Ground False text file
				 openFileDialog1->Title = "選擇GroundFalse.txt";

				 if (openFileDialog1->ShowDialog(this) == System::Windows::Forms::DialogResult::Cancel)   // 使用者沒有選檔案
					 return;

				 textFile = msclr::interop::marshal_as<std::string>(openFileDialog1->FileName);

				 ifstream gfText(textFile, ios::in);
				 
				 while (std::getline(gfText, strLine)) {
					 istr.str(strLine);
					 float feature;
					 istr >> feature;
					 while (!istr.eof()) {
						 inputVector.push_back(feature);
						 istr >> feature;
					 }

					 inputVectors.push_back(inputVector);

					 System::String^ string = gcnew System::String(std::to_string(inputVectors.size()).c_str());
					 fileTextBox->Text = string;
					 fileTextBox->Refresh();
					 
					 inputVector.clear();
					 istr.clear();
				 }

				 w_svm.concatenateOther(inputVectors);

				 inputVectors.clear();
				 gfText.close();

				 w_svm.trainSVM();
				 w_svm.clearVectors();
				 MessageBoxA(0, "跑完了!", "SVM", MB_OK);
			 }
	private: System::Void testImageButton_Click(System::Object^  sender, System::EventArgs^  e) {
				 /*
				 vector<std::string> models = vector<std::string>();

				 FolderBrowserDialog^ folderBrowserDiaglog = gcnew FolderBrowserDialog();
				 if (folderBrowserDiaglog->ShowDialog() == System::Windows::Forms::DialogResult::Cancel) {
					 return ;
				 }
				 std::string path = msclr::interop::marshal_as<std::string>(folderBrowserDiaglog->SelectedPath);

				 vector<std::string> filenames = vector<std::string>();
				 getdir(path,filenames);

				 std::string message;
				 for(unsigned int i = 0; i < filenames.size(); i++)
				 {
					 if (filenames[i].find("xml") == std::string::npos)
						 continue;
					 message = path + "\\" + filenames[i];
					 models.push_back(message);
				 }
				 */
				 vector<std::string> all_files = loadImgsFromFolder();
				 /*
				 vector<vector<float>> distance(all_files.size());
				 for(int i=0; i<all_files.size(); i++)
					distance[i].resize(5);
				 */
				 if (all_files.empty())	return;

				 namedWindow("debug");
				 fstream output("result_auto.txt", ios::out);
				// testVideoButton->Enabled = true;
				// testImageButton->Enabled = true;
					
				// for(int g_model=0; g_model<models.size(); g_model++) {
				//	 w_svm.setModel(models[g_model]);
				//	 modelTextBox->Text = gcnew System::String(models[g_model].c_str());
				//	 modelTextBox->Refresh();
					 /*
					 string message = "Model NO." + std::to_string(g_model+1) + " is processing";
					 System::String^ string = gcnew System::String(message.c_str());
					 fileTextBox->Text = string;
					 fileTextBox->Refresh();
					 */
					 //for(int i=0; i<all_files.size(); i++) {
				 while(all_files.size() > 0) {
					 w_opencv.readImage(all_files[0]);

					 /*int histSize = 16;
					 float range[] = { 0, 256 } ;
					 const float* histRange = { range };
					 w_opencv.calHistogram(histSize, histRange);*/

					 w_opencv.detectSkin();
					 w_opencv.regionCut();

					 w_opencv.extractBOW();

					 w_opencv.HuMoment();

					 w_fourier.image_process(w_opencv.getImage());

					 vector< vector<float> > features;

					 //features.push_back(w_opencv.getHistVector());
					 features.push_back(w_opencv.getHuVector());
					 features.push_back(w_opencv.getSiftVector());
					 features.push_back(w_fourier.get_vector());

					 w_svm.concatenateTest(features);

					 float res = w_svm.testSVM();
					 // distance[i][g_model] = res;

					 //===========display on window==============
					 // To avoid memory leakage
					 if(!originPictureBox->Image)
						 delete originPictureBox->Image;
					 Bitmap^ testImage;
					 Bitmap^ resizeImage;
					 try {
						 testImage = w_opencv.getBitmap();
						 if (testImage->Width > originPictureBox->Width || testImage->Height > originPictureBox->Height) {
							 resizeImage = gcnew Bitmap(testImage, originPictureBox->Size);
							 originPictureBox->Image = resizeImage;
						 }
						 else {
							 originPictureBox->Image = testImage;
						 }
						 originPictureBox->Refresh();
					 }
					 finally {
						 delete testImage;
						 delete resizeImage;
					 }
					 waitKey(10);

					 string message = std::to_string(all_files.size()) + " images left\t" + all_files[0] + '\t' + std::to_string(res);
					 System::String^ string = gcnew System::String(message.c_str());
					 fileTextBox->Text = string;
					 fileTextBox->Refresh();
					 //==========================================
					 output << all_files[0] << " ";
					 output << res << endl;

					 features.clear();
					 w_opencv.clear();
					 w_fourier.clear_vector();
					 all_files.erase(all_files.begin());
					 w_svm.clear_testVector();
				 }
				 string message = "SVM Testing finish !!";
				 System::String^ string = gcnew System::String(message.c_str());
				 fileTextBox->Text = string;
				 fileTextBox->Refresh();
				 delete originPictureBox->Image;		originPictureBox->Image = nullptr;

				 output.close();
				 /*
				 fstream output("5GROUP_result_auto.txt", ios::out);

				 for(int i=0; i<all_files.size(); i++) {	
				 //int min=100,min_index=0;
				 output << all_files[i] << " ";
				 for(int g_model=0; g_model<5; g_model++) {
				 //if(min > distance[i][g_model]) {
				 //	min_index = g_model;
				 //	min = distance[i][g_model];
				 //}
				 output << distance[i][g_model] << " ";
				 }	
				 output << endl;
				 //output << min_index << endl;
				 }
				 output.close();
				 */
			 }
};
}
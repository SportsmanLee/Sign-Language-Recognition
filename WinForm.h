#pragma once
#include "MyCV.h"
#include <msclr/marshal_cppstd.h>
#include "include\opencv2\core\core.hpp"
#include "include\opencv2\highgui\highgui.hpp"

MyCV w_opencv;

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
	private: System::Windows::Forms::Button^  histButton;
	private: System::Windows::Forms::Button^  SIFTButton;




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
			this->histButton = (gcnew System::Windows::Forms::Button());
			this->SIFTButton = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->originPictureBox))->BeginInit();
			this->SuspendLayout();
			// 
			// loadImageButton
			// 
			this->loadImageButton->Location = System::Drawing::Point(90, 51);
			this->loadImageButton->Name = L"loadImageButton";
			this->loadImageButton->Size = System::Drawing::Size(75, 23);
			this->loadImageButton->TabIndex = 0;
			this->loadImageButton->Text = L"Load Image";
			this->loadImageButton->UseVisualStyleBackColor = true;
			this->loadImageButton->Click += gcnew System::EventHandler(this, &WinForm::loadImageButton_Click);
			// 
			// originPictureBox
			// 
			this->originPictureBox->Location = System::Drawing::Point(291, 12);
			this->originPictureBox->Name = L"originPictureBox";
			this->originPictureBox->Size = System::Drawing::Size(411, 418);
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
			// histButton
			// 
			this->histButton->Location = System::Drawing::Point(90, 118);
			this->histButton->Name = L"histButton";
			this->histButton->Size = System::Drawing::Size(75, 23);
			this->histButton->TabIndex = 2;
			this->histButton->Text = L"Histogram";
			this->histButton->UseVisualStyleBackColor = true;
			this->histButton->Click += gcnew System::EventHandler(this, &WinForm::histButton_Click);
			// 
			// SIFTButton
			// 
			this->SIFTButton->Location = System::Drawing::Point(90, 184);
			this->SIFTButton->Name = L"SIFTButton";
			this->SIFTButton->Size = System::Drawing::Size(75, 23);
			this->SIFTButton->TabIndex = 3;
			this->SIFTButton->Text = L"SIFT";
			this->SIFTButton->UseVisualStyleBackColor = true;
			this->SIFTButton->Click += gcnew System::EventHandler(this, &WinForm::SIFTButton_Click);
			// 
			// WinForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(714, 442);
			this->Controls->Add(this->SIFTButton);
			this->Controls->Add(this->histButton);
			this->Controls->Add(this->originPictureBox);
			this->Controls->Add(this->loadImageButton);
			this->Name = L"WinForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"WinForm";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->originPictureBox))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void loadImageButton_Click(System::Object^  sender, System::EventArgs^  e) {
				 if (fileChooser->ShowDialog() == System::Windows::Forms::DialogResult::OK) {

					 // read an image & resize it to fit the picture box
					 w_opencv.readImage(msclr::interop::marshal_as<std::string>(fileChooser->FileName));
					 Bitmap^ tempImage = w_opencv.getBitmap();
					 Bitmap^ originImage = gcnew Bitmap(tempImage, originPictureBox->Size);
					 originPictureBox->Image = originImage;
				 }
			 }
	private: System::Void histButton_Click(System::Object^  sender, System::EventArgs^  e) {
				 int histSize = 16;
				 float range[] = { 0, 256 } ;
				 const float* histRange = { range };

				 w_opencv.calHistogram(histSize, histRange);


			 }
	private: System::Void SIFTButton_Click(System::Object^  sender, System::EventArgs^  e) {
				 w_opencv.detectSIFT();
			 }
	};
}
#pragma once

namespace CWinFormOpenCV {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

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
			this->fileChooser->Filter = "點陣圖檔案 (*.bmp;*.dib)|*.bmp|JPEG (*.jpg;*.jpeg;*.jpe;*.jfif)|*.jpg|GIF (*.gif)|*.gi" +
				"f|TIFF (*.tiff;*.tif)|*.tiff|PNG (*.png)|*.png|ICO (*.ico)|*.ico|所有檔案 (*.*)|*.*";
			this->fileChooser->FilterIndex = 7;
			this->fileChooser->Multiselect = true;
			this->fileChooser->Title = "選取影像";
			// 
			// WinForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(714, 442);
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
					Bitmap^ tempImage = gcnew Bitmap(fileChooser->FileName);
					Bitmap^ loadedImage = gcnew Bitmap(tempImage, originPictureBox->Size);
					originPictureBox->Image = loadedImage;
				}
			 }
	};
}

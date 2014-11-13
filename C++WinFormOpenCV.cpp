// C++WinFormOpenCV.cpp: 主要專案檔。

#include "stdafx.h"
#include "WinForm.h"

using namespace System;
using namespace CWinFormOpenCV;

MyCV w_opencv;
fourier w_fourier;
MySVM w_svm;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
    Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	Application::Run(gcnew WinForm());

	return 0;
}

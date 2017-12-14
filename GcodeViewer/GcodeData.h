#pragma once
#include "State.h"
#include "Polyline.h"
#include "Kadr.h"

namespace GData {
	using namespace System;
	using namespace System::Collections;
	using namespace System::Drawing;
	using namespace System::Globalization;
	//	using namespace OpenTK;
	//	using namespace OpenTK::Platform::Windows;
	//	using namespace OpenTK::Graphics::OpenGL;

	//����� ������, �������� ��������� ����� �������, ����������� �� �����, 
	//��� ��������� � ������� ���������, �������� ��������� ������� ����� � �� ������
	public ref class GcodeData
	{
	public:
		System::Collections::Generic::List<Int32>^ errorRows;
		System::Collections::Generic::List<String^>^ commands;
		System::Collections::Generic::List<Polyline^>^ polylines;
		float minX = 0, maxX = 0;
		float minY = 0, maxY = 0;
		float minZ = 0, maxZ = 0;
		GcodeData();
		~GcodeData();
		//�������� ��� ��������� � ��������� ����� ������� � ���� gcoded->str[i]
		property String^ GcodeData::str[int]{
			String^ get(int index);
		void set(int index, String^ value);
		}

			//������� �������� ������ �� ����� � ��������� ����� �������
		bool loadFile(String^ filepath);
		//��������� ����������� ����������� ���������� ���� ������
		bool checkMinMax();
		void resetMinMax();
		//������� ������������� �������� ������� ��������� � ����������, 
		// � ������� ����� ��������� ������ � ��������� polylines
		System::Collections::Generic::List<Polyline^>^ tranlate(System::Collections::Generic::List<String^>^ cmds);

	};

}
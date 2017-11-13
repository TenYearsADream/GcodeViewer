#pragma once
namespace GData {
	using namespace System;
	using namespace System::Collections;
	using namespace System::Drawing;
	using namespace OpenTK;
	using namespace OpenTK::Platform::Windows;
	using namespace OpenTK::Graphics::OpenGL;

	//�����, ������������ ��� ���������� ��������� ����� ������� ����������� ����� ������ ����� (RGB)
	public ref class Polyline sealed {
	public:
		Polyline(int _n);
		Polyline(int _n, array<float>^ _x, array<float>^  _y, array<float>^  _z) :n(n), x(_x), y(_y), z(_z) {};
		~Polyline()				//����������
		{
			delete x;
			delete y;
			delete z;
		}
	//	static bool    operator ==(const Polyline^, const Polyline^);
		static Polyline^ operator /(Polyline^, float d);
		int n;					//����� ��������
		array<float>^ x;		//������� ���������
		array<float>^ y;
		array<float>^ z;
		float red, green, blue;		//�������� ����� � ������� RGB 
	//	Color color;
	};

	//����� ������, �������� ��������� ����� �������, ����������� �� �����, 
	//��� ��������� � ������� ���������, �������� ��������� ������� ����� � �� ������
	public ref class GcodeData
	{
	public:
		System::Collections::Generic::List<String^>^ commands;
		System::Collections::Generic::List<Polyline^>^ polyline;

		GcodeData();
		~GcodeData();
		//�������� ��� ��������� � ��������� ����� ������� � ���� gcoded->str[i]
		property String^ GcodeData::str[int]{
			String^ get(int index);
		void set(int index, String^ value);
		}

		//������� �������� ������ �� ����� � ��������� ����� �������
		bool loadFile(String^ filepath);

		//������� ������������� �������� ������� ��������� � ����������, 
		// � ������� ����� ��������� ������ � ��������� polyline
		System::Collections::Generic::List<Polyline^>^ tranlate(System::Collections::Generic::List<String^>^ cmds);
	};

}
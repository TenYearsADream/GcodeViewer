#pragma once
namespace GData {
	using namespace System;
	using namespace System::Collections;
	using namespace System::Drawing;
//	using namespace OpenTK;
//	using namespace OpenTK::Platform::Windows;
//	using namespace OpenTK::Graphics::OpenGL;

	//������������ ��������� �� �������� � ����������� ��������� �������� ��� �������� �� ������������������
	//� ������ �������
	public enum class GState {
		None = -1, NotLoad = 0, LineRun = 1, CircClockwise = 2, CircCntrClockwise = 3, NewNullCoor = 10,
		SurfaceXY = 17, SurfaceZX=18, SurfaceYZ=19
	};
	public enum class Modal {
		None = -1, PreStart = 0, StopNoReset = 2, StartRotateClockwise = 3, StartRotateCntrClockwise = 4,
		StopRotate = 5, ChangeInsrtument = 6, StopWhithReset = 30
	};
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
		System::Collections::Generic::List<Int32>^ errorRows;
		System::Collections::Generic::List<String^>^ commands;
		System::Collections::Generic::List<Polyline^>^ polyline;

		GcodeData();
		~GcodeData();
		//�������� ��� ��������� � ��������� ����� ������� � ���� gcoded->str[i]
		property String^ GcodeData::str[int]{
			String^ get(int index);
		void set(int index, String^ value);
		}

			//���������� ������, ���������� ������ ����� �� ������� �������� ������ � �������� ��������,
			//���� ������ ����� ���, �� ���������� nulptr
		String^ getStartFloat(String^,int startIndex);

			//������� �������� ������ �� ����� � ��������� ����� �������
		bool loadFile(String^ filepath);

		//������� ������������� �������� ������� ��������� � ����������, 
		// � ������� ����� ��������� ������ � ��������� polyline
		System::Collections::Generic::List<Polyline^>^ tranlate(System::Collections::Generic::List<String^>^ cmds);
	};

}
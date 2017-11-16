#pragma once
namespace GData {
	using namespace System;
	using namespace System::Collections;
	using namespace System::Drawing;
	using namespace System::Globalization;
	//	using namespace OpenTK;
	//	using namespace OpenTK::Platform::Windows;
	//	using namespace OpenTK::Graphics::OpenGL;

		//������������ ��������� �� �������� � ����������� ��������� �������� ��� �������� �� ������������������
		//� ������ �������
	public enum class GState {
		None = -1, NotLoad = 0, LineRun = 1, CircClockwise = 2, CircCntrClockwise = 3, NewNullCoor = 10,
		SurfaceXY = 17, SurfaceZX = 18, SurfaceYZ = 19
	};
	public enum class Modal {
		None = -1, PreStart = 0, StopNoReset = 2, StartRotateClockwise = 3, StartRotateCntrClockwise = 4,
		StopRotate = 5, ChangeInsrtument = 6, StopWithReset = 30
	};
	//��������� ���������� ������ ����� ������
	public ref struct kadr
	{
		kadr() {};
		float x, y, z, i, j, k,f;
		bool bx, by, bz, bi, bj, bk, bf, bs;
		int s;
		//notful reset exclude f,s
		void reset() {
			bx = false;
			by = false;
			bz = false;
			bi = false;
			bj = false;
			bk = false;
		}
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

			//������� �������� ������ �� ����� � ��������� ����� �������
		bool loadFile(String^ filepath);

		//������� ������������� �������� ������� ��������� � ����������, 
		// � ������� ����� ��������� ������ � ��������� polyline
		System::Collections::Generic::List<Polyline^>^ tranlate(System::Collections::Generic::List<String^>^ cmds);

	private:
		NumberFormatInfo^ formatInfo;

		//���������� ������, ���������� ������ ����� �� ������� �������� ������ � �������� ��������,
		//���� ������ ����� ���, �� ���������� System::String::Empty, ���� ������ ��� ����� ������ �� ������ nulptr
		String^ getNumeric(String^, int startIndex, int &backlastindex);

		//��������� true, ���� ������� ���������� ���������� ������ ����� result �� ������ str,
		//������� � ������� startIndex,
		//�������� ������ �� ������� ����������� �����
		bool GData::GcodeData::getInt(String ^str, int &result, int startIndex, int &backlastindex);

		//��������� true, ���� ������� ���������� ���������� ������ ����� � ��������� ������ result
		//�� ������ str, ������� � ������� startIndex,
		//�������� ������ �� ������� ����������� �����
		bool GData::GcodeData::getFloat(String ^str, float &result, int startIndex, int &backlastindex);
	};

}
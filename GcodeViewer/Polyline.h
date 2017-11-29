#pragma once
#include "State.h"
//#include "Kadr.h"

namespace GData {

	using namespace System;
	using namespace System::Collections;
	//��������� ���������� ������ ����� ������
	//�����, ������������ ��� ���������� ��������� ����� ������� ����������� ����� ������ ����� (RGB)
	public ref class Polyline sealed {
	public:
		Polyline(int _n);		//����������� ��������� ������� ��������
		Polyline(int _n, Generic::List<float>^ _x, Generic::List<float>^  _y, Generic::List<float>^  _z)
			:n(n), x(_x), y(_y), z(_z) {};
		~Polyline() {};
	public:
		//������������� ���������� ������� ���������� ����, ��/�� (��� ����� �������������� �����������)
		static float stickLimit = 0.1; 
		//������������� ���������� ������, �� 
		static float feedLimit = 4000;
		GState gstate = GState::None;
		float feed;				//������ � ��/���
		MState mstate = MState::None;
		int speedrate;			//�������� ��������
		int n;					//����� ��������
		Collections::Generic::List<float>^ x;		//������� ���������
		Collections::Generic::List<float>^ y;
		Collections::Generic::List<float>^ z;
	private:int red, green, blue;		//�������� ����� � ������� RGB 
	public:
		bool update();
		//	bool setLastPointAsFirst(Polyline^);

		//��������� ������� ���������� �� ������ �������� ����, �������� ���������� ������ � �����,
		//� ���� ����� red, green, blue
		void stickToColor();
		static Polyline^ operator /(Polyline^, float d);
		static void setColor(Polyline^p, int r, int g, int b);

	};

}
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
		Polyline(int _n);
		//�������� ��������� ������� ��������, �������������� ��
		Polyline(int _n,
			Collections::Generic::List<float>^ _x,
			Collections::Generic::List<float>^  _y,
			Collections::Generic::List<float>^  _z) :n(n), x(_x), y(_y), z(_z) {};
		~Polyline()				//����������
		{
			delete x;
			delete y;
			delete z;
		}
	public:
		GState gstate = GState::None;
		float feed;				//������ � ��/���
		MState mstate = MState::None;
		int speedrate;			//�������� ��������
		int n;					//����� ��������
		Collections::Generic::List<float>^ x;		//������� ���������
		Collections::Generic::List<float>^ y;
		Collections::Generic::List<float>^ z;
		int red, green, blue;		//�������� ����� � ������� RGB 

		bool update();
	//	bool setLastPointAsFirst(Polyline^);

		//	static bool    operator ==(const Polyline^, const Polyline^);
		static Polyline^ operator /(Polyline^, float d);




	};

}
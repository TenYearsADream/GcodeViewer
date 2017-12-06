#pragma once
#include "Polyline.h"

namespace GData {
	using namespace System;
	using namespace System::Collections;
	using namespace System::Globalization;

	//��������� ���������� ������ ����� ������
	ref class Kadr
	{
	public:
		Kadr();

		//�����, ������������ ��� ������������� � � ������ ��������� ������, ���� true, �� ������ �����, ������� ��� ���������
		static	void reset(bool full);

		//������� ����������� �������� ������� � ��������� ������,
		// � ������� ����� polyline, ���������� ������� �� ��������������
		static	bool getPolyline(String ^ str, Polyline^ %pl);


		static	float x, y, z, i, j, k;
		static float lx=0, ly=0, lz=0; //���������� �������� ���������, ����������� ��� �������� �����������
		static	bool bg, bf,
			bm, bs,
			bx, by, bz,
			bi, bj, bk,
			fool;	//����������, ��� �� ������ �����
		static	int s = 0;
		static GState curGstate;
		static	float feedrate;
		static	float feedNoLoad;
		static	MState m;
		static	NumberFormatInfo^ formatInfo;
		static	Polyline^ polyline;

	private:
		//���������� ������, ���������� ������ ����� �� ������� �������� ������ � �������� ��������,
		//���� ������ ����� ���, �� ���������� System::String::Empty, ���� ������ ��� ����� ������ �� ������ nulptr
		static	String^ getNumeric(String^, int startIndex, int &backlastindex);

		//��������� true, ���� ������� ���������� ���������� ������ ����� result �� ������ str,
		//������� � ������� startIndex,
		//�������� ������ �� ������� ����������� �����
		static	bool getInt(String ^str, int &result, int startIndex, int &backlastindex);

		//��������� true, ���� ������� ���������� ���������� ����� � ��������� ������
		//���������� ����������� ��������� param �� ������ str, ������� � ������� startIndex,
		//�������� ������ �� ������� ����������� �����
		static	bool getFloat(String ^str, wchar_t param, int startIndex, int &backlastindex);

		static	bool isGoodNumber(wchar_t ch);
		//������������� ������ ����� � ������ ���������
		static	bool interpolate(float lx, float ly, float lz, float x, float y, float i, float j,
			Generic::List<float>^ listx, Generic::List<float>^ listy, Generic::List<float>^ listz);

		static	bool isGoodLetter(wchar_t);
	};
}

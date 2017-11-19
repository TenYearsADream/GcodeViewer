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

		//�����, ������������ ��� ������������� � � ������ ��������� ������
		static	void reset();

		//������� ����������� �������� ������� � ��������� ������,
		// � ������� ����� polyline, ���������� ������� �� ��������������
		static	bool getPolyline(String ^ str, Polyline^ %pl);
	private :

		static	float x, y, z, i, j, k, f;
	static	bool bg, bf,
			bm, bs,
			bx, by, bz,
			bi, bj, bk,
			fool;
	static	int s;
	static	GState g;
	static	MState m;
	static	NumberFormatInfo^ formatInfo;
	static	Polyline^ polyline;
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

		static	bool isGoodLetter(wchar_t);
	};
}

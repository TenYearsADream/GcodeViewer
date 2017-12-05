#include "Kadr.h"

using namespace GData;
using namespace System;
using namespace System::Collections;
using namespace System::Data;


Kadr::Kadr()
{
	this->reset(true);
}
void Kadr::reset(bool full) {
	if (full) {
		feedNoLoad = 2000;
		feedrate = 0;
		curGstate = GState::None;
		formatInfo = gcnew NumberFormatInfo();
		fool = true;
		s = 0;
	}
	bg = false;
	bm = false;
	bs = false;
	bx = false;
	by = false;
	bz = false;
	bi = false;
	bj = false;
	bk = false;
	bf = false;
}
//������� ����������� �������� ������� � ��������� ������,
// � ������� ����� polyline, ���������� ������� �� ��������������
bool Kadr::getPolyline(String^ str, Polyline^ %pl)
{
	//���� ������ �����, �� ������ � ������
	if (String::IsNullOrWhiteSpace(str)) {
		pl = gcnew Polyline(0);
		return true;
	}

	//����������, ���������
	reset(false);
	Polyline^ tpl = gcnew Polyline(1);
	int paramInt = 0;
	int curindex = 0, tmpindex = 0;

	str = str->Trim();

	while (curindex < str->Length) {
		tmpindex = curindex;

		switch (str[curindex]) {
			//���������������� (��������) �������
		case 'G':
			if (!getInt(str, paramInt, tmpindex, curindex))
				return false;
			curGstate = safe_cast<GState>(paramInt);
			bg = true;
			Console::Write("G" + paramInt);
			break;
		case 'X':
			if (!getFloat(str, 'X', tmpindex, curindex))
				return false;
			break;
		case 'Y':
			if (!getFloat(str, 'Y', tmpindex, curindex))
				return false;
			break;
		case 'Z':
			if (!getFloat(str, 'Z', tmpindex, curindex))
				return false;
			break;
			//�������� ������� ������, ��� ��������� ��������� � ������, mm/min
		case 'F':
			if (!getFloat(str, 'F', tmpindex, curindex))
				return false;
			break;
			//������� �������� ��������
		case 'S':
			if (!getInt(str, paramInt, tmpindex, curindex))
				return false;
			bs = true;
			break;
			//case P:	//�������� �������
			//case R:	//�� ������������ � ���� ������, �������� ������������ ����� ��� ������
			//case D:	//�� ������������ � ���� ������, �������� ��������� ���������� �����������
			//case L:	//�� ������������ � ���� ������, ����� ������� ������������
			//�������� ���� ��� �������� ������������. ��������������� ���������� �� ��������� ����� �� ������ ���� �� ��� X.
		case 'I':
			if (!getFloat(str, 'I', tmpindex, curindex))
				return false;
			break;
			//�������� ���� ��� �������� ������������. ��������������� ���������� �� ��������� ����� �� ������ ���� �� ��� Y
		case 'J':
			if (!getFloat(str, 'J', tmpindex, curindex))
				return false;
			break;
			//�������� ���� ��� �������� ������������. ��������������� ���������� ���� �� ��� Z.
		case 'K':
			if (!getFloat(str, 'K', tmpindex, curindex))
				return false;
			break;
			//��������������� (���������������) �������
		case'M':
			if (!getInt(str, paramInt, tmpindex, curindex)) {
				return false;
			}
			m = safe_cast<MState>(paramInt);
			bm = true;
			Console::Write("M" + paramInt);
			break;
			//����� �����������
		case 'T':
			if (!getInt(str, paramInt, tmpindex, curindex))
				return false;
			Console::Write("T" + paramInt);
			break;
		default:	//��������� �������������������� (���������) ��������
			Console::WriteLine("Wrong Symbol \"" + str + "\":" + str[curindex] + " from tmpindex:" + tmpindex + "curindex:" + curindex);
			return false;
		}
	}

	//������� ��������� � �������� ���������
	if (bg)tpl->gstate = curGstate;
	if (bm)tpl->mstate = m;
	if (bs)tpl->speedrate = s;
	if (bf)tpl->feed = feedrate;
	//���� ��� ����������-��������� � ������
	if (!(bx || by || bz || bi || bj || bk)) {

	}
	else {
		//��� ����� ���������, �������� ������ ��� ���������, ���� ��� �� ������ �� �������� ����
		//� ������� ����� ������ ���� ������, ������ ��� ��� ������ ����� �������� ������ ����
		//�� �������� ���� ����� ����������� ������ �� ���������
		if (m == MState::StartRotateClockwise || curGstate == GState::NotLoad) {
			if ((curGstate == GState::NotLoad || curGstate == GState::LineRun || curGstate == GState::CircClockwise)
				&& (curGstate == GState::NotLoad || feedrate > 0)) {
				//��������, ��� �� ��������� �� ����� ��� K������� ������������ (������ ��� �������� �� ������� �������)
				if (curGstate == GState::CircClockwise && ((bi&&bj&&bx&&by) || (bj&&bk&&by&&bz) || (bk&&bi&&bz&&bx))) {
					if (bi&&bj&&bx&&by) {

					}
					if (bj&&bk&&by&&bz) {

					}
					if (bk&&bi&&bz&&bx) {

					}
					tpl->stickToColor();
				}
				else {


					//��������, ��� �� ��������� �� ����� ��� �������� ������������
					if ((curGstate == GState::NotLoad || curGstate == GState::LineRun) && !(bi || bj || bk)) {
						if (bx)tpl->x->Insert(0, Kadr::x);
						if (by)tpl->y->Insert(0, Kadr::y);
						if (bz)tpl->z->Insert(0, Kadr::z);
						if (curGstate == GState::NotLoad) {
							Polyline::setColor(tpl, 170, 170, 170);
						}
						else {
							tpl->stickToColor();
						}
					}
					else {
						if (curGstate == GState::CircClockwise) {
							Console::WriteLine(" Wrong coor for Gstate:CircleInterpol " + str + ";");
							return false;
						}
						if (curGstate == GState::NotLoad || curGstate == GState::LineRun) {
							Console::WriteLine(" Wrong coor for Gstate:notload or linerun " + str + ";");
							return false;
						}
					}
				}
			}
			else {
				Console::WriteLine(" need GState or Feedrate " + str + ":feedrate:" + feedrate + "\n");
				return false;
			}
		}
		else {
			Console::WriteLine(" Turn on M03 before " + str + ";");
			return false;
		}
		//����� ����� ����������� ����� ����� �� �������� ������, � �������� ��������
		//��� �� �����
	}

	return true;
}

//���������� ������, ���������� ������ ����� �� ������� �������� ������ � �������� ��������,
//���� ������ ����� ���, �� ���������� System::String::Empty, ���� ������ ��� ����� ������ �� ������ nulptr
//� backlastindex ���������� ������ �� ������� ����������� ����� �����
String ^ Kadr::getNumeric(String ^str, int startIndex, int &backlastindex)
{
	int start = startIndex, end = startIndex;
	bool wasdot = false;
	while (start < str->Length && Kadr::isGoodLetter(str[start]))
	{
		start++;
	}
	end = start;
	//���� �� �� ��������� �� ����� ������
	if (end < str->Length) {
		end++;
		//���� ���������� �� �����, ������� ���, ��� �� ������ ������
		if (str[start] == '.') wasdot = true;
	}
	// (��������� end �� ��������� ������) � (������ �� end �� ������) � 
	// � ( ������ �������� ������ ��� ( (  ������ �������� ������ ) �����������_��� ����� � ����� ��� ���������� � �����) )
	//��������� �������� ��� ����������� ���� ����� � �����
	while (end < str->Length && (((str[end] == '.') && (!wasdot)) ^ Kadr::isGoodNumber(str[end]))) {
		//while (end < str->Length && !Char::IsWhiteSpace(str[end]) && !(str[end] == 'Y') && !(str[end] == 'Z')
		//	&& !(str[end] == 'F') && !(str[end] == 'S') && (((str[end] == '.') ^ wasdot) || Char::IsDigit(str[end]))) {
		if (str[end] == '.') wasdot = true;
		end++;
	}

	//��� �� ���� ����� ������?
	if (end < str->Length && str[end] == '.') {
		Console::Write("parseFloat:nulptr:dot id:" + start + "-" + end + " str-substr: " + str + "-" + str->Substring(start, end - start));
		return nullptr;
	}
	backlastindex = end;
	return str->Substring(start, end - start);
}

//��������� true, ���� ������� ���������� ���������� ������ ����� result �� ������ str,
//������� � ������� startIndex,
//�������� ������ �� ������� ����������� �����
bool Kadr::getInt(String ^str, int &result, int startIndex, int &backlastindex) {
	//������� ������ ���������� �����
	String^ resultStr = Kadr::getNumeric(str, startIndex, backlastindex);
	//���� ������ ������, �� �������� ��������, �� ����, ���� ������� � ����������� ������� ��� �����
	if (System::String::Empty->Equals(resultStr)) {
		Console::WriteLine("not get int \"" + str + "\"is empty. index " + startIndex);
		Kadr::reset(false);
		backlastindex = str->Length;
		return false;
	}
	//���� ������ ������ ������� nulptr
	if (resultStr == nullptr) {
		Console::WriteLine(" Cannot get int (nulptr)\"" + str + "\" index " + startIndex + "too many dots");
		Kadr::reset(false);
		backlastindex = str->Length;
		return false;
	}
	//����� �������� ����� startindex+1, ������, ���� ��������� ������ �������� � �� ����������� �����,
	//�� ���� ��������� ����� ����� ����� �������� �� startIndex-1, ��� ��������� ������
	if (backlastindex - startIndex - 1 > resultStr->Length) {
		Console::WriteLine(" int is not that whitch  expected\"" + str + "\"need int by index " + startIndex);
		Kadr::reset(false);
		backlastindex = str->Length;
		return false;
	}
	//�������� ��������� ���������� ����� ��������, ������
	if (!System::Int32::TryParse(resultStr, NumberStyles::Integer, formatInfo, result)) {
		Console::WriteLine("Falture to TryParse for int");
		Kadr::reset(false);
		backlastindex = str->Length;
		return false;
	}
	return true;
}

//��������� true, ���� ������� ���������� ���������� ����� � ��������� ������
//���������� ����������� ��������� param �� ������ str, ������� � ������� startIndex,
//�������� ������ �� ������� ����������� �����
bool Kadr::getFloat(String ^str, wchar_t param, int startIndex, int &backlastindex) {
	float result = 0;
	//������� ������ ���������� �����
	String^ resultStr = Kadr::getNumeric(str, startIndex, backlastindex);
	//���� ������ ������, �� �������� ��������, �� ����, ���� ������� � ����������� ������� ��� �����
	if (System::String::Empty->Equals(resultStr)) {
		Console::WriteLine("not get float \"" + str + "\"is empty. index " + startIndex);
		Kadr::reset(false);
		backlastindex = str->Length;
		return false;
	}
	//���� ������ ������ ������� nulptr
	if (resultStr == nullptr) {
		Console::WriteLine(" Cannot get int (nulptr)\"" + str + "\" index " + startIndex + "too many dots");
		Kadr::reset(false);
		backlastindex = str->Length;
		return false;
	}

	//�������� ��������� ���������� ����� ��������, ������
	if (!System::Single::TryParse(resultStr, NumberStyles::AllowDecimalPoint | NumberStyles::AllowLeadingSign, formatInfo, result)) {
		Console::WriteLine("Falture to TryParse for float\"" + str + "\"-" + resultStr);
		Kadr::reset(false);
		backlastindex = str->Length;
		return false;
	}
	switch (param) {
	case 'X':
		x = result;
		bx = true;
		Console::Write("X" + x);
		break;
	case 'Y':
		y = result;
		by = true;
		Console::Write("Y" + y);
		break;
	case 'Z':
		z = result;
		bz = true;
		Console::Write("Z" + z);
		break;
		//�������� �������
//	case 'P':	
	case 'F':
		if (result > 0) {
			if (curGstate != GState::None)
				feedrate = result;
			bf = true;
			Console::Write("F" + feedrate);
			break;
		}
		else {
			Console::WriteLine("F.Error<=0");
			Kadr::reset(false);
			backlastindex = str->Length;
			return false;
		}
		//������� �������� ��������

		//case R:	//�� ������������ � ���� ������, �������� ������������ ����� ��� ������
		//case D:	//�� ������������ � ���� ������, �������� ��������� ���������� �����������
		//case L:	//�� ������������ � ���� ������, ����� ������� ������������
		//�������� ���� ��� �������� ������������. ��������������� ���������� �� ��������� ����� �� ������ ���� �� ��� X.
	case 'I':
		i = result;
		bi = true;
		Console::Write("I" + i);
		break;
		//�������� ���� ��� �������� ������������. ��������������� ���������� �� ��������� ����� �� ������ ���� �� ��� Y
	case 'J':
		j = result;
		bj = true;
		Console::Write("J" + j);
		break;
		//�������� ���� ��� �������� ������������. ��������������� ���������� ���� �� ��� Z.
	case 'K':
		k = result;
		bk = true;
		Console::Write("K" + k);
		break;
	default:	//��������� �������������������� (���������) ��������
		Console::WriteLine(" wrong param for get result \"" + str + "\" param: " + param + ".");
		Kadr::reset(false);
		backlastindex = str->Length;
		return false;
	}
	return true;
}

bool GData::Kadr::isGoodLetter(wchar_t ch)
{	// :88-X;:89-Y;:90-Z;:73-I;:74-J;:75-K;:71-G;:77-M;:70-F;:83-S;:45--;:46-.;
	bool res = (
		(ch == 'X') ||
		(ch == 'Y') ||
		(ch == 'Z') ||
		(ch == 'I') ||	// (safe_cast<Int16>(ch) == 73) ||
		(ch == 'J') ||	// (safe_cast<Int16>(ch) == 74) ||
		(ch == 'K') ||	// (safe_cast<Int16>(ch) == 75) ||
		(ch == 'F') ||
		(ch == 'S') ||
		(ch == 'M') ||
		(ch == 'G') ||
		(ch == 'T'));
	//bool res = ((ch == 'Y') || Char::IsLetter(ch)) && !(ch == '.');
	return res;
}
bool Kadr::isGoodNumber(wchar_t ch) {
	bool res = ((safe_cast<Int16>(ch) >= 48) && (safe_cast<Int16>(ch) <= 57)) || (safe_cast<Int16>(ch) == 45);
	//bool res = ((ch == 'Y') || Char::IsLetter(ch)) && !(ch == '.');
	return res;
}


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
		g = gcnew Collections::Generic::List<GState>(0);
		f = gcnew Collections::Generic::List<float>(0);
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
	Polyline^ tpl = gcnew Polyline(0);
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
			if (!g->Contains(safe_cast<GState>(paramInt))) {
				g->Add(safe_cast<GState>(paramInt));
				f->Insert(g->IndexOf(safe_cast<GState>(paramInt)),0);
			//	f[g->IndexOf(safe_cast<GState>(paramInt))] = 0;
			}
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

	if (!(bx || by || bz || bi || bj || bk)) {
		if (bg)tpl->gstate = curGstate;
		if (bm)tpl->mstate = m;
		if (bs)tpl->speedrate = s;
		if (bf)tpl->feed = f[g->IndexOf(curGstate)];
	}
	else {

		//����������� ������������ ������, �������� �������� � ������ �������� ������� ����������
		if (pl->feed != f[g->IndexOf(curGstate)] || pl->speedrate != s || pl->gstate != curGstate) {
			tpl = gcnew Polyline(0);
			//����� ��������� ����� � ���������� ����� ��� ��������� ��� �����
	//		tpl->setLastPointAsFirst(pl);
			//������������� ������� ���������
			tpl->gstate = curGstate;
			if (bm)tpl->mstate = m;
			if (bs)tpl->speedrate = s;
			if (bf)tpl->feed = f[g->IndexOf(curGstate)];

			//�������� ������������ � ��������� 
			if (!(bi&&bj&&bk) && (curGstate == GState::LineRun || curGstate == GState::LineRun)) {
				if (bx&&by&&bz) {
					if (bx)	tpl->x->Insert(tpl->x->Count, x); else tpl->x->Insert(tpl->x->Count, tpl->x[tpl->x->Count - 1]);
					if (by)	tpl->y->Insert(tpl->y->Count, y); else tpl->y->Insert(tpl->y->Count, tpl->y[tpl->y->Count - 1]);
					if (bz)	tpl->z->Insert(tpl->z->Count, z); else tpl->z->Insert(tpl->z->Count, tpl->z[tpl->z->Count - 1]);
				}
			}
			if (((bi&&bj) || (bj&&bk) || (bk&&bi)) && ((bx&&by) || (by&&bz) || (bz&&bx)) &&
				(curGstate == GState::CircClockwise || curGstate == GState::CircCntrClockwise)) {
			}
			else {
			}
		}
	}

	return true;
}

//���������� ������, ���������� ������ ����� �� ������� �������� ������ � �������� ��������,
//���� ������ ����� ���, �� ���������� System::String::Empty, ���� ������ ��� ����� ������ �� ������ nulptr
//� backlastindex ���������� ������ �� ������� ����������� ����� �����
String ^ Kadr::getNumeric(String ^str, int startIndex, int &backlastindex)
{
	int start = startIndex, end = startIndex;
	bool isdot = false;
	while (start < str->Length && Kadr::isGoodLetter(str[start]))
	{
		start++;
	}
	end = start;
	//���� �� �� ��������� �� ����� ������
	if (end < str->Length) {
		end++;
		//���� ���������� �� �����, ������� ���, ��� �� ������ ������
		if (str[start] == '.') isdot = true;
	}
	// (��������� end �� ��������� ������) � (������ �� end �� ������) � 
	// � ( ������ �������� ������ ��� ( (  ������ �������� ������ ) �����������_��� ����� � ����� ��� ���������� � �����) )
	//��������� �������� ��� ����������� ���� ����� � �����
	while (end < str->Length && !Char::IsWhiteSpace(str[end]) && !(str[end] == 'Y') && !(str[end] == 'Z')
		&& !(str[end] == 'F') && !(str[end] == 'S') && (((str[end] == '.') ^ isdot) || Char::IsDigit(str[end]))) {
		if (str[end] == '.') isdot = true;
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
		if (curGstate != GState::None)
			f[g->IndexOf(curGstate)] = result;
		bf = true;
		Console::Write("F" + f[g->IndexOf(curGstate)]);
		break;
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
{
	bool res = (
		(ch == 'X') ||
		(ch == 'Y') ||
		(ch == 'Z') ||
		(ch == 'I') ||
		(ch == 'J') ||
		(ch == 'K') ||
		(ch == 'F') ||
		(ch == 'S') ||
		(ch == 'M') ||
		(ch == 'G') ||
		(ch == 'T'));
	//bool res = ((ch == 'Y') || Char::IsLetter(ch)) && !(ch == '.');
	return res;
}


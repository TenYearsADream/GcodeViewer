#pragma once
namespace GLArea {
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace OpenTK;
using namespace OpenTK::Platform::Windows;
using namespace OpenTK::Graphics::OpenGL;

	ref class GLArea
	{
	public:
		GLArea(OpenTK::GLControl^  glControl);
		~GLArea();

		static void changeModelView();
		static void rotateG(double U);
		static void rotateV(double U);

		static bool glLoaded = false;
		static Matrix4 modelview;
		static int w = 9;
		static float multiplyEye = 1.175f;
		static float eyeX = 300, eyeY = -400, eyeZ = 500;
		static float  targetX = 0, targetY = 0, targetZ = 0;
		static float upX = 0, upY = 0, upZ = 24;
		static double rXY, rXYZ, pi = Math::Acos(-1.0);
		static bool mdown = false;	//������ ���� ������
		static int mdX, mdY;		//���������� ������� ����
		static int mX, mY;			//������� ���������� ����
		static double startAngG = 2.5, startAngV = pi / 4; //��������� ���� ������
		static double angleG = 0, angleV = 0;//���� ��� �������� ������ �� ������� ����, �������������� � ������������
		static OpenTK::GLControl^  glCtrl;
	};

System::Void gl_mouseWheel(System::Object ^ sender, System::Windows::Forms::MouseEventArgs ^ e);
System::Void gl_Load(System::Object ^ sender, System::EventArgs ^ e);
System::Void gl_Paint(System::Object ^ sender, System::Windows::Forms::PaintEventArgs ^ e);
System::Void gl_mouseMove(System::Object ^ sender, System::Windows::Forms::MouseEventArgs ^ e);
System::Void gl_mouseDown(System::Object ^ sender, System::Windows::Forms::MouseEventArgs ^ e);
System::Void gl_mouseUp(System::Object ^ sender, System::Windows::Forms::MouseEventArgs ^ e);
};
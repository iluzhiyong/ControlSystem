#include "StdAfx.h"
#include "HalconAction.h"
#include "DetectCircularhole.h"
#include "DataUtility.h"


#define MINGRAY			_T("MinGray")
#define	MAXGRAY			_T("MaxGray")
#define	MINCIRLEAREA	_T("MinCirleArea")
#define	MAXCIRLEAREA	_T("MaxCirleArea")
#define	MINROUNDNESS	_T("MinRoundness")
#define	MAXROUNDNESS	_T("MaxRoundness")

#define SPECIALMINGRAY			_T("SpecialMinGray")
#define	SPECIALMAXGRAY			_T("SpecialMaxGray")
#define	SPECIALMINCIRLEAREA		_T("SpecialMinCirleArea")
#define	SPECIALMAXCIRLEAREA		_T("SpecialMaxCirleArea")
#define	SPECIALMINROUNDNESS		_T("SpecialMinRoundness")
#define	SPECIALMAXROUNDNESS		_T("SpecialMaxRoundness")


CDetectCircularhole::CDetectCircularhole(int type)
	: m_MinGray(128)
	, m_MaxGray(255)
	, m_MinCirleArea(0)
	, m_MaxCirleArea(999999.0)
	, m_MinRoundness(0.5)
	, m_MaxRoundness(1.0)
	, m_detectType(type)
{
	
	m_ShowErrorMeg = true;
	m_ShowProcessingImage = true;

	m_ConfigPath = DataUtility::GetExePath() + _T("\\ProcessConfig\\ImageProcess.ini");
	LoadConfig();
}



CDetectCircularhole::~CDetectCircularhole(void)
{
}

void CDetectCircularhole::SetImageObject(Hobject image)
{
	ho_Capture1 = image;
	Halcon::get_image_size(ho_Capture1, &hv_imageWidth, &hv_imageHeight);
}

bool CDetectCircularhole::DetectCirleCenter(float &row, float &column)
{
	if(RunSelectTarget())
	{
		row = (float)(hv_targetRow[0].D());
		column = (float)(hv_targetColumn[0].D());
		return true;
	}
	return false;
}

void CDetectCircularhole::ShowErrorMessage(bool show)
{
	m_ShowErrorMeg = show;
}

void CDetectCircularhole::ShowMessage(CString msg)
{
	if(m_ShowErrorMeg)
	{
		AfxMessageBox(msg);
	}
}

bool CDetectCircularhole::RunThreshold()
{
	try
	{
		threshold(ho_Capture1, &ho_Region, m_MinGray, m_MaxGray);
		if (m_ShowProcessingImage && HDevWindowStack::IsOpen())
		{
			clear_window(HDevWindowStack::GetActive());
			set_color(HDevWindowStack::GetActive(),"red");
			disp_obj(ho_Region, HDevWindowStack::GetActive());
			connection(ho_Region, &ho_ConnectedRegions);

			//为了设定参数方便，显示面积在m_MinCirleArea/10~m_MaxCirleArea*10（Pixel）的区域面积值
			Hobject selectRegions;
			HTuple RegionCount, Area, Row, Column;
			select_shape(ho_ConnectedRegions, &selectRegions, "area", "and", m_MinCirleArea/10, m_MaxCirleArea*10);
			count_obj(selectRegions, &RegionCount);
			area_center(selectRegions, &Area, &Row, &Column);
			set_color(HDevWindowStack::GetActive(),"blue");
			for (int i=1; i<=RegionCount; i+=1)
			{
				set_tposition(HDevWindowStack::GetActive(), HTuple(Row[i-1]), HTuple(Column[i-1]));
				write_string(HDevWindowStack::GetActive(), Area[i-1].D());
			}
		}
	}
	catch(...)
	{
		ShowMessage("二值化失败！");
		return false;
	}
	return true;
}

bool CDetectCircularhole::RunSelectTarget()
{
	if(RunThreshold())
	{
		try
		{
			select_shape(ho_ConnectedRegions, &ho_SelectedRegions, (HTuple("area").Append("roundness")), "and", (HTuple(m_MinCirleArea).Append(m_MinRoundness)), (HTuple(m_MaxCirleArea).Append(m_MaxRoundness)));
			
			fill_up(ho_SelectedRegions, &ho_RegionFillUp);
			gen_contour_region_xld(ho_RegionFillUp, &ho_Contour, "border");

			HTuple row1, column1, row2, column2, length;
			diameter_region(ho_RegionFillUp, &row1, &column1, &row2, &column2, &length);
			hv_targetRow = (row1 + row2) / 2;
			hv_targetColumn = (column1 + column2) / 2;

			if (m_ShowProcessingImage && HDevWindowStack::IsOpen())
			{
				clear_window(HDevWindowStack::GetActive());
				set_color(HDevWindowStack::GetActive(),"red");
				disp_obj(ho_Contour, HDevWindowStack::GetActive());

				set_color(HDevWindowStack::GetActive(),"red");
				disp_cross(HDevWindowStack::GetActive(), hv_targetRow, hv_targetColumn, 20, 0);
				set_color(HDevWindowStack::GetActive(),"green");
				disp_cross(HDevWindowStack::GetActive(), hv_imageHeight / 2, hv_imageWidth / 2, 20, 0);
			}
			return true;
		}
		catch(...)
		{
			ShowMessage("检测圆形区域失败！");
		}
	}
	return false;
}

void CDetectCircularhole::LoadConfig()
{
	if(CIRCLE_DETECT_TYPE_SPECIAL == m_detectType)
	{
		m_MinGray = DataUtility::GetProfileInt(_T("Special Circle Threshold"), SPECIALMINGRAY, m_ConfigPath, 0);
		m_MaxGray = DataUtility::GetProfileInt(_T("Special Circle Threshold"), SPECIALMAXGRAY, m_ConfigPath, 100);
		m_MinCirleArea = DataUtility::GetProfileFloat(_T("Special Circle Area"), SPECIALMINCIRLEAREA, m_ConfigPath, 150000.0);
		m_MaxCirleArea = DataUtility::GetProfileFloat(_T("Special Circle Area"), SPECIALMAXCIRLEAREA, m_ConfigPath, 300000.0);
		m_MinRoundness = DataUtility::GetProfileFloat(_T("Special Circle Roundness"), SPECIALMINROUNDNESS, m_ConfigPath, 0.5);
		m_MaxRoundness = DataUtility::GetProfileFloat(_T("Special Circle Roundness"), SPECIALMAXROUNDNESS, m_ConfigPath, 1.0);
	}
	else
	{
		m_MinGray = DataUtility::GetProfileInt(_T("Circle Threshold"), MINGRAY, m_ConfigPath, 0);
		m_MaxGray = DataUtility::GetProfileInt(_T("Circle Threshold"), MAXGRAY, m_ConfigPath, 100);
		m_MinCirleArea = DataUtility::GetProfileFloat(_T("Circle Area"), MINCIRLEAREA, m_ConfigPath, 150000.0);
		m_MaxCirleArea = DataUtility::GetProfileFloat(_T("Circle Area"), MAXCIRLEAREA, m_ConfigPath, 300000.0);
		m_MinRoundness = DataUtility::GetProfileFloat(_T("Circle Roundness"), MINROUNDNESS, m_ConfigPath, 0.5);
		m_MaxRoundness = DataUtility::GetProfileFloat(_T("Circle Roundness"), MAXROUNDNESS, m_ConfigPath, 1.0);
	}
}

void CDetectCircularhole::SaveConfig()
{
	if(CIRCLE_DETECT_TYPE_SPECIAL == m_detectType)
	{
		DataUtility::SetProfileInt(_T("Special Threshold"), SPECIALMINGRAY, m_ConfigPath, m_MinGray);
		DataUtility::SetProfileInt(_T("Special Threshold"), SPECIALMAXGRAY, m_ConfigPath, m_MaxGray);
		DataUtility::SetProfileFloat(_T("Special Circle Area"), SPECIALMINCIRLEAREA, m_ConfigPath, m_MinCirleArea);
		DataUtility::SetProfileFloat(_T("Special Circle Area"), SPECIALMAXCIRLEAREA, m_ConfigPath, m_MaxCirleArea);
		DataUtility::SetProfileFloat(_T("Special Circle Roundness"), SPECIALMINROUNDNESS, m_ConfigPath, m_MinRoundness);
		DataUtility::SetProfileFloat(_T("Special Circle Roundness"), SPECIALMAXROUNDNESS, m_ConfigPath, m_MaxRoundness);
	}
	else
	{
		DataUtility::SetProfileInt(_T("Threshold"), MINGRAY, m_ConfigPath, m_MinGray);
		DataUtility::SetProfileInt(_T("Threshold"), MAXGRAY, m_ConfigPath, m_MaxGray);
		DataUtility::SetProfileFloat(_T("Circle Area"), MINCIRLEAREA, m_ConfigPath, m_MinCirleArea);
		DataUtility::SetProfileFloat(_T("Circle Area"), MAXCIRLEAREA, m_ConfigPath, m_MaxCirleArea);
		DataUtility::SetProfileFloat(_T("Circle Roundness"), MINROUNDNESS, m_ConfigPath, m_MinRoundness);
		DataUtility::SetProfileFloat(_T("Circle Roundness"), MAXROUNDNESS, m_ConfigPath, m_MaxRoundness);
	}
}

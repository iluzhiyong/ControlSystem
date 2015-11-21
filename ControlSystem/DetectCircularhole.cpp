#include "StdAfx.h"
#include "HalconAction.h"
#include "DetectCircularhole.h"
#include "DataUtility.h"


#define MINGRAY			_T("MinGray")
#define	MAXGRAY			_T("MaxGray")
#define	MINAREA			_T("MinArea")
#define	MAXAREA			_T("MaxArea")
#define	MINROUNDNESS	_T("MinRoundness")
#define	MAXROUNDNESS	_T("MaxRoundness")

#define STANDARDCIRLEGRAY			_T("Standard Circle Gray")
#define	STANDARDCIRLEAREA			_T("Standard Circle Area")
#define	STANDARDCIRLEROUNDNESS		_T("Standard Circle Roundness")

#define OBLONGCIRLEGRAY				_T("Oblong Circle Gray")
#define	OBLONGCIRLEAREA				_T("Oblong Circle Area")
#define	OBLONGCIRLEROUNDNESS		_T("Oblong Circle Roundness")

#define FIXTURECIRLEGRAY			_T("Fixture Circle Gray")
#define	FIXTURECIRLEAREA			_T("Fixture Circle Area")
#define	FIXTURECIRLEROUNDNESS		_T("Fixture Circle Roundness")

#define SPECIALCIRLEGRAY			_T("Special Circle Gray")
#define	SPECIALCIRLEAREA			_T("Special Circle Area")
#define	SPECIALCIRLEROUNDNESS		_T("Special Circle Roundness")

#define CUSTOM1GRAY					_T("Custom1 Gray")
#define CUSTOM1AREA					_T("Custom1 Area")
#define CUSTOM1ROUNDNESS			_T("Custom1 Roundness")

#define CUSTOM2GRAY					_T("Custom2 Gray")
#define CUSTOM2AREA					_T("Custom2 Area")
#define CUSTOM2ROUNDNESS			_T("Custom2 Roundness")

#define CUSTOM3GRAY					_T("Custom3 Gray")
#define CUSTOM3AREA					_T("Custom3 Area")
#define CUSTOM3ROUNDNESS			_T("Custom3 Roundness")

#define CUSTOM4GRAY					_T("Custom4 Gray")
#define CUSTOM4AREA					_T("Custom4 Area")
#define CUSTOM4ROUNDNESS			_T("Custom4 Roundness")

#define CUSTOM5GRAY					_T("Custom5 Gray")
#define CUSTOM5AREA					_T("Custom5 Area")
#define CUSTOM5ROUNDNESS			_T("Custom5 Roundness")

#define CUSTOM6GRAY					_T("Custom6 Gray")
#define CUSTOM6AREA					_T("Custom6 Area")
#define CUSTOM6ROUNDNESS			_T("Custom6 Roundness")

#define CUSTOM7GRAY					_T("Custom7 Gray")
#define CUSTOM7AREA					_T("Custom7 Area")
#define CUSTOM7ROUNDNESS			_T("Custom7 Roundness")

#define CUSTOM8GRAY					_T("Custom8 Gray")
#define CUSTOM8AREA					_T("Custom8 Area")
#define CUSTOM8ROUNDNESS			_T("Custom8 Roundness")

#define CUSTOM9GRAY					_T("Custom9 Gray")
#define CUSTOM9AREA					_T("Custom9 Area")
#define CUSTOM9ROUNDNESS			_T("Custom9 Roundness")

#define CUSTOM10GRAY				_T("Custom10 Gray")
#define CUSTOM10AREA				_T("Custom10 Area")
#define CUSTOM10ROUNDNESS			_T("Custom10 Roundness")

#define CUSTOM11GRAY				_T("Custom11 Gray")
#define CUSTOM11AREA				_T("Custom11 Area")
#define CUSTOM11ROUNDNESS			_T("Custom11 Roundness")

#define CUSTOM12GRAY				_T("Custom12 Gray")
#define CUSTOM12AREA				_T("Custom12 Area")
#define CUSTOM12ROUNDNESS			_T("Custom12 Roundness")

#define CUSTOM13GRAY				_T("Custom13 Gray")
#define CUSTOM13AREA				_T("Custom13 Area")
#define CUSTOM13ROUNDNESS			_T("Custom13 Roundness")

#define CUSTOM14GRAY				_T("Custom14 Gray")
#define CUSTOM14AREA				_T("Custom14 Area")
#define CUSTOM14ROUNDNESS			_T("Custom14 Roundness")

#define CUSTOM15GRAY				_T("Custom15 Gray")
#define CUSTOM15AREA				_T("Custom15 Area")
#define CUSTOM15ROUNDNESS			_T("Custom15 Roundness")

CDetectCircularhole::CDetectCircularhole()
	: m_MinGray(128)
	, m_MaxGray(255)
	, m_MinCirleArea(0)
	, m_MaxCirleArea(999999.0)
	, m_MinRoundness(0.5)
	, m_MaxRoundness(1.0)
	, m_detectType(CIRCLE_DETECT_TYPE_NORMAL)
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
		Halcon::get_image_size(ho_Capture1, &hv_imageWidth, &hv_imageHeight);
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
			//算法1：通过计算两条最远线的交点，计算区域中心，适用于不标准图形
			diameter_region(ho_RegionFillUp, &row1, &column1, &row2, &column2, &length);
			hv_targetRow = (row1 + row2) / 2;
			hv_targetColumn = (column1 + column2) / 2;
			//算法2：直接利用算子求区域的中心，适用于比较标准的图形
			//area_center(ho_RegionFillUp, &hv_Area, &hv_targetRow, &hv_targetColumn);

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
	switch(m_detectType)
	{
	case CIRCLE_DETECT_TYPE_NORMAL:
		m_MinGray = DataUtility::GetProfileInt(STANDARDCIRLEGRAY, MINGRAY, m_ConfigPath, 0);
		m_MaxGray = DataUtility::GetProfileInt(STANDARDCIRLEGRAY, MAXGRAY, m_ConfigPath, 100);
		m_MinCirleArea = DataUtility::GetProfileFloat(STANDARDCIRLEAREA, MINAREA, m_ConfigPath, 150000.0);
		m_MaxCirleArea = DataUtility::GetProfileFloat(STANDARDCIRLEAREA, MAXAREA, m_ConfigPath, 300000.0);
		m_MinRoundness = DataUtility::GetProfileFloat(STANDARDCIRLEROUNDNESS, MINROUNDNESS, m_ConfigPath, 0.5);
		m_MaxRoundness = DataUtility::GetProfileFloat(STANDARDCIRLEROUNDNESS, MAXROUNDNESS, m_ConfigPath, 1.0);
		break;

	case CIRCLE_DETECT_TYPE_OBLONG:
		m_MinGray = DataUtility::GetProfileInt(OBLONGCIRLEGRAY, MINGRAY, m_ConfigPath, 0);
		m_MaxGray = DataUtility::GetProfileInt(OBLONGCIRLEGRAY, MAXGRAY, m_ConfigPath, 100);
		m_MinCirleArea = DataUtility::GetProfileFloat(OBLONGCIRLEAREA, MINAREA, m_ConfigPath, 150000.0);
		m_MaxCirleArea = DataUtility::GetProfileFloat(OBLONGCIRLEAREA, MAXAREA, m_ConfigPath, 300000.0);
		m_MinRoundness = DataUtility::GetProfileFloat(OBLONGCIRLEROUNDNESS, MINROUNDNESS, m_ConfigPath, 0.5);
		m_MaxRoundness = DataUtility::GetProfileFloat(OBLONGCIRLEROUNDNESS, MAXROUNDNESS, m_ConfigPath, 1.0);
		break;

	case CIRCLE_DETECT_TYPE_FIXTURE:
		m_MinGray = DataUtility::GetProfileInt(FIXTURECIRLEGRAY, MINGRAY, m_ConfigPath, 0);
		m_MaxGray = DataUtility::GetProfileInt(FIXTURECIRLEGRAY, MAXGRAY, m_ConfigPath, 100);
		m_MinCirleArea = DataUtility::GetProfileFloat(FIXTURECIRLEAREA, MINAREA, m_ConfigPath, 150000.0);
		m_MaxCirleArea = DataUtility::GetProfileFloat(FIXTURECIRLEAREA, MAXAREA, m_ConfigPath, 300000.0);
		m_MinRoundness = DataUtility::GetProfileFloat(FIXTURECIRLEROUNDNESS, MINROUNDNESS, m_ConfigPath, 0.5);
		m_MaxRoundness = DataUtility::GetProfileFloat(FIXTURECIRLEROUNDNESS, MAXROUNDNESS, m_ConfigPath, 1.0);
		break;

	case CIRCLE_DETECT_TYPE_SPECIAL:
		m_MinGray = DataUtility::GetProfileInt(SPECIALCIRLEGRAY, MINGRAY, m_ConfigPath, 0);
		m_MaxGray = DataUtility::GetProfileInt(SPECIALCIRLEGRAY, MAXGRAY, m_ConfigPath, 100);
		m_MinCirleArea = DataUtility::GetProfileFloat(SPECIALCIRLEAREA, MINAREA, m_ConfigPath, 150000.0);
		m_MaxCirleArea = DataUtility::GetProfileFloat(SPECIALCIRLEAREA, MAXAREA, m_ConfigPath, 300000.0);
		m_MinRoundness = DataUtility::GetProfileFloat(SPECIALCIRLEROUNDNESS, MINROUNDNESS, m_ConfigPath, 0.5);
		m_MaxRoundness = DataUtility::GetProfileFloat(SPECIALCIRLEROUNDNESS, MAXROUNDNESS, m_ConfigPath, 1.0);
		break;

	case CIRCLE_DETECT_TYPE_CUSTOM1:
		m_MinGray = DataUtility::GetProfileInt(CUSTOM1GRAY, MINGRAY, m_ConfigPath, 0);
		m_MaxGray = DataUtility::GetProfileInt(CUSTOM1GRAY, MAXGRAY, m_ConfigPath, 100);
		m_MinCirleArea = DataUtility::GetProfileFloat(CUSTOM1AREA, MINAREA, m_ConfigPath, 150000.0);
		m_MaxCirleArea = DataUtility::GetProfileFloat(CUSTOM1AREA, MAXAREA, m_ConfigPath, 300000.0);
		m_MinRoundness = DataUtility::GetProfileFloat(CUSTOM1ROUNDNESS, MINROUNDNESS, m_ConfigPath, 0.5);
		m_MaxRoundness = DataUtility::GetProfileFloat(CUSTOM1ROUNDNESS, MAXROUNDNESS, m_ConfigPath, 1.0);
		break;

	case CIRCLE_DETECT_TYPE_CUSTOM2:
		m_MinGray = DataUtility::GetProfileInt(CUSTOM2GRAY, MINGRAY, m_ConfigPath, 0);
		m_MaxGray = DataUtility::GetProfileInt(CUSTOM2GRAY, MAXGRAY, m_ConfigPath, 100);
		m_MinCirleArea = DataUtility::GetProfileFloat(CUSTOM2AREA, MINAREA, m_ConfigPath, 150000.0);
		m_MaxCirleArea = DataUtility::GetProfileFloat(CUSTOM2AREA, MAXAREA, m_ConfigPath, 300000.0);
		m_MinRoundness = DataUtility::GetProfileFloat(CUSTOM2ROUNDNESS, MINROUNDNESS, m_ConfigPath, 0.5);
		m_MaxRoundness = DataUtility::GetProfileFloat(CUSTOM2ROUNDNESS, MAXROUNDNESS, m_ConfigPath, 1.0);
		break;

	case CIRCLE_DETECT_TYPE_CUSTOM3:
		m_MinGray = DataUtility::GetProfileInt(CUSTOM3GRAY, MINGRAY, m_ConfigPath, 0);
		m_MaxGray = DataUtility::GetProfileInt(CUSTOM3GRAY, MAXGRAY, m_ConfigPath, 100);
		m_MinCirleArea = DataUtility::GetProfileFloat(CUSTOM3AREA, MINAREA, m_ConfigPath, 150000.0);
		m_MaxCirleArea = DataUtility::GetProfileFloat(CUSTOM3AREA, MAXAREA, m_ConfigPath, 300000.0);
		m_MinRoundness = DataUtility::GetProfileFloat(CUSTOM3ROUNDNESS, MINROUNDNESS, m_ConfigPath, 0.5);
		m_MaxRoundness = DataUtility::GetProfileFloat(CUSTOM3ROUNDNESS, MAXROUNDNESS, m_ConfigPath, 1.0);
		break;

	case CIRCLE_DETECT_TYPE_CUSTOM4:
		m_MinGray = DataUtility::GetProfileInt(CUSTOM4GRAY, MINGRAY, m_ConfigPath, 0);
		m_MaxGray = DataUtility::GetProfileInt(CUSTOM4GRAY, MAXGRAY, m_ConfigPath, 100);
		m_MinCirleArea = DataUtility::GetProfileFloat(CUSTOM4AREA, MINAREA, m_ConfigPath, 150000.0);
		m_MaxCirleArea = DataUtility::GetProfileFloat(CUSTOM4AREA, MAXAREA, m_ConfigPath, 300000.0);
		m_MinRoundness = DataUtility::GetProfileFloat(CUSTOM4ROUNDNESS, MINROUNDNESS, m_ConfigPath, 0.5);
		m_MaxRoundness = DataUtility::GetProfileFloat(CUSTOM4ROUNDNESS, MAXROUNDNESS, m_ConfigPath, 1.0);
		break;

	case CIRCLE_DETECT_TYPE_CUSTOM5:
		m_MinGray = DataUtility::GetProfileInt(CUSTOM5GRAY, MINGRAY, m_ConfigPath, 0);
		m_MaxGray = DataUtility::GetProfileInt(CUSTOM5GRAY, MAXGRAY, m_ConfigPath, 100);
		m_MinCirleArea = DataUtility::GetProfileFloat(CUSTOM5AREA, MINAREA, m_ConfigPath, 150000.0);
		m_MaxCirleArea = DataUtility::GetProfileFloat(CUSTOM5AREA, MAXAREA, m_ConfigPath, 300000.0);
		m_MinRoundness = DataUtility::GetProfileFloat(CUSTOM5ROUNDNESS, MINROUNDNESS, m_ConfigPath, 0.5);
		m_MaxRoundness = DataUtility::GetProfileFloat(CUSTOM5ROUNDNESS, MAXROUNDNESS, m_ConfigPath, 1.0);
		break;

	case CIRCLE_DETECT_TYPE_CUSTOM6:
		m_MinGray = DataUtility::GetProfileInt(CUSTOM6GRAY, MINGRAY, m_ConfigPath, 0);
		m_MaxGray = DataUtility::GetProfileInt(CUSTOM6GRAY, MAXGRAY, m_ConfigPath, 100);
		m_MinCirleArea = DataUtility::GetProfileFloat(CUSTOM6AREA, MINAREA, m_ConfigPath, 150000.0);
		m_MaxCirleArea = DataUtility::GetProfileFloat(CUSTOM6AREA, MAXAREA, m_ConfigPath, 300000.0);
		m_MinRoundness = DataUtility::GetProfileFloat(CUSTOM6ROUNDNESS, MINROUNDNESS, m_ConfigPath, 0.5);
		m_MaxRoundness = DataUtility::GetProfileFloat(CUSTOM6ROUNDNESS, MAXROUNDNESS, m_ConfigPath, 1.0);
		break;

	case CIRCLE_DETECT_TYPE_CUSTOM7:
		m_MinGray = DataUtility::GetProfileInt(CUSTOM7GRAY, MINGRAY, m_ConfigPath, 0);
		m_MaxGray = DataUtility::GetProfileInt(CUSTOM7GRAY, MAXGRAY, m_ConfigPath, 100);
		m_MinCirleArea = DataUtility::GetProfileFloat(CUSTOM7AREA, MINAREA, m_ConfigPath, 150000.0);
		m_MaxCirleArea = DataUtility::GetProfileFloat(CUSTOM7AREA, MAXAREA, m_ConfigPath, 300000.0);
		m_MinRoundness = DataUtility::GetProfileFloat(CUSTOM7ROUNDNESS, MINROUNDNESS, m_ConfigPath, 0.5);
		m_MaxRoundness = DataUtility::GetProfileFloat(CUSTOM7ROUNDNESS, MAXROUNDNESS, m_ConfigPath, 1.0);
		break;

	case CIRCLE_DETECT_TYPE_CUSTOM8:
		m_MinGray = DataUtility::GetProfileInt(CUSTOM8GRAY, MINGRAY, m_ConfigPath, 0);
		m_MaxGray = DataUtility::GetProfileInt(CUSTOM8GRAY, MAXGRAY, m_ConfigPath, 100);
		m_MinCirleArea = DataUtility::GetProfileFloat(CUSTOM8AREA, MINAREA, m_ConfigPath, 150000.0);
		m_MaxCirleArea = DataUtility::GetProfileFloat(CUSTOM8AREA, MAXAREA, m_ConfigPath, 300000.0);
		m_MinRoundness = DataUtility::GetProfileFloat(CUSTOM8ROUNDNESS, MINROUNDNESS, m_ConfigPath, 0.5);
		m_MaxRoundness = DataUtility::GetProfileFloat(CUSTOM8ROUNDNESS, MAXROUNDNESS, m_ConfigPath, 1.0);
		break;

	case CIRCLE_DETECT_TYPE_CUSTOM9:
		m_MinGray = DataUtility::GetProfileInt(CUSTOM9GRAY, MINGRAY, m_ConfigPath, 0);
		m_MaxGray = DataUtility::GetProfileInt(CUSTOM9GRAY, MAXGRAY, m_ConfigPath, 100);
		m_MinCirleArea = DataUtility::GetProfileFloat(CUSTOM9AREA, MINAREA, m_ConfigPath, 150000.0);
		m_MaxCirleArea = DataUtility::GetProfileFloat(CUSTOM9AREA, MAXAREA, m_ConfigPath, 300000.0);
		m_MinRoundness = DataUtility::GetProfileFloat(CUSTOM9ROUNDNESS, MINROUNDNESS, m_ConfigPath, 0.5);
		m_MaxRoundness = DataUtility::GetProfileFloat(CUSTOM9ROUNDNESS, MAXROUNDNESS, m_ConfigPath, 1.0);
		break;

	case CIRCLE_DETECT_TYPE_CUSTOM10:
		m_MinGray = DataUtility::GetProfileInt(CUSTOM10GRAY, MINGRAY, m_ConfigPath, 0);
		m_MaxGray = DataUtility::GetProfileInt(CUSTOM10GRAY, MAXGRAY, m_ConfigPath, 100);
		m_MinCirleArea = DataUtility::GetProfileFloat(CUSTOM10AREA, MINAREA, m_ConfigPath, 150000.0);
		m_MaxCirleArea = DataUtility::GetProfileFloat(CUSTOM10AREA, MAXAREA, m_ConfigPath, 300000.0);
		m_MinRoundness = DataUtility::GetProfileFloat(CUSTOM10ROUNDNESS, MINROUNDNESS, m_ConfigPath, 0.5);
		m_MaxRoundness = DataUtility::GetProfileFloat(CUSTOM10ROUNDNESS, MAXROUNDNESS, m_ConfigPath, 1.0);
		break;

	case CIRCLE_DETECT_TYPE_CUSTOM11:
		m_MinGray = DataUtility::GetProfileInt(CUSTOM11GRAY, MINGRAY, m_ConfigPath, 0);
		m_MaxGray = DataUtility::GetProfileInt(CUSTOM11GRAY, MAXGRAY, m_ConfigPath, 100);
		m_MinCirleArea = DataUtility::GetProfileFloat(CUSTOM11AREA, MINAREA, m_ConfigPath, 150000.0);
		m_MaxCirleArea = DataUtility::GetProfileFloat(CUSTOM11AREA, MAXAREA, m_ConfigPath, 300000.0);
		m_MinRoundness = DataUtility::GetProfileFloat(CUSTOM11ROUNDNESS, MINROUNDNESS, m_ConfigPath, 0.5);
		m_MaxRoundness = DataUtility::GetProfileFloat(CUSTOM11ROUNDNESS, MAXROUNDNESS, m_ConfigPath, 1.0);
		break;

	case CIRCLE_DETECT_TYPE_CUSTOM12:
		m_MinGray = DataUtility::GetProfileInt(CUSTOM12GRAY, MINGRAY, m_ConfigPath, 0);
		m_MaxGray = DataUtility::GetProfileInt(CUSTOM12GRAY, MAXGRAY, m_ConfigPath, 100);
		m_MinCirleArea = DataUtility::GetProfileFloat(CUSTOM12AREA, MINAREA, m_ConfigPath, 150000.0);
		m_MaxCirleArea = DataUtility::GetProfileFloat(CUSTOM12AREA, MAXAREA, m_ConfigPath, 300000.0);
		m_MinRoundness = DataUtility::GetProfileFloat(CUSTOM12ROUNDNESS, MINROUNDNESS, m_ConfigPath, 0.5);
		m_MaxRoundness = DataUtility::GetProfileFloat(CUSTOM12ROUNDNESS, MAXROUNDNESS, m_ConfigPath, 1.0);
		break;

	case CIRCLE_DETECT_TYPE_CUSTOM13:
		m_MinGray = DataUtility::GetProfileInt(CUSTOM13GRAY, MINGRAY, m_ConfigPath, 0);
		m_MaxGray = DataUtility::GetProfileInt(CUSTOM13GRAY, MAXGRAY, m_ConfigPath, 100);
		m_MinCirleArea = DataUtility::GetProfileFloat(CUSTOM13AREA, MINAREA, m_ConfigPath, 150000.0);
		m_MaxCirleArea = DataUtility::GetProfileFloat(CUSTOM13AREA, MAXAREA, m_ConfigPath, 300000.0);
		m_MinRoundness = DataUtility::GetProfileFloat(CUSTOM13ROUNDNESS, MINROUNDNESS, m_ConfigPath, 0.5);
		m_MaxRoundness = DataUtility::GetProfileFloat(CUSTOM13ROUNDNESS, MAXROUNDNESS, m_ConfigPath, 1.0);
		break;

	case CIRCLE_DETECT_TYPE_CUSTOM14:
		m_MinGray = DataUtility::GetProfileInt(CUSTOM14GRAY, MINGRAY, m_ConfigPath, 0);
		m_MaxGray = DataUtility::GetProfileInt(CUSTOM14GRAY, MAXGRAY, m_ConfigPath, 100);
		m_MinCirleArea = DataUtility::GetProfileFloat(CUSTOM14AREA, MINAREA, m_ConfigPath, 150000.0);
		m_MaxCirleArea = DataUtility::GetProfileFloat(CUSTOM14AREA, MAXAREA, m_ConfigPath, 300000.0);
		m_MinRoundness = DataUtility::GetProfileFloat(CUSTOM14ROUNDNESS, MINROUNDNESS, m_ConfigPath, 0.5);
		m_MaxRoundness = DataUtility::GetProfileFloat(CUSTOM14ROUNDNESS, MAXROUNDNESS, m_ConfigPath, 1.0);
		break;

	case CIRCLE_DETECT_TYPE_CUSTOM15:
		m_MinGray = DataUtility::GetProfileInt(CUSTOM15GRAY, MINGRAY, m_ConfigPath, 0);
		m_MaxGray = DataUtility::GetProfileInt(CUSTOM15GRAY, MAXGRAY, m_ConfigPath, 100);
		m_MinCirleArea = DataUtility::GetProfileFloat(CUSTOM15AREA, MINAREA, m_ConfigPath, 150000.0);
		m_MaxCirleArea = DataUtility::GetProfileFloat(CUSTOM15AREA, MAXAREA, m_ConfigPath, 300000.0);
		m_MinRoundness = DataUtility::GetProfileFloat(CUSTOM15ROUNDNESS, MINROUNDNESS, m_ConfigPath, 0.5);
		m_MaxRoundness = DataUtility::GetProfileFloat(CUSTOM15ROUNDNESS, MAXROUNDNESS, m_ConfigPath, 1.0);
		break;

	default:
		m_MinGray = DataUtility::GetProfileInt(STANDARDCIRLEGRAY, MINGRAY, m_ConfigPath, 0);
		m_MaxGray = DataUtility::GetProfileInt(STANDARDCIRLEGRAY, MAXGRAY, m_ConfigPath, 100);
		m_MinCirleArea = DataUtility::GetProfileFloat(STANDARDCIRLEAREA, MINAREA, m_ConfigPath, 150000.0);
		m_MaxCirleArea = DataUtility::GetProfileFloat(STANDARDCIRLEAREA, MAXAREA, m_ConfigPath, 300000.0);
		m_MinRoundness = DataUtility::GetProfileFloat(STANDARDCIRLEROUNDNESS, MINROUNDNESS, m_ConfigPath, 0.5);
		m_MaxRoundness = DataUtility::GetProfileFloat(STANDARDCIRLEROUNDNESS, MAXROUNDNESS, m_ConfigPath, 1.0);
		break;
	}
}

void CDetectCircularhole::SaveConfig()
{
	switch(m_detectType)
	{
	case CIRCLE_DETECT_TYPE_NORMAL:
		DataUtility::SetProfileInt(STANDARDCIRLEGRAY, MINGRAY, m_ConfigPath, m_MinGray);
		DataUtility::SetProfileInt(STANDARDCIRLEGRAY, MAXGRAY, m_ConfigPath, m_MaxGray);
		DataUtility::SetProfileFloat(STANDARDCIRLEAREA, MINAREA, m_ConfigPath, m_MinCirleArea);
		DataUtility::SetProfileFloat(STANDARDCIRLEAREA, MAXAREA, m_ConfigPath, m_MaxCirleArea);
		DataUtility::SetProfileFloat(STANDARDCIRLEROUNDNESS, MINROUNDNESS, m_ConfigPath, m_MinRoundness);
		DataUtility::SetProfileFloat(STANDARDCIRLEROUNDNESS, MAXROUNDNESS, m_ConfigPath, m_MaxRoundness);
		break;

	case CIRCLE_DETECT_TYPE_OBLONG:
		DataUtility::SetProfileInt(OBLONGCIRLEGRAY, MINGRAY, m_ConfigPath, m_MinGray);
		DataUtility::SetProfileInt(OBLONGCIRLEGRAY, MAXGRAY, m_ConfigPath, m_MaxGray);
		DataUtility::SetProfileFloat(OBLONGCIRLEAREA, MINAREA, m_ConfigPath, m_MinCirleArea);
		DataUtility::SetProfileFloat(OBLONGCIRLEAREA, MAXAREA, m_ConfigPath, m_MaxCirleArea);
		DataUtility::SetProfileFloat(OBLONGCIRLEROUNDNESS, MINROUNDNESS, m_ConfigPath, m_MinRoundness);
		DataUtility::SetProfileFloat(OBLONGCIRLEROUNDNESS, MAXROUNDNESS, m_ConfigPath, m_MaxRoundness);
		break;

	case CIRCLE_DETECT_TYPE_FIXTURE:
		DataUtility::SetProfileInt(FIXTURECIRLEGRAY, MINGRAY, m_ConfigPath, m_MinGray);
		DataUtility::SetProfileInt(FIXTURECIRLEGRAY, MAXGRAY, m_ConfigPath, m_MaxGray);
		DataUtility::SetProfileFloat(FIXTURECIRLEAREA, MINAREA, m_ConfigPath, m_MinCirleArea);
		DataUtility::SetProfileFloat(FIXTURECIRLEAREA, MAXAREA, m_ConfigPath, m_MaxCirleArea);
		DataUtility::SetProfileFloat(FIXTURECIRLEROUNDNESS, MINROUNDNESS, m_ConfigPath, m_MinRoundness);
		DataUtility::SetProfileFloat(FIXTURECIRLEROUNDNESS, MAXROUNDNESS, m_ConfigPath, m_MaxRoundness);
		break;

	case CIRCLE_DETECT_TYPE_SPECIAL:
		DataUtility::SetProfileInt(SPECIALCIRLEGRAY, MINGRAY, m_ConfigPath, m_MinGray);
		DataUtility::SetProfileInt(SPECIALCIRLEGRAY, MAXGRAY, m_ConfigPath, m_MaxGray);
		DataUtility::SetProfileFloat(SPECIALCIRLEAREA, MINAREA, m_ConfigPath, m_MinCirleArea);
		DataUtility::SetProfileFloat(SPECIALCIRLEAREA, MAXAREA, m_ConfigPath, m_MaxCirleArea);
		DataUtility::SetProfileFloat(SPECIALCIRLEROUNDNESS, MINROUNDNESS, m_ConfigPath, m_MinRoundness);
		DataUtility::SetProfileFloat(SPECIALCIRLEROUNDNESS, MAXROUNDNESS, m_ConfigPath, m_MaxRoundness);
		break;

	case CIRCLE_DETECT_TYPE_CUSTOM1:
		DataUtility::SetProfileInt(CUSTOM1GRAY, MINGRAY, m_ConfigPath, m_MinGray);
		DataUtility::SetProfileInt(CUSTOM1GRAY, MAXGRAY, m_ConfigPath, m_MaxGray);
		DataUtility::SetProfileFloat(CUSTOM1AREA, MINAREA, m_ConfigPath, m_MinCirleArea);
		DataUtility::SetProfileFloat(CUSTOM1AREA, MAXAREA, m_ConfigPath, m_MaxCirleArea);
		DataUtility::SetProfileFloat(CUSTOM1ROUNDNESS, MINROUNDNESS, m_ConfigPath, m_MinRoundness);
		DataUtility::SetProfileFloat(CUSTOM1ROUNDNESS, MAXROUNDNESS, m_ConfigPath, m_MaxRoundness);
		break;

	case CIRCLE_DETECT_TYPE_CUSTOM2:
		DataUtility::SetProfileInt(CUSTOM2GRAY, MINGRAY, m_ConfigPath, m_MinGray);
		DataUtility::SetProfileInt(CUSTOM2GRAY, MAXGRAY, m_ConfigPath, m_MaxGray);
		DataUtility::SetProfileFloat(CUSTOM2AREA, MINAREA, m_ConfigPath, m_MinCirleArea);
		DataUtility::SetProfileFloat(CUSTOM2AREA, MAXAREA, m_ConfigPath, m_MaxCirleArea);
		DataUtility::SetProfileFloat(CUSTOM2ROUNDNESS, MINROUNDNESS, m_ConfigPath, m_MinRoundness);
		DataUtility::SetProfileFloat(CUSTOM2ROUNDNESS, MAXROUNDNESS, m_ConfigPath, m_MaxRoundness);
		break;

	case CIRCLE_DETECT_TYPE_CUSTOM3:
		DataUtility::SetProfileInt(CUSTOM3GRAY, MINGRAY, m_ConfigPath, m_MinGray);
		DataUtility::SetProfileInt(CUSTOM3GRAY, MAXGRAY, m_ConfigPath, m_MaxGray);
		DataUtility::SetProfileFloat(CUSTOM3AREA, MINAREA, m_ConfigPath, m_MinCirleArea);
		DataUtility::SetProfileFloat(CUSTOM3AREA, MAXAREA, m_ConfigPath, m_MaxCirleArea);
		DataUtility::SetProfileFloat(CUSTOM3ROUNDNESS, MINROUNDNESS, m_ConfigPath, m_MinRoundness);
		DataUtility::SetProfileFloat(CUSTOM3ROUNDNESS, MAXROUNDNESS, m_ConfigPath, m_MaxRoundness);
		break;

	case CIRCLE_DETECT_TYPE_CUSTOM4:
		DataUtility::SetProfileInt(CUSTOM4GRAY, MINGRAY, m_ConfigPath, m_MinGray);
		DataUtility::SetProfileInt(CUSTOM4GRAY, MAXGRAY, m_ConfigPath, m_MaxGray);
		DataUtility::SetProfileFloat(CUSTOM4AREA, MINAREA, m_ConfigPath, m_MinCirleArea);
		DataUtility::SetProfileFloat(CUSTOM4AREA, MAXAREA, m_ConfigPath, m_MaxCirleArea);
		DataUtility::SetProfileFloat(CUSTOM4ROUNDNESS, MINROUNDNESS, m_ConfigPath, m_MinRoundness);
		DataUtility::SetProfileFloat(CUSTOM4ROUNDNESS, MAXROUNDNESS, m_ConfigPath, m_MaxRoundness);
		break;

	case CIRCLE_DETECT_TYPE_CUSTOM5:
		DataUtility::SetProfileInt(CUSTOM5GRAY, MINGRAY, m_ConfigPath, m_MinGray);
		DataUtility::SetProfileInt(CUSTOM5GRAY, MAXGRAY, m_ConfigPath, m_MaxGray);
		DataUtility::SetProfileFloat(CUSTOM5AREA, MINAREA, m_ConfigPath, m_MinCirleArea);
		DataUtility::SetProfileFloat(CUSTOM5AREA, MAXAREA, m_ConfigPath, m_MaxCirleArea);
		DataUtility::SetProfileFloat(CUSTOM5ROUNDNESS, MINROUNDNESS, m_ConfigPath, m_MinRoundness);
		DataUtility::SetProfileFloat(CUSTOM5ROUNDNESS, MAXROUNDNESS, m_ConfigPath, m_MaxRoundness);
		break;

	case CIRCLE_DETECT_TYPE_CUSTOM6:
		DataUtility::SetProfileInt(CUSTOM6GRAY, MINGRAY, m_ConfigPath, m_MinGray);
		DataUtility::SetProfileInt(CUSTOM6GRAY, MAXGRAY, m_ConfigPath, m_MaxGray);
		DataUtility::SetProfileFloat(CUSTOM6AREA, MINAREA, m_ConfigPath, m_MinCirleArea);
		DataUtility::SetProfileFloat(CUSTOM6AREA, MAXAREA, m_ConfigPath, m_MaxCirleArea);
		DataUtility::SetProfileFloat(CUSTOM6ROUNDNESS, MINROUNDNESS, m_ConfigPath, m_MinRoundness);
		DataUtility::SetProfileFloat(CUSTOM6ROUNDNESS, MAXROUNDNESS, m_ConfigPath, m_MaxRoundness);
		break;

	case CIRCLE_DETECT_TYPE_CUSTOM7:
		DataUtility::SetProfileInt(CUSTOM7GRAY, MINGRAY, m_ConfigPath, m_MinGray);
		DataUtility::SetProfileInt(CUSTOM7GRAY, MAXGRAY, m_ConfigPath, m_MaxGray);
		DataUtility::SetProfileFloat(CUSTOM7AREA, MINAREA, m_ConfigPath, m_MinCirleArea);
		DataUtility::SetProfileFloat(CUSTOM7AREA, MAXAREA, m_ConfigPath, m_MaxCirleArea);
		DataUtility::SetProfileFloat(CUSTOM7ROUNDNESS, MINROUNDNESS, m_ConfigPath, m_MinRoundness);
		DataUtility::SetProfileFloat(CUSTOM7ROUNDNESS, MAXROUNDNESS, m_ConfigPath, m_MaxRoundness);
		break;

	case CIRCLE_DETECT_TYPE_CUSTOM8:
		DataUtility::SetProfileInt(CUSTOM8GRAY, MINGRAY, m_ConfigPath, m_MinGray);
		DataUtility::SetProfileInt(CUSTOM8GRAY, MAXGRAY, m_ConfigPath, m_MaxGray);
		DataUtility::SetProfileFloat(CUSTOM8AREA, MINAREA, m_ConfigPath, m_MinCirleArea);
		DataUtility::SetProfileFloat(CUSTOM8AREA, MAXAREA, m_ConfigPath, m_MaxCirleArea);
		DataUtility::SetProfileFloat(CUSTOM8ROUNDNESS, MINROUNDNESS, m_ConfigPath, m_MinRoundness);
		DataUtility::SetProfileFloat(CUSTOM8ROUNDNESS, MAXROUNDNESS, m_ConfigPath, m_MaxRoundness);
		break;

	case CIRCLE_DETECT_TYPE_CUSTOM9:
		DataUtility::SetProfileInt(CUSTOM9GRAY, MINGRAY, m_ConfigPath, m_MinGray);
		DataUtility::SetProfileInt(CUSTOM9GRAY, MAXGRAY, m_ConfigPath, m_MaxGray);
		DataUtility::SetProfileFloat(CUSTOM9AREA, MINAREA, m_ConfigPath, m_MinCirleArea);
		DataUtility::SetProfileFloat(CUSTOM9AREA, MAXAREA, m_ConfigPath, m_MaxCirleArea);
		DataUtility::SetProfileFloat(CUSTOM9ROUNDNESS, MINROUNDNESS, m_ConfigPath, m_MinRoundness);
		DataUtility::SetProfileFloat(CUSTOM9ROUNDNESS, MAXROUNDNESS, m_ConfigPath, m_MaxRoundness);
		break;

	case CIRCLE_DETECT_TYPE_CUSTOM10:
		DataUtility::SetProfileInt(CUSTOM10GRAY, MINGRAY, m_ConfigPath, m_MinGray);
		DataUtility::SetProfileInt(CUSTOM10GRAY, MAXGRAY, m_ConfigPath, m_MaxGray);
		DataUtility::SetProfileFloat(CUSTOM10AREA, MINAREA, m_ConfigPath, m_MinCirleArea);
		DataUtility::SetProfileFloat(CUSTOM10AREA, MAXAREA, m_ConfigPath, m_MaxCirleArea);
		DataUtility::SetProfileFloat(CUSTOM10ROUNDNESS, MINROUNDNESS, m_ConfigPath, m_MinRoundness);
		DataUtility::SetProfileFloat(CUSTOM10ROUNDNESS, MAXROUNDNESS, m_ConfigPath, m_MaxRoundness);
		break;

	case CIRCLE_DETECT_TYPE_CUSTOM11:
		DataUtility::SetProfileInt(CUSTOM11GRAY, MINGRAY, m_ConfigPath, m_MinGray);
		DataUtility::SetProfileInt(CUSTOM11GRAY, MAXGRAY, m_ConfigPath, m_MaxGray);
		DataUtility::SetProfileFloat(CUSTOM11AREA, MINAREA, m_ConfigPath, m_MinCirleArea);
		DataUtility::SetProfileFloat(CUSTOM11AREA, MAXAREA, m_ConfigPath, m_MaxCirleArea);
		DataUtility::SetProfileFloat(CUSTOM11ROUNDNESS, MINROUNDNESS, m_ConfigPath, m_MinRoundness);
		DataUtility::SetProfileFloat(CUSTOM11ROUNDNESS, MAXROUNDNESS, m_ConfigPath, m_MaxRoundness);
		break;

	case CIRCLE_DETECT_TYPE_CUSTOM12:
		DataUtility::SetProfileInt(CUSTOM12GRAY, MINGRAY, m_ConfigPath, m_MinGray);
		DataUtility::SetProfileInt(CUSTOM12GRAY, MAXGRAY, m_ConfigPath, m_MaxGray);
		DataUtility::SetProfileFloat(CUSTOM12AREA, MINAREA, m_ConfigPath, m_MinCirleArea);
		DataUtility::SetProfileFloat(CUSTOM12AREA, MAXAREA, m_ConfigPath, m_MaxCirleArea);
		DataUtility::SetProfileFloat(CUSTOM12ROUNDNESS, MINROUNDNESS, m_ConfigPath, m_MinRoundness);
		DataUtility::SetProfileFloat(CUSTOM12ROUNDNESS, MAXROUNDNESS, m_ConfigPath, m_MaxRoundness);
		break;

	case CIRCLE_DETECT_TYPE_CUSTOM13:
		DataUtility::SetProfileInt(CUSTOM13GRAY, MINGRAY, m_ConfigPath, m_MinGray);
		DataUtility::SetProfileInt(CUSTOM13GRAY, MAXGRAY, m_ConfigPath, m_MaxGray);
		DataUtility::SetProfileFloat(CUSTOM13AREA, MINAREA, m_ConfigPath, m_MinCirleArea);
		DataUtility::SetProfileFloat(CUSTOM13AREA, MAXAREA, m_ConfigPath, m_MaxCirleArea);
		DataUtility::SetProfileFloat(CUSTOM13ROUNDNESS, MINROUNDNESS, m_ConfigPath, m_MinRoundness);
		DataUtility::SetProfileFloat(CUSTOM13ROUNDNESS, MAXROUNDNESS, m_ConfigPath, m_MaxRoundness);
		break;

	case CIRCLE_DETECT_TYPE_CUSTOM14:
		DataUtility::SetProfileInt(CUSTOM14GRAY, MINGRAY, m_ConfigPath, m_MinGray);
		DataUtility::SetProfileInt(CUSTOM14GRAY, MAXGRAY, m_ConfigPath, m_MaxGray);
		DataUtility::SetProfileFloat(CUSTOM14AREA, MINAREA, m_ConfigPath, m_MinCirleArea);
		DataUtility::SetProfileFloat(CUSTOM14AREA, MAXAREA, m_ConfigPath, m_MaxCirleArea);
		DataUtility::SetProfileFloat(CUSTOM14ROUNDNESS, MINROUNDNESS, m_ConfigPath, m_MinRoundness);
		DataUtility::SetProfileFloat(CUSTOM14ROUNDNESS, MAXROUNDNESS, m_ConfigPath, m_MaxRoundness);
		break;

	case CIRCLE_DETECT_TYPE_CUSTOM15:
		DataUtility::SetProfileInt(CUSTOM15GRAY, MINGRAY, m_ConfigPath, m_MinGray);
		DataUtility::SetProfileInt(CUSTOM15GRAY, MAXGRAY, m_ConfigPath, m_MaxGray);
		DataUtility::SetProfileFloat(CUSTOM15AREA, MINAREA, m_ConfigPath, m_MinCirleArea);
		DataUtility::SetProfileFloat(CUSTOM15AREA, MAXAREA, m_ConfigPath, m_MaxCirleArea);
		DataUtility::SetProfileFloat(CUSTOM15ROUNDNESS, MINROUNDNESS, m_ConfigPath, m_MinRoundness);
		DataUtility::SetProfileFloat(CUSTOM15ROUNDNESS, MAXROUNDNESS, m_ConfigPath, m_MaxRoundness);
		break;

	default:
		DataUtility::SetProfileInt(STANDARDCIRLEGRAY, MINGRAY, m_ConfigPath, m_MinGray);
		DataUtility::SetProfileInt(STANDARDCIRLEGRAY, MAXGRAY, m_ConfigPath, m_MaxGray);
		DataUtility::SetProfileFloat(STANDARDCIRLEAREA, MINAREA, m_ConfigPath, m_MinCirleArea);
		DataUtility::SetProfileFloat(STANDARDCIRLEAREA, MAXAREA, m_ConfigPath, m_MaxCirleArea);
		DataUtility::SetProfileFloat(STANDARDCIRLEROUNDNESS, MINROUNDNESS, m_ConfigPath, m_MinRoundness);
		DataUtility::SetProfileFloat(STANDARDCIRLEROUNDNESS, MAXROUNDNESS, m_ConfigPath, m_MaxRoundness);
		break;
	}
}

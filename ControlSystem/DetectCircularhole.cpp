#include "StdAfx.h"
#include "HalconAction.h"
#include "DetectCircularhole.h"


CDetectCircularhole::CDetectCircularhole(void)
	: m_MinGray(128)
	, m_MaxGray(255)
	, m_MinCirleArea(0)
	, m_MaxCirleArea(99999.0)
	, m_MinRoundness(0.5)
	, m_MaxRoundness(1.0)
	, m_DilationRadius(3.5)
	, m_EdgeFilter(_T("canny"))
	, m_EdgeAlpha(1.0)
	, m_EdgeMinThreld(20)
	, m_EdgeMaxThreld(40)
{
	
	m_ShowErrorMeg = true;
	m_ShowProcessingImage = true;
}


CDetectCircularhole::~CDetectCircularhole(void)
{
}

void CDetectCircularhole::SetImageObject(HObject image)
{
	ho_Capture1 = image;
}

bool CDetectCircularhole::DetectCirleCenter(float &row, float &column)
{
	if(RunFitCirle())
	{
		row = hv_Row;
		column = hv_Column;
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
		Threshold(ho_Capture1, &ho_Region, m_MinGray, m_MaxGray);
		if (m_ShowProcessingImage && HDevWindowStack::IsOpen())
		{
			SetColor(HDevWindowStack::GetActive(),"red");
			DispObj(ho_Region, HDevWindowStack::GetActive());
		}
	}
	catch(...)
	{
		ShowMessage("∂˛÷µªØ ß∞‹£°");
		return false;
	}
	return true;
}

bool CDetectCircularhole::RunSelectCirles()
{
	if(RunThreshold())
	{
		try
		{
			Connection(ho_Region, &ho_ConnectedRegions);
			SelectShape(ho_ConnectedRegions, &ho_SelectedRegions, (HTuple("area").Append("roundness")), "and", (HTuple(m_MinCirleArea).Append(m_MinRoundness)), (HTuple(m_MaxCirleArea).Append(m_MaxRoundness)));
			if (m_ShowProcessingImage && HDevWindowStack::IsOpen())
			{
				SetColor(HDevWindowStack::GetActive(),"green");
				DispObj(ho_SelectedRegions, HDevWindowStack::GetActive());
			}
			return true;
		}
		catch(...)
		{
			ShowMessage("ºÏ≤‚‘≤–Œ«¯”Ú ß∞‹£°");
		}
	}
	return false;
}

bool CDetectCircularhole::RunDilationCircle()
{
	if(RunSelectCirles())
	{
		try
		{
			DilationCircle(ho_SelectedRegions, &ho_RegionDilation, m_DilationRadius);
			FillUp(ho_RegionDilation, &ho_RegionFillUp);
			if (m_ShowProcessingImage && HDevWindowStack::IsOpen())
			{
				SetColor(HDevWindowStack::GetActive(),"blue");
				DispObj(ho_RegionFillUp, HDevWindowStack::GetActive());
			}
			return true;
		}
		catch(...)
		{
			ShowMessage("≈Ú’ÕÕºœÒ ß∞‹£°");
		}
	}
	return false;
}

bool CDetectCircularhole::RunDetectEdges()
{
	if(RunDilationCircle())
	{
		try
		{
			Rgb1ToGray(ho_Capture1, &ho_GrayImage);
			ReduceDomain(ho_GrayImage, ho_RegionFillUp, &ho_ImageReduced);
	
			m_EdgeAlpha = (float)(m_EdgeAlpha < 0.0 ? 1.0 : m_EdgeAlpha);
			EdgesSubPix(ho_ImageReduced, &ho_Edges, (char*)(LPCTSTR(m_EdgeFilter)), m_EdgeAlpha, m_EdgeMinThreld, m_EdgeMaxThreld);

			if (m_ShowProcessingImage && HDevWindowStack::IsOpen())
			{
				SetColor(HDevWindowStack::GetActive(),"red");
				DispObj(ho_Edges, HDevWindowStack::GetActive());
			}

			return true;
		}
		catch(...)
		{
			ShowMessage("±ﬂ—ÿºÏ≤‚ ß∞‹£°");
		}
	}
	return false;
}

bool CDetectCircularhole::RunFitCirle()
{
	if(RunDetectEdges())
	{
		try
		{
			LengthXld(ho_Edges, &hv_Length);
			TupleMax(hv_Length, &hv_Max);
			SelectShapeXld(ho_Edges, &ho_SelectedXLD, "contlength", "and", hv_Max, hv_Max);
			FitCircleContourXld(ho_SelectedXLD, "algebraic", -1, 0, 0, 3, 2, &hv_Row, &hv_Column, 
				&hv_Radius, &hv_StartPhi, &hv_EndPhi, &hv_PointOrder);

			if (m_ShowProcessingImage && HDevWindowStack::IsOpen())
			{
				ClearWindow(HDevWindowStack::GetActive());
				SetColor(HDevWindowStack::GetActive(),"yellow");
				DispObj(ho_Capture1, HDevWindowStack::GetActive());
				DispObj(ho_Edges, HDevWindowStack::GetActive());
				DispCross(HDevWindowStack::GetActive(), hv_Row, hv_Column, 20, 0);
			}

			return true;
		}
		catch(...)
		{
			ShowMessage("—°‘Ò∫Õƒ‚∫œ ß∞‹£°");
		}
	}
	return false;
}
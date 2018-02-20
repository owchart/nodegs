#include "stdafx.h"
#include "CFunctionEx.h"
#include "CFunctionBase.h"
#include "CFunctionHttp.h"

static String FUNCTIONS = L"";

namespace OwLibSV
{
	CFunctionEx::CFunctionEx(CIndicator *indicator, int cid, const String& name, NativeBase *native)
	{
		m_indicator = indicator;
		m_ID = cid;
		m_name = name;
		m_native = native;
	}

	CFunctionEx::~CFunctionEx()
	{
		m_indicator = 0;
		m_native = 0;
	}

	/////////////////////////////////////////////////////////////////////////////////////////

	CIndicator* CFunctionEx::CreateIndicator(const String& script, NativeBase *native)
	{
		CIndicator *indicator = new CIndicator();
		CTable *table = new CTable;
		indicator->SetDataSource(table);
		int index = 1000000;
		vector<String> functions = CStr::Split(FUNCTIONS, L",");
		int functionsSize = (int)functions.size();
		for (int i = 0; i < functionsSize; i++)
		{
			indicator->AddFunction(new CFunctionEx(indicator, index + i, functions[i], native));
		}
		CFunctionBase::AddFunctions(indicator);
		CFunctionHttp::AddFunctions(indicator);
		indicator->SetScript(script);
		table->AddColumn(0);
		table->Set(0, 0, 0);
		indicator->OnCalculate(0);
		functions.clear();
		return indicator;
	}

	double CFunctionEx::OnCalculate(CVariable *var)
	{
		switch (var->m_functionID)
		{
			case 1000000:
				return 0;
			default:
				return 0;
		}
	}
}

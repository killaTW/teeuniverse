/*
 * Copyright (C) 2016 necropotame (necropotame@gmail.com)
 * 
 * This file is part of TeeUniverse.
 * 
 * TeeUniverse is free software: you can redistribute it and/or  modify
 * it under the terms of the GNU Affero General Public License, version 3,
 * as published by the Free Software Foundation.
 *
 * TeeUniverse is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with TeeUniverse.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * THIS FILE HAS BEEN GENERATED BY A SCRIPT.
 * DO NOT EDIT MANUALLY!
 *
 * Please use the script "scripts/assets/assets.py" to regenerate it.
 *
 * Why this file is generated by a script?
 * Because there is more than 10 files that follow the same format.
 * In addition, each time a new member is added, enums, getter, setters,
 * copy/transfert functions and storage structure must be updated.
 * It's too much to avoid mistakes.
 */

#include <generated/assets/zonetype.h>
#include <shared/assets/assetssaveloadcontext.h>
#include <shared/archivefile.h>

CAsset_ZoneType::CIndex::CIndex()
{
	m_Used = true;
	m_Color = 1.0f;
}


void CAsset_ZoneType::CIndex::CTuaType_0_1_0::Read(CAssetsSaveLoadContext* pLoadingContext, const CTuaType_0_1_0& TuaType, CAsset_ZoneType::CIndex& SysType)
{
	SysType.m_Used = pLoadingContext->ArchiveFile()->ReadBool(TuaType.m_Used);
	SysType.m_Description.copy(pLoadingContext->ArchiveFile()->GetString(TuaType.m_Description));
	SysType.m_Color = pLoadingContext->ArchiveFile()->ReadColor(TuaType.m_Color);
}


void CAsset_ZoneType::CTuaType_0_1_0::Read(CAssetsSaveLoadContext* pLoadingContext, const CTuaType_0_1_0& TuaType, CAsset_ZoneType& SysType)
{
	CAsset::CTuaType_0_1_0::Read(pLoadingContext, TuaType, SysType);

	{
		const CAsset_ZoneType::CIndex::CTuaType_0_1_0* pData = (const CAsset_ZoneType::CIndex::CTuaType_0_1_0*) pLoadingContext->ArchiveFile()->GetData(TuaType.m_Index.m_Data);
		uint32 Size = pLoadingContext->ArchiveFile()->ReadUInt32(TuaType.m_Index.m_Size);
		SysType.m_Index.resize(Size);
		for(uint32 i=0; i<Size; i++)
		{
			CAsset_ZoneType::CIndex::CTuaType_0_1_0::Read(pLoadingContext, pData[i], SysType.m_Index[i]);
		}
	}
	
}


void CAsset_ZoneType::CIndex::CTuaType_0_1_0::Write(CAssetsSaveLoadContext* pLoadingContext, const CAsset_ZoneType::CIndex& SysType, CTuaType_0_1_0& TuaType)
{
	TuaType.m_Used = pLoadingContext->ArchiveFile()->WriteBool(SysType.m_Used);
	TuaType.m_Description = pLoadingContext->ArchiveFile()->AddString(SysType.m_Description.buffer());
	TuaType.m_Color = pLoadingContext->ArchiveFile()->WriteColor(SysType.m_Color);
}

void CAsset_ZoneType::CTuaType_0_1_0::Write(CAssetsSaveLoadContext* pLoadingContext, const CAsset_ZoneType& SysType, CTuaType_0_1_0& TuaType)
{
	CAsset::CTuaType_0_1_0::Write(pLoadingContext, SysType, TuaType);

	{
		TuaType.m_Index.m_Size = SysType.m_Index.size();
		CAsset_ZoneType::CIndex::CTuaType_0_1_0* pData = new CAsset_ZoneType::CIndex::CTuaType_0_1_0[SysType.m_Index.size()];
		for(int i=0; i<SysType.m_Index.size(); i++)
		{
			CAsset_ZoneType::CIndex::CTuaType_0_1_0::Write(pLoadingContext, SysType.m_Index[i], pData[i]);
		}
		TuaType.m_Index.m_Data = pLoadingContext->ArchiveFile()->AddData((uint8*) pData, sizeof(CAsset_ZoneType::CIndex::CTuaType_0_1_0)*SysType.m_Index.size());
		delete[] pData;
	}
}

void CAsset_ZoneType::CIndex::CTuaType_0_2_0::Read(CAssetsSaveLoadContext* pLoadingContext, const CTuaType_0_2_0& TuaType, CAsset_ZoneType::CIndex& SysType)
{
	SysType.m_Used = pLoadingContext->ArchiveFile()->ReadBool(TuaType.m_Used);
	SysType.m_Description.copy(pLoadingContext->ArchiveFile()->GetString(TuaType.m_Description));
	SysType.m_Color = pLoadingContext->ArchiveFile()->ReadColor(TuaType.m_Color);
}


void CAsset_ZoneType::CTuaType_0_2_0::Read(CAssetsSaveLoadContext* pLoadingContext, const CTuaType_0_2_0& TuaType, CAsset_ZoneType& SysType)
{
	CAsset::CTuaType_0_2_0::Read(pLoadingContext, TuaType, SysType);

	{
		const CAsset_ZoneType::CIndex::CTuaType_0_2_0* pData = (const CAsset_ZoneType::CIndex::CTuaType_0_2_0*) pLoadingContext->ArchiveFile()->GetData(TuaType.m_Index.m_Data);
		uint32 Size = pLoadingContext->ArchiveFile()->ReadUInt32(TuaType.m_Index.m_Size);
		SysType.m_Index.resize(Size);
		for(uint32 i=0; i<Size; i++)
		{
			CAsset_ZoneType::CIndex::CTuaType_0_2_0::Read(pLoadingContext, pData[i], SysType.m_Index[i]);
		}
	}
	
}


void CAsset_ZoneType::CIndex::CTuaType_0_2_0::Write(CAssetsSaveLoadContext* pLoadingContext, const CAsset_ZoneType::CIndex& SysType, CTuaType_0_2_0& TuaType)
{
	TuaType.m_Used = pLoadingContext->ArchiveFile()->WriteBool(SysType.m_Used);
	TuaType.m_Description = pLoadingContext->ArchiveFile()->AddString(SysType.m_Description.buffer());
	TuaType.m_Color = pLoadingContext->ArchiveFile()->WriteColor(SysType.m_Color);
}

void CAsset_ZoneType::CTuaType_0_2_0::Write(CAssetsSaveLoadContext* pLoadingContext, const CAsset_ZoneType& SysType, CTuaType_0_2_0& TuaType)
{
	CAsset::CTuaType_0_2_0::Write(pLoadingContext, SysType, TuaType);

	{
		TuaType.m_Index.m_Size = SysType.m_Index.size();
		CAsset_ZoneType::CIndex::CTuaType_0_2_0* pData = new CAsset_ZoneType::CIndex::CTuaType_0_2_0[SysType.m_Index.size()];
		for(int i=0; i<SysType.m_Index.size(); i++)
		{
			CAsset_ZoneType::CIndex::CTuaType_0_2_0::Write(pLoadingContext, SysType.m_Index[i], pData[i]);
		}
		TuaType.m_Index.m_Data = pLoadingContext->ArchiveFile()->AddData((uint8*) pData, sizeof(CAsset_ZoneType::CIndex::CTuaType_0_2_0)*SysType.m_Index.size());
		delete[] pData;
	}
}

template<>
int CAsset_ZoneType::GetValue(int ValueType, const CSubPath& SubPath, int DefaultValue) const
{
	switch(ValueType)
	{
		case INDEX_ARRAYSIZE:
			return GetIndexArraySize();
	}
	return CAsset::GetValue<int>(ValueType, SubPath, DefaultValue);
}

template<>
bool CAsset_ZoneType::SetValue(int ValueType, const CSubPath& SubPath, int Value)
{
	switch(ValueType)
	{
		case INDEX_ARRAYSIZE:
			SetIndexArraySize(Value);
			return true;
	}
	return CAsset::SetValue<int>(ValueType, SubPath, Value);
}

template<>
bool CAsset_ZoneType::GetValue(int ValueType, const CSubPath& SubPath, bool DefaultValue) const
{
	switch(ValueType)
	{
		case INDEX_USED:
			return GetIndexUsed(SubPath);
	}
	return CAsset::GetValue<bool>(ValueType, SubPath, DefaultValue);
}

template<>
bool CAsset_ZoneType::SetValue(int ValueType, const CSubPath& SubPath, bool Value)
{
	switch(ValueType)
	{
		case INDEX_USED:
			SetIndexUsed(SubPath, Value);
			return true;
	}
	return CAsset::SetValue<bool>(ValueType, SubPath, Value);
}

template<>
const char* CAsset_ZoneType::GetValue(int ValueType, const CSubPath& SubPath, const char* DefaultValue) const
{
	switch(ValueType)
	{
		case INDEX_DESCRIPTION:
			return GetIndexDescription(SubPath);
	}
	return CAsset::GetValue<const char*>(ValueType, SubPath, DefaultValue);
}

template<>
bool CAsset_ZoneType::SetValue(int ValueType, const CSubPath& SubPath, const char* Value)
{
	switch(ValueType)
	{
		case INDEX_DESCRIPTION:
			SetIndexDescription(SubPath, Value);
			return true;
	}
	return CAsset::SetValue<const char*>(ValueType, SubPath, Value);
}

template<>
vec4 CAsset_ZoneType::GetValue(int ValueType, const CSubPath& SubPath, vec4 DefaultValue) const
{
	switch(ValueType)
	{
		case INDEX_COLOR:
			return GetIndexColor(SubPath);
	}
	return CAsset::GetValue<vec4>(ValueType, SubPath, DefaultValue);
}

template<>
bool CAsset_ZoneType::SetValue(int ValueType, const CSubPath& SubPath, vec4 Value)
{
	switch(ValueType)
	{
		case INDEX_COLOR:
			SetIndexColor(SubPath, Value);
			return true;
	}
	return CAsset::SetValue<vec4>(ValueType, SubPath, Value);
}

int CAsset_ZoneType::AddSubItem(int Type, const CSubPath& SubPath)
{
	switch(Type)
	{
		case TYPE_INDEX:
			return AddIndex();
	}
	return -1;
}

int CAsset_ZoneType::AddSubItemAt(int Type, const CSubPath& SubPath, int Index)
{
	switch(Type)
	{
		case TYPE_INDEX:
			AddAtIndex(Index);
			return Index;
	}
	return -1;
}

void CAsset_ZoneType::DeleteSubItem(const CSubPath& SubPath)
{
	switch(SubPath.GetType())
	{
		case TYPE_INDEX:
			DeleteIndex(SubPath);
			break;
	}
}

void CAsset_ZoneType::RelMoveSubItem(const CSubPath& SubPath, int RelMove)
{
	switch(SubPath.GetType())
	{
		case TYPE_INDEX:
			RelMoveIndex(SubPath, RelMove);
			break;
	}
}



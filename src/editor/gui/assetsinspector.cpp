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

#include <editor/gui/assetsinspector.h>
#include <editor/components/gui.h>
#include <shared/components/assetsmanager.h>
#include <client/gui/text-edit.h>
#include <client/gui/integer-edit.h>
#include <client/gui/float-edit.h>
#include <client/gui/color-edit.h>
#include <client/gui/toggle.h>
#include <client/gui/combobox.h>
#include <client/gui/popup.h>
#include <client/gui/expand.h>
#include <shared/components/localization.h>

/* COMBOBOX ***********************************************************/
	
class CMemberComboBox : public gui::CComboBox
{
protected:
	CGuiEditor* m_pAssetsEditor;
	int m_Member;
	
	virtual int GetValue() const
	{
		return m_pAssetsEditor->AssetsManager()->GetAssetValue<int>(
			m_pAssetsEditor->GetEditedAssetPath(),
			m_pAssetsEditor->GetEditedSubPath(),
			m_Member,
			0
		);
	}
	
	virtual void SetValue(int Value)
	{
		m_pAssetsEditor->AssetsManager()->SetAssetValue<int>(
			m_pAssetsEditor->GetEditedAssetPath(),
			m_pAssetsEditor->GetEditedSubPath(),
			m_Member,
			Value
		);
	}
	
public:
	CMemberComboBox(CGuiEditor* pAssetsEditor, int Member) :
		gui::CComboBox(pAssetsEditor),
		m_pAssetsEditor(pAssetsEditor),
		m_Member(Member)
	{
		
	}
	
	virtual void Update(bool ParentEnabled)
	{
		if(IsEnabled() && ParentEnabled)
		{
			if(!AssetsManager()->IsValidPackage(m_pAssetsEditor->GetEditedPackageId()) || AssetsManager()->IsReadOnlyPackage(m_pAssetsEditor->GetEditedPackageId()))
				Editable(false);
			else
				Editable(true);
		}
		
		gui::CComboBox::Update(ParentEnabled);
	}
};

class CConditionalEditor : public gui::CVListLayout
{
protected:
	CGuiEditor* m_pAssetsEditor;
	int m_Member;
	
public:
	CConditionalEditor(CGuiEditor* pAssetsEditor, int Member) :
		gui::CVListLayout(pAssetsEditor),
		m_pAssetsEditor(pAssetsEditor),
		m_Member(Member)
	{
		
	}
	
	virtual void Update(bool ParentEnabled)
	{
		if(ParentEnabled)
		{
			bool Condition = AssetsManager()->GetAssetValue<bool>(m_pAssetsEditor->GetEditedAssetPath(), m_pAssetsEditor->GetEditedSubPath(), m_Member, false);
			if(Condition)
				Enable();
			else
				Disable();
		}
		
		gui::CVListLayout::Update(ParentEnabled);
	}
};

class CSubItemEditor : public gui::CVListLayout
{
protected:
	CGuiEditor* m_pAssetsEditor;
	int m_SubPathType;
	
public:
	CSubItemEditor(CGuiEditor* pAssetsEditor, int SubPathType) :
		gui::CVListLayout(pAssetsEditor),
		m_pAssetsEditor(pAssetsEditor),
		m_SubPathType(SubPathType)
	{
		
	}
	
	virtual void Update(bool ParentEnabled)
	{
		if(ParentEnabled)
		{
			if(m_pAssetsEditor->GetEditedSubPath().GetType() == m_SubPathType)
				Enable();
			else
				Disable();
		}
		
		gui::CVListLayout::Update(ParentEnabled);
	}
};

class CSubItem : public gui::CButton
{
protected:
	class CDeleteButton : public gui::CButton
	{
	protected:
		CGuiEditor* m_pAssetsEditor;
		CContextMenu* m_pContextMenu;
		
	protected:
		virtual void MouseClickAction()
		{
			AssetsManager()->DeleteSubItem(m_pAssetsEditor->GetEditedAssetPath(), m_pAssetsEditor->GetEditedSubPath());
			m_pAssetsEditor->SetEditedAsset(m_pAssetsEditor->GetEditedAssetPath(), CSubPath::Null());
			
			CAssetState* pState = AssetsManager()->GetAssetState(m_pAssetsEditor->GetEditedAssetPath());
			pState->m_NumUpdates++;
			
			m_pContextMenu->Close();
		}
		
	public:
		CDeleteButton(CGuiEditor* pAssetsEditor, CContextMenu* pContextMenu) :
			gui::CButton(pAssetsEditor, _LSTRING("Delete"), pAssetsEditor->m_Path_Sprite_IconDelete),
			m_pAssetsEditor(pAssetsEditor),
			m_pContextMenu(pContextMenu)
		{
			SetButtonStyle(m_pAssetsEditor->m_Path_Button_Menu);
		}
	};
	
	class CRelMoveButton : public gui::CButton
	{
	protected:
		CGuiEditor* m_pAssetsEditor;
		CContextMenu* m_pContextMenu;
		int m_ArraySizeMember;
		int m_Shift;
		
	protected:
		virtual void MouseClickAction()
		{
			AssetsManager()->RelMoveSubItem(m_pAssetsEditor->GetEditedAssetPath(), m_pAssetsEditor->GetEditedSubPath(), m_Shift);
			
			CAssetState* pState = AssetsManager()->GetAssetState(m_pAssetsEditor->GetEditedAssetPath());
			pState->m_NumUpdates++;
			
			m_pContextMenu->Close();
		}
		
	public:
		CRelMoveButton(CGuiEditor* pAssetsEditor, CContextMenu* pContextMenu, const CLocalizableString& LString, CAssetPath Iconpath, int Shift) :
			gui::CButton(pAssetsEditor, LString, Iconpath),
			m_pAssetsEditor(pAssetsEditor),
			m_pContextMenu(pContextMenu),
			m_Shift(Shift)
		{
			SetButtonStyle(m_pAssetsEditor->m_Path_Button_Menu);
		}
	};
	
protected:
	CGuiEditor* m_pAssetsEditor;
	CSubPath m_SubPath;
	
protected:
	virtual void MouseClickAction() { Action(); }
	
	void Action()
	{
		m_pAssetsEditor->SetEditedAsset(m_pAssetsEditor->GetEditedAssetPath(), m_SubPath);
	}

public:
	CSubItem(CGuiEditor* pAssetsEditor, CSubPath SubPath, const char* pName, CAssetPath IconPath) :
		gui::CButton(pAssetsEditor, pName, IconPath),
		m_pAssetsEditor(pAssetsEditor),
		m_SubPath(SubPath)
	{
		SetButtonStyle(m_pAssetsEditor->m_Path_Button_ListItem);
	}
	
	CSubItem(CGuiEditor* pAssetsEditor, CSubPath SubPath, const CLocalizableString& Text, CAssetPath IconPath) :
		gui::CButton(pAssetsEditor, Text, IconPath),
		m_pAssetsEditor(pAssetsEditor),
		m_SubPath(SubPath)
	{
		SetButtonStyle(m_pAssetsEditor->m_Path_Button_ListItem);
	}
	
	virtual void Update(bool ParentEnabled)
	{
		if(m_pAssetsEditor->GetEditedSubPath() == m_SubPath)
			SetButtonStyle(m_pAssetsEditor->m_Path_Button_ListItemHL);
		else
			SetButtonStyle(m_pAssetsEditor->m_Path_Button_ListItem);
		
		gui::CButton::Update(ParentEnabled);
	}
	
	virtual void OnButtonClick(int Button)
	{
		if(Button == KEY_MOUSE_2 && m_DrawRect.IsInside(Context()->GetMousePos()))
		{
			CContextMenu* pMenu = new CContextMenu(m_pAssetsEditor);
			
			pMenu->List()->Add(new CRelMoveButton(m_pAssetsEditor, pMenu, _LSTRING("Move to the back"), m_pAssetsEditor->m_Path_Sprite_IconMoveBack, -99999999), false);
			pMenu->List()->Add(new CRelMoveButton(m_pAssetsEditor, pMenu, _LSTRING("Move backard"), m_pAssetsEditor->m_Path_Sprite_IconUp, -1), false);
			pMenu->List()->Add(new CRelMoveButton(m_pAssetsEditor, pMenu, _LSTRING("Move forward"), m_pAssetsEditor->m_Path_Sprite_IconDown, 1), false);
			pMenu->List()->Add(new CRelMoveButton(m_pAssetsEditor, pMenu, _LSTRING("Move to the front"), m_pAssetsEditor->m_Path_Sprite_IconMoveFront, 99999999), false);
			pMenu->List()->AddSeparator();
			pMenu->List()->Add(new CDeleteButton(m_pAssetsEditor, pMenu));
			
			m_pAssetsEditor->DisplayPopup(pMenu);
			Action();
			return;
		}
		
		gui::CButton::OnButtonClick(Button);
	}
};

class CSubItemList : public gui::CVScrollLayout
{
protected:
	CGuiEditor* m_pAssetsEditor;
	CAssetPath m_AssetPath;
	int m_AssetType;
	int m_AssetVersion;
	bool m_UpdateNeeded;
	CAssetPath m_IconPath;
	
protected:
	virtual void GenerateList() = 0;
	
public:
	CSubItemList(CGuiEditor* pAssetsEditor, int AssetType) :
		gui::CVScrollLayout(pAssetsEditor),
		m_pAssetsEditor(pAssetsEditor),
		m_AssetType(AssetType),
		m_AssetVersion(-1),
		m_UpdateNeeded(true)
	{
		SetBoxStyle(m_pAssetsEditor->m_Path_Box_SubList);
	}
	
	virtual void Update(bool ParentEnabled)
	{
		if(ParentEnabled && IsEnabled())
		{
			if(m_pAssetsEditor->GetEditedAssetPath().GetType() == m_AssetType)
			{
				CAssetState* pState = AssetsManager()->GetAssetState(m_pAssetsEditor->GetEditedAssetPath());
				
				if(
					m_AssetPath != m_pAssetsEditor->GetEditedAssetPath() ||
					(pState && pState->m_NumUpdates != m_AssetVersion)
				)
				{
					m_AssetPath = m_pAssetsEditor->GetEditedAssetPath();
					m_AssetVersion = pState->m_NumUpdates;
					m_UpdateNeeded = true;
				}
			}
			
			if(m_UpdateNeeded)
			{
				GenerateList();				
				m_UpdateNeeded = false;
			}
		}
		
		gui::CVScrollLayout::Update(ParentEnabled);
	}
};

/* ASSETS INSPECTOR ***************************************************/

CAssetsInspector::CAssetsInspector(CGuiEditor* pAssetsEditor) :
	gui::CTabs(pAssetsEditor),
	m_pAssetsEditor(pAssetsEditor)
{
	for(int i=0; i<NUM_TABS; i++)
		m_pTabs[TAB_IMAGE_ASSET] = 0;
	
	//TAG_NEW_ASSET
	m_pTabs[TAB_GENERIC_ASSET] = CreateTab_Generic_Asset();
	m_pTabs[TAB_GUIRECTSTYLE_ASSET] = CreateTab_GuiRectStyle_Asset();
	m_pTabs[TAB_GUILINESTYLE_ASSET] = CreateTab_GuiLineStyle_Asset();
	m_pTabs[TAB_GUIBOXSTYLE_ASSET] = CreateTab_GuiBoxStyle_Asset();
	m_pTabs[TAB_GUILABELSTYLE_ASSET] = CreateTab_GuiLabelStyle_Asset();
	m_pTabs[TAB_GUIBUTTONSTYLE_ASSET] = CreateTab_GuiButtonStyle_Asset();
	m_pTabs[TAB_GUIINTEDITSTYLE_ASSET] = CreateTab_GuiIntEditStyle_Asset();
	m_pTabs[TAB_GUITOGGLESTYLE_ASSET] = CreateTab_GuiToggleStyle_Asset();
	m_pTabs[TAB_GUISLIDERSTYLE_ASSET] = CreateTab_GuiSliderStyle_Asset();
	m_pTabs[TAB_GUISCROLLBARSTYLE_ASSET] = CreateTab_GuiScrollbarStyle_Asset();
	m_pTabs[TAB_GUITABSSTYLE_ASSET] = CreateTab_GuiTabsStyle_Asset();
	m_pTabs[TAB_IMAGE_ASSET] = CreateTab_Image_Asset();
	m_pTabs[TAB_MAP_ASSET] = CreateTab_Map_Asset();
	m_pTabs[TAB_MAPGROUP_ASSET] = CreateTab_MapGroup_Asset();
	m_pTabs[TAB_MAPLAYERTILES_ASSET] = CreateTab_MapLayerTiles_Asset();
	m_pTabs[TAB_MAPLAYERQUADS_ASSET] = CreateTab_MapLayerQuads_Asset();
	m_pTabs[TAB_MAPLAYEROBJECTS_ASSET] = CreateTab_MapLayerObjects_Asset();
	m_pTabs[TAB_MAPZONETILES_ASSET] = CreateTab_MapZoneTiles_Asset();
	m_pTabs[TAB_MAPENTITIES_ASSET] = CreateTab_MapEntities_Asset();
	m_pTabs[TAB_ZONETYPE_ASSET] = CreateTab_ZoneType_Asset();
	m_pTabs[TAB_ENTITYTYPE_ASSET] = CreateTab_EntityType_Asset();
	m_pTabs[TAB_IMAGE_ASSET] = CreateTab_Image_Asset();
	m_pTabs[TAB_SPRITE_ASSET] = CreateTab_Sprite_Asset();
	m_pTabs[TAB_MATERIAL_ASSET] = CreateTab_Material_Asset();
}

void CAssetsInspector::Update(bool ParentEnabled)
{
	for(int i=0; i<NUM_TABS; i++)
		m_pTabs[i]->Disable();
	
	if(AssetsManager()->GetAssetState(AssetsEditor()->GetEditedAssetPath()))
	{
		Enable();
		
		//TAG_NEW_ASSET
		switch(AssetsEditor()->GetEditedAssetPath().GetType())
		{
			case CAsset_GuiRectStyle::TypeId:
				m_pTabs[TAB_GUIRECTSTYLE_ASSET]->Enable();
				break;
			case CAsset_GuiLineStyle::TypeId:
				m_pTabs[TAB_GUILINESTYLE_ASSET]->Enable();
				break;
			case CAsset_GuiBoxStyle::TypeId:
				m_pTabs[TAB_GUIBOXSTYLE_ASSET]->Enable();
				break;
			case CAsset_GuiLabelStyle::TypeId:
				m_pTabs[TAB_GUILABELSTYLE_ASSET]->Enable();
				break;
			case CAsset_GuiButtonStyle::TypeId:
				m_pTabs[TAB_GUIBUTTONSTYLE_ASSET]->Enable();
				break;
			case CAsset_GuiIntEditStyle::TypeId:
				m_pTabs[TAB_GUIINTEDITSTYLE_ASSET]->Enable();
				break;
			case CAsset_GuiToggleStyle::TypeId:
				m_pTabs[TAB_GUITOGGLESTYLE_ASSET]->Enable();
				break;
			case CAsset_GuiSliderStyle::TypeId:
				m_pTabs[TAB_GUISLIDERSTYLE_ASSET]->Enable();
				break;
			case CAsset_GuiScrollbarStyle::TypeId:
				m_pTabs[TAB_GUISCROLLBARSTYLE_ASSET]->Enable();
				break;
			case CAsset_GuiTabsStyle::TypeId:
				m_pTabs[TAB_GUITABSSTYLE_ASSET]->Enable();
				break;
			case CAsset_Image::TypeId:
				m_pTabs[TAB_IMAGE_ASSET]->Enable();
				break;
			case CAsset_Map::TypeId:
				m_pTabs[TAB_MAP_ASSET]->Enable();
				break;
			case CAsset_MapGroup::TypeId:
				m_pTabs[TAB_MAPGROUP_ASSET]->Enable();
				break;
			case CAsset_MapLayerTiles::TypeId:
				m_pTabs[TAB_MAPLAYERTILES_ASSET]->Enable();
				break;
			case CAsset_MapLayerQuads::TypeId:
				m_pTabs[TAB_MAPLAYERQUADS_ASSET]->Enable();
				break;
			case CAsset_MapLayerObjects::TypeId:
				m_pTabs[TAB_MAPLAYEROBJECTS_ASSET]->Enable();
				break;
			case CAsset_MapZoneTiles::TypeId:
				m_pTabs[TAB_MAPZONETILES_ASSET]->Enable();
				break;
			case CAsset_MapEntities::TypeId:
				m_pTabs[TAB_MAPENTITIES_ASSET]->Enable();
				break;
			case CAsset_ZoneType::TypeId:
				m_pTabs[TAB_ZONETYPE_ASSET]->Enable();
				break;
			case CAsset_EntityType::TypeId:
				m_pTabs[TAB_ENTITYTYPE_ASSET]->Enable();
				break;
			case CAsset_Sprite::TypeId:
				m_pTabs[TAB_SPRITE_ASSET]->Enable();
				break;
			case CAsset_Material::TypeId:
				m_pTabs[TAB_MATERIAL_ASSET]->Enable();
				break;
			default:
				m_pTabs[TAB_GENERIC_ASSET]->Enable();
				break;
		}
		
		gui::CTabs::Update(ParentEnabled);
	}
	else
	{
		Disable();
	
		return;
	}
}

gui::CVScrollLayout* CAssetsInspector::CreateTab_Generic_Asset()
{
	gui::CVScrollLayout* pTab = new gui::CVScrollLayout(Context());
	
	pTab->Disable();
	AddTab(pTab, _LSTRING("Asset"), AssetsEditor()->m_Path_Sprite_IconAsset);
	
	AddField_AssetProperties(pTab);
	
	return pTab;
}

/* IMAGE **************************************************************/
	
class CImageTilingToggle : public gui::CAbstractToggle
{
protected:
	CGuiEditor* m_pAssetsEditor;
	
	virtual bool GetValue()
	{
		return m_pAssetsEditor->AssetsManager()->GetAssetValue<bool>(
			m_pAssetsEditor->GetEditedAssetPath(),
			CSubPath::Null(),
			CAsset_Image::TILINGENABLED,
			false
		);
	}
	
	virtual void SetValue(bool Value)
	{
		m_pAssetsEditor->AssetsManager()->SetAssetValue<bool>(
			m_pAssetsEditor->GetEditedAssetPath(),
			CSubPath::Null(),
			CAsset_Image::TILINGENABLED,
			Value
		);
		if(Value)
		{
			m_pAssetsEditor->AssetsManager()->SetAssetValue<int>(
				m_pAssetsEditor->GetEditedAssetPath(),
				CSubPath::Null(),
				CAsset_Image::GRIDWIDTH,
				16
			);
			m_pAssetsEditor->AssetsManager()->SetAssetValue<int>(
				m_pAssetsEditor->GetEditedAssetPath(),
				CSubPath::Null(),
				CAsset_Image::GRIDHEIGHT,
				16
			);
		}
		AssetsManager()->RequestUpdate(m_pAssetsEditor->GetEditedAssetPath());
	}
	
public:
	CImageTilingToggle(CGuiEditor* pAssetsEditor) :
		gui::CAbstractToggle(pAssetsEditor),
		m_pAssetsEditor(pAssetsEditor)
	{ }
};

gui::CVScrollLayout* CAssetsInspector::CreateTab_Image_Asset()
{
	gui::CVScrollLayout* pTab = new gui::CVScrollLayout(Context());
	pTab->Disable();
	AddTab(pTab, _LSTRING("Image"), AssetsEditor()->m_Path_Sprite_IconImage);
	
	AddField_AssetProperties(pTab);
	
	AddField_Integer_NoEdit(pTab, CAsset_Image::DATA_WIDTH, _LSTRING("Width"));
	AddField_Integer_NoEdit(pTab, CAsset_Image::DATA_HEIGHT, _LSTRING("Height"));
	//AddField_Integer_NoEdit(pTab, CAsset_Image::TEXTURE_ID, _LSTRING("Texture Id"));
	AddField_Integer(pTab, CAsset_Image::TEXELSIZE, _LSTRING("Texel size"));
	AddField_Integer(pTab, CAsset_Image::GRIDWIDTH, _LSTRING("Grid width"));
	AddField_Integer(pTab, CAsset_Image::GRIDHEIGHT, _LSTRING("Grid height"));
	AddField_Integer(pTab, CAsset_Image::GRIDSPACING, _LSTRING("Grid spacing"));
	
	CImageTilingToggle* pWidget = new CImageTilingToggle(m_pAssetsEditor);
	AddField(pTab, pWidget, _LSTRING("Compatible with tiles"));
	
	return pTab;
}

/* SPRITE *************************************************************/

gui::CVScrollLayout* CAssetsInspector::CreateTab_Sprite_Asset()
{
	gui::CVScrollLayout* pTab = new gui::CVScrollLayout(Context());
	pTab->Disable();
	AddTab(pTab, _LSTRING("Sprite"), AssetsEditor()->m_Path_Sprite_IconSprite);
	
	AddField_AssetProperties(pTab);
	
	AddField_Asset(pTab, CAsset_Sprite::IMAGEPATH, CAsset_Image::TypeId, _LSTRING("Image"));
	AddField_Integer(pTab, CAsset_Sprite::X, _LSTRING("X"));
	AddField_Integer(pTab, CAsset_Sprite::Y, _LSTRING("Y"));
	AddField_Integer(pTab, CAsset_Sprite::WIDTH, _LSTRING("Width"));
	AddField_Integer(pTab, CAsset_Sprite::HEIGHT, _LSTRING("Height"));
	
	return pTab;
}

gui::CVScrollLayout* CAssetsInspector::CreateTab_Map_Asset()
{
	gui::CVScrollLayout* pTab = new gui::CVScrollLayout(Context());
	pTab->Disable();
	AddTab(pTab, _LSTRING("Map"), AssetsEditor()->m_Path_Sprite_IconMap);
	
	AddField_AssetProperties(pTab);
	
	return pTab;
}

gui::CVScrollLayout* CAssetsInspector::CreateTab_MapGroup_Asset()
{
	gui::CVScrollLayout* pTab = new gui::CVScrollLayout(Context());
	pTab->Disable();
	AddTab(pTab, _LSTRING("Map Group"), AssetsEditor()->m_Path_Sprite_IconFolder);
	
	AddField_AssetProperties(pTab);
	
	AddField_Vec2(pTab, CAsset_MapGroup::POSITION_X, CAsset_MapGroup::POSITION_Y, _LSTRING("Position"));	
	AddField_Vec2(pTab, CAsset_MapGroup::HARDPARALLAX_X, CAsset_MapGroup::HARDPARALLAX_Y, _LSTRING("Parallax"));	
	pTab->AddSeparator();
	AddField_Bool(pTab, CAsset_MapGroup::CLIPPING, _LSTRING("Clipping"));	
	AddField_Vec2(pTab, CAsset_MapGroup::CLIPPOSITION_X, CAsset_MapGroup::CLIPPOSITION_Y, _LSTRING("Clip Position"));	
	AddField_Vec2(pTab, CAsset_MapGroup::CLIPSIZE_X, CAsset_MapGroup::CLIPSIZE_Y, _LSTRING("Clip Size"));
	
	return pTab;
}

class CMapLayerTilesButton_HFlip : public gui::CButton
{
protected:
	CGuiEditor* m_pAssetsEditor;
	
	virtual void MouseClickAction()
	{
		int Token = AssetsManager()->GenerateToken();
		AssetsManager()->MapLayerTiles_HFlip(m_pAssetsEditor->GetEditedAssetPath(), Token);
	}
	
public:
	CMapLayerTilesButton_HFlip(CGuiEditor* pAssetsEditor) :
		gui::CButton(pAssetsEditor, _LSTRING("Vertical Mirror"), pAssetsEditor->m_Path_Sprite_IconHFlip),
		m_pAssetsEditor(pAssetsEditor)
	{ }
};

class CMapLayerTilesButton_VFlip : public gui::CButton
{
protected:
	CGuiEditor* m_pAssetsEditor;
	
	virtual void MouseClickAction()
	{
		int Token = AssetsManager()->GenerateToken();
		AssetsManager()->MapLayerTiles_VFlip(m_pAssetsEditor->GetEditedAssetPath(), Token);
	}
	
public:
	CMapLayerTilesButton_VFlip(CGuiEditor* pAssetsEditor) :
		gui::CButton(pAssetsEditor, _LSTRING("Horizontal Mirror"), pAssetsEditor->m_Path_Sprite_IconVFlip),
		m_pAssetsEditor(pAssetsEditor)
	{ }
};

gui::CVScrollLayout* CAssetsInspector::CreateTab_MapLayerTiles_Asset()
{
	gui::CVScrollLayout* pTab = new gui::CVScrollLayout(Context());
	pTab->Disable();
	AddTab(pTab, _LSTRING("Map Layer Tiles"), AssetsEditor()->m_Path_Sprite_IconTiles);
	
	AddField_AssetProperties(pTab);
	
	AddField_Integer(pTab, CAsset_MapLayerTiles::TILE_WIDTH, _LSTRING("Width"));	
	AddField_Integer(pTab, CAsset_MapLayerTiles::TILE_HEIGHT, _LSTRING("Height"));	
	AddField_ImageTiles(pTab, CAsset_MapLayerTiles::IMAGEPATH, CAsset_Image::TypeId, _LSTRING("Image"));
	AddField_Color(pTab, CAsset_MapLayerTiles::COLOR, _LSTRING("Color"));	
	
	pTab->AddSeparator();
	
	{
		gui::CHListLayout* pLayout = new gui::CHListLayout(Context());
		pTab->Add(pLayout, false);
		pLayout->Add(new CMapLayerTilesButton_HFlip(AssetsEditor()), true);
		pLayout->Add(new CMapLayerTilesButton_VFlip(AssetsEditor()), true);
	}
	
	return pTab;
}

/* MAP LAYER QUADS ****************************************************/

class CSubItemEditor_Quad : public gui::CVListLayout
{
protected:
	CGuiEditor* m_pAssetsEditor;
	
public:
	CSubItemEditor_Quad(CGuiEditor* pAssetsEditor) :
		gui::CVListLayout(pAssetsEditor),
		m_pAssetsEditor(pAssetsEditor)
	{
		
	}
	
	virtual void Update(bool ParentEnabled)
	{
		if(ParentEnabled)
		{
			if(
				m_pAssetsEditor->GetEditedSubPath().GetType() == CAsset_MapLayerQuads::TYPE_QUAD &&
				m_pAssetsEditor->GetEditedSubPath().GetId2() < CAsset_MapLayerQuads::VERTEX0
			)
				Enable();
			else
				Disable();
		}
		
		gui::CVListLayout::Update(ParentEnabled);
	}
};

class CSubItemEditor_QuadVertex : public gui::CVListLayout
{
protected:
	CGuiEditor* m_pAssetsEditor;
	int m_Vertex;
	
public:
	CSubItemEditor_QuadVertex(CGuiEditor* pAssetsEditor, int Vertex) :
		gui::CVListLayout(pAssetsEditor),
		m_pAssetsEditor(pAssetsEditor),
		m_Vertex(Vertex)
	{
		
	}
	
	virtual void Update(bool ParentEnabled)
	{
		if(ParentEnabled)
		{
			if(
				m_pAssetsEditor->GetEditedSubPath().GetType() == CAsset_MapLayerQuads::TYPE_QUAD &&
				m_pAssetsEditor->GetEditedSubPath().GetId2() == m_Vertex
			)
				Enable();
			else
				Disable();
		}
		
		gui::CVListLayout::Update(ParentEnabled);
	}
};

class CSubItemList_MapLayerQuads_Quad : public CSubItemList
{
protected:
	virtual void GenerateList()
	{
		Clear();
		const CAsset_MapLayerQuads* pLayer = AssetsManager()->GetAsset<CAsset_MapLayerQuads>(m_pAssetsEditor->GetEditedAssetPath());
		if(pLayer)
		{
			CLocalizableString LString(_("Quad {int:Id}"));
			
			CAsset_MapLayerQuads::CIteratorQuad Iter;
			int Counter = 1;
			for(Iter = pLayer->BeginQuad(); Iter != pLayer->EndQuad(); ++Iter)
			{
				LString.ClearParameters();
				LString.AddInteger("Id", Counter);
				Add(new CSubItem(m_pAssetsEditor, *Iter, LString, m_pAssetsEditor->m_Path_Sprite_IconQuad), false);
				Counter++;
			}
		}
	}

public:
	CSubItemList_MapLayerQuads_Quad(CGuiEditor* pAssetsEditor) :
		CSubItemList(pAssetsEditor, CAsset_MapLayerQuads::TypeId)
	{ }	
};

gui::CVScrollLayout* CAssetsInspector::CreateTab_MapLayerQuads_Asset()
{
	gui::CVScrollLayout* pTab = new gui::CVScrollLayout(Context());
	pTab->Disable();
	AddTab(pTab, _LSTRING("Map Layer Quads"), AssetsEditor()->m_Path_Sprite_IconQuad);
	
	AddField_AssetProperties(pTab);
	
	AddField_Asset(pTab, CAsset_MapLayerQuads::IMAGEPATH, CAsset_Image::TypeId, _LSTRING("Image"));
	AddField_Integer_NoEdit(pTab, CAsset_MapLayerQuads::QUAD_ARRAYSIZE, _LSTRING("Number of quads"));
	
	pTab->Add(new CSubItemList_MapLayerQuads_Quad(AssetsEditor()), true);
	
	{
		gui::CVListLayout* pQuadEditor = new CSubItemEditor_Quad(AssetsEditor());
		pTab->Add(pQuadEditor);
		
		AddField_Vec2(pQuadEditor, CAsset_MapLayerQuads::QUAD_PIVOT_X, CAsset_MapLayerQuads::QUAD_PIVOT_Y, _LSTRING("Position"));
		AddField_Vec2(pQuadEditor, CAsset_MapLayerQuads::QUAD_SIZE_X, CAsset_MapLayerQuads::QUAD_SIZE_Y, _LSTRING("Size"));
		AddField_Angle(pQuadEditor, CAsset_MapLayerQuads::QUAD_ANGLE, _LSTRING("Angle"));
		AddField_Asset(pQuadEditor, CAsset_MapLayerQuads::QUAD_ANIMATIONPATH, CAsset_SkeletonAnimation::TypeId, _LSTRING("Animation"));
	}
	
	{
		gui::CVListLayout* pQuadEditor = new CSubItemEditor_QuadVertex(AssetsEditor(), CAsset_MapLayerQuads::VERTEX0);
		pTab->Add(pQuadEditor);
		
		AddField_Vec2(pQuadEditor, CAsset_MapLayerQuads::QUAD_VERTEX0_X, CAsset_MapLayerQuads::QUAD_VERTEX0_Y, _LSTRING("Position"));
		AddField_Vec2(pQuadEditor, CAsset_MapLayerQuads::QUAD_UV0_X, CAsset_MapLayerQuads::QUAD_UV0_Y, _LSTRING("Texture"));
		AddField_Color(pQuadEditor, CAsset_MapLayerQuads::QUAD_COLOR0, _LSTRING("Color"));
	}
	
	{
		gui::CVListLayout* pQuadEditor = new CSubItemEditor_QuadVertex(AssetsEditor(), CAsset_MapLayerQuads::VERTEX1);
		pTab->Add(pQuadEditor);
		
		AddField_Vec2(pQuadEditor, CAsset_MapLayerQuads::QUAD_VERTEX1_X, CAsset_MapLayerQuads::QUAD_VERTEX1_Y, _LSTRING("Position"));
		AddField_Vec2(pQuadEditor, CAsset_MapLayerQuads::QUAD_UV1_X, CAsset_MapLayerQuads::QUAD_UV1_Y, _LSTRING("Texture"));
		AddField_Color(pQuadEditor, CAsset_MapLayerQuads::QUAD_COLOR1, _LSTRING("Color"));
	}
	
	{
		gui::CVListLayout* pQuadEditor = new CSubItemEditor_QuadVertex(AssetsEditor(), CAsset_MapLayerQuads::VERTEX2);
		pTab->Add(pQuadEditor);
		
		AddField_Vec2(pQuadEditor, CAsset_MapLayerQuads::QUAD_VERTEX2_X, CAsset_MapLayerQuads::QUAD_VERTEX2_Y, _LSTRING("Position"));
		AddField_Vec2(pQuadEditor, CAsset_MapLayerQuads::QUAD_UV2_X, CAsset_MapLayerQuads::QUAD_UV2_Y, _LSTRING("Texture"));
		AddField_Color(pQuadEditor, CAsset_MapLayerQuads::QUAD_COLOR2, _LSTRING("Color"));
	}
	
	{
		gui::CVListLayout* pQuadEditor = new CSubItemEditor_QuadVertex(AssetsEditor(), CAsset_MapLayerQuads::VERTEX3);
		pTab->Add(pQuadEditor);
		
		AddField_Vec2(pQuadEditor, CAsset_MapLayerQuads::QUAD_VERTEX3_X, CAsset_MapLayerQuads::QUAD_VERTEX3_Y, _LSTRING("Position"));
		AddField_Vec2(pQuadEditor, CAsset_MapLayerQuads::QUAD_UV3_X, CAsset_MapLayerQuads::QUAD_UV3_Y, _LSTRING("Texture"));
		AddField_Color(pQuadEditor, CAsset_MapLayerQuads::QUAD_COLOR3, _LSTRING("Color"));
	}
	
	return pTab;
}

/* MAP LAYER OBJECTS **************************************************/

class CSubItemList_MapLayerObjects_Object : public CSubItemList
{
protected:
	virtual void GenerateList()
	{
		Clear();
		const CAsset_MapLayerObjects* pLayer = AssetsManager()->GetAsset<CAsset_MapLayerObjects>(m_pAssetsEditor->GetEditedAssetPath());
		if(pLayer)
		{
			CLocalizableString LString(_("Object {int:Id}"));
			
			CAsset_MapLayerObjects::CIteratorObject Iter;
			int Counter = 1;
			for(Iter = pLayer->BeginObject(); Iter != pLayer->EndObject(); ++Iter)
			{
				LString.ClearParameters();
				LString.AddInteger("Id", Counter);
				Add(new CSubItem(m_pAssetsEditor, *Iter, LString, m_pAssetsEditor->m_Path_Sprite_IconPolygon), false);
				Counter++;
			}
		}
	}

public:
	CSubItemList_MapLayerObjects_Object(CGuiEditor* pAssetsEditor) :
		CSubItemList(pAssetsEditor, CAsset_MapLayerObjects::TypeId)
	{ }	
};

class CSubItemList_MapLayerObjects_Vertex : public gui::CVScrollLayout
{
protected:
	CAssetPath m_AssetPath;
	int m_ObjectId;
	int m_AssetVersion;
	CGuiEditor* m_pAssetsEditor;
	bool m_UpdateNeeded;
	
public:
	CSubItemList_MapLayerObjects_Vertex(CGuiEditor* pAssetsEditor) :
		gui::CVScrollLayout(pAssetsEditor),
		m_pAssetsEditor(pAssetsEditor),
		m_UpdateNeeded(true)
	{
		SetBoxStyle(m_pAssetsEditor->m_Path_Box_SubList);
	}
	
	virtual void Update(bool ParentEnabled)
	{
		if(ParentEnabled && IsEnabled())
		{
			if(m_pAssetsEditor->GetEditedAssetPath().GetType() == CAsset_MapLayerObjects::TypeId)
			{
				if(
					m_pAssetsEditor->GetEditedSubPath().GetType() == CAsset_MapLayerObjects::TYPE_OBJECT ||
					m_pAssetsEditor->GetEditedSubPath().GetType() == CAsset_MapLayerObjects::TYPE_OBJECT_VERTEX
				)
				{
					CAssetState* pState = AssetsManager()->GetAssetState(m_pAssetsEditor->GetEditedAssetPath());
					if(
						(m_AssetPath != m_pAssetsEditor->GetEditedAssetPath()) ||
						(m_ObjectId != m_pAssetsEditor->GetEditedSubPath().GetId()) ||
						(pState && m_AssetVersion != pState->m_NumUpdates)
					)
					{
						m_AssetPath = m_pAssetsEditor->GetEditedAssetPath();
						m_ObjectId = m_pAssetsEditor->GetEditedSubPath().GetId();
						m_AssetVersion = pState->m_NumUpdates;
						m_UpdateNeeded = true;
					}
				}
				else
				{
					CAssetState* pState = AssetsManager()->GetAssetState(m_pAssetsEditor->GetEditedAssetPath());
					if(
						(m_AssetPath != m_pAssetsEditor->GetEditedAssetPath()) ||
						(pState && m_AssetVersion != pState->m_NumUpdates)
					)
					{
						m_AssetPath = m_pAssetsEditor->GetEditedAssetPath();
						m_ObjectId = -1;
						m_AssetVersion = pState->m_NumUpdates;
						m_UpdateNeeded = true;
					}
				}	
			}
			
			CSubPath ObjectPath = CAsset_MapLayerObjects::SubPath_Object(m_ObjectId);
			
			if(m_ObjectId >= 0)
			{
				const CAsset_MapLayerObjects* pLayer = AssetsManager()->GetAsset<CAsset_MapLayerObjects>(m_pAssetsEditor->GetEditedAssetPath());
				if(pLayer && pLayer->IsValidObject(ObjectPath))
				{
					if(m_UpdateNeeded)
					{
						Clear();
						char aBuf[128];
							
						const CAsset_MapLayerObjects::CObject& Object = pLayer->GetObject(ObjectPath);
						
						for(int i=0; i<Object.GetVertexArraySize(); i++)
						{
							str_format(aBuf, sizeof(aBuf), "Vertex %d", i+1);
							CSubPath VertexPath = CAsset_MapLayerObjects::SubPath_ObjectVertex(m_pAssetsEditor->GetEditedSubPath().GetId(), i);
							Add(new CSubItem(m_pAssetsEditor, VertexPath, aBuf, m_pAssetsEditor->m_Path_Sprite_IconVertex), false);
						}
						
						m_UpdateNeeded = false;
					}
				}
			}
			else if(m_UpdateNeeded)
			{
				Clear();
				m_UpdateNeeded = false;
			}
		}
		
		gui::CVScrollLayout::Update(ParentEnabled);
	}
};

gui::CVScrollLayout* CAssetsInspector::CreateTab_MapLayerObjects_Asset()
{
	gui::CVScrollLayout* pTab = new gui::CVScrollLayout(Context());
	pTab->Disable();
	AddTab(pTab, _LSTRING("Map Layer Objects"), AssetsEditor()->m_Path_Sprite_IconPolygon);
	
	AddField_AssetProperties(pTab);
	
	pTab->Add(new CSubItemList_MapLayerObjects_Object(AssetsEditor()), true);
	pTab->Add(new CSubItemList_MapLayerObjects_Vertex(AssetsEditor()), true);
	
	gui::CVListLayout* pObjectEditor = new CSubItemEditor(AssetsEditor(), CAsset_MapLayerObjects::TYPE_OBJECT);
	pTab->Add(pObjectEditor, false);
	
	AddField_Asset(pObjectEditor, CAsset_MapLayerObjects::OBJECT_STYLEPATH, CAsset_Material::TypeId, _LSTRING("Style"));	
	AddField_Vec2(pObjectEditor, CAsset_MapLayerObjects::OBJECT_POSITION_X, CAsset_MapLayerObjects::OBJECT_POSITION_Y, _LSTRING("Position"));	
	AddField_Vec2(pObjectEditor, CAsset_MapLayerObjects::OBJECT_SIZE_X, CAsset_MapLayerObjects::OBJECT_SIZE_Y, _LSTRING("Size"));	
	AddField_Angle(pObjectEditor, CAsset_MapLayerObjects::OBJECT_ANGLE, _LSTRING("Angle"));
	AddField_Bool(pObjectEditor, CAsset_MapLayerObjects::OBJECT_CLOSEDPATH, _LSTRING("Closed"));
	
	gui::CVListLayout* pVertexEditor = new CSubItemEditor(AssetsEditor(), CAsset_MapLayerObjects::TYPE_OBJECT_VERTEX);
	pTab->Add(pVertexEditor, false);
	
	AddField_Vec2(pVertexEditor, CAsset_MapLayerObjects::OBJECT_VERTEX_POSITION_X, CAsset_MapLayerObjects::OBJECT_VERTEX_POSITION_Y, _LSTRING("Position"));	
	AddField_Color(pVertexEditor, CAsset_MapLayerObjects::OBJECT_VERTEX_COLOR, _LSTRING("Color"));
	AddField_Float(pVertexEditor, CAsset_MapLayerObjects::OBJECT_VERTEX_WEIGHT, _LSTRING("Weight"));
	
	{
		CMemberComboBox* pComboBox = new CMemberComboBox(AssetsEditor(), CAsset_MapLayerObjects::OBJECT_VERTEX_SMOOTHNESS);
		pComboBox->Add(_LSTRING("Corner"), AssetsEditor()->m_Path_Sprite_IconVertexCorner);
		pComboBox->Add(_LSTRING("Free"), AssetsEditor()->m_Path_Sprite_IconVertexFree);
		pComboBox->Add(_LSTRING("Aligned"), AssetsEditor()->m_Path_Sprite_IconVertexAligned);
		pComboBox->Add(_LSTRING("Symetric"), AssetsEditor()->m_Path_Sprite_IconVertexSymetric);
		pComboBox->Add(_LSTRING("Automatic"), AssetsEditor()->m_Path_Sprite_IconVertexSmooth);
		AddField(pVertexEditor, pComboBox, _LSTRING("Smoothness type"));
	}
	
	return pTab;
}

/* MAP ZONE TILES *****************************************************/

gui::CVScrollLayout* CAssetsInspector::CreateTab_MapZoneTiles_Asset()
{
	gui::CVScrollLayout* pTab = new gui::CVScrollLayout(Context());
	pTab->Disable();
	AddTab(pTab, _LSTRING("Map Zone Tiles"), AssetsEditor()->m_Path_Sprite_IconTiles);
	
	AddField_AssetProperties(pTab);
		
	AddField_Integer(pTab, CAsset_MapZoneTiles::TILE_WIDTH, _LSTRING("Width"));	
	AddField_Integer(pTab, CAsset_MapZoneTiles::TILE_HEIGHT, _LSTRING("Height"));
	AddField_Asset(pTab, CAsset_MapZoneTiles::ZONETYPEPATH, CAsset_ZoneType::TypeId, _LSTRING("Zone type"));
	
	return pTab;
}

/* MAP ENTITIES *******************************************************/

class CSubItemList_MapEntities_Entity : public CSubItemList
{
protected:
	CAssetPath m_AssetPath;
	bool m_UpdateNeeded;
	
protected:
	virtual void GenerateList()
	{
		Clear();
		const CAsset_MapEntities* pEntities = AssetsManager()->GetAsset<CAsset_MapEntities>(m_pAssetsEditor->GetEditedAssetPath());
		if(pEntities)
		{
			CAsset_MapEntities::CIteratorEntity Iter;
			for(Iter = pEntities->BeginEntity(); Iter != pEntities->EndEntity(); ++Iter)
			{
				CAssetPath TypePath = pEntities->GetEntityTypePath(*Iter);
				const CAsset_EntityType* pEntityType = AssetsManager()->GetAsset<CAsset_EntityType>(TypePath);
				if(pEntityType)
					Add(new CSubItem(m_pAssetsEditor, *Iter, pEntityType->GetName(), m_pAssetsEditor->m_Path_Sprite_IconEntities), false);
				else
					Add(new CSubItem(m_pAssetsEditor, *Iter, _LSTRING("Unknown entity"), m_pAssetsEditor->m_Path_Sprite_IconEntities), false);
			}
			
			m_UpdateNeeded = false;
		}
	}

public:
	CSubItemList_MapEntities_Entity(CGuiEditor* pAssetsEditor) :
		CSubItemList(pAssetsEditor, CAsset_MapEntities::TypeId)
	{ }
};

gui::CVScrollLayout* CAssetsInspector::CreateTab_MapEntities_Asset()
{
	gui::CVScrollLayout* pTab = new gui::CVScrollLayout(Context());
	pTab->Disable();
	AddTab(pTab, _LSTRING("Map Entities"), AssetsEditor()->m_Path_Sprite_IconEntities);
	
	AddField_AssetProperties(pTab);
	
	pTab->Add(new CSubItemList_MapEntities_Entity(AssetsEditor()), true);
	
	return pTab;
}

/* ZONE TYPE **********************************************************/

class CSubItemList_ZoneTypeIndices : public gui::CVScrollLayout
{
protected:
	CAssetPath m_AssetPath;
	CGuiEditor* m_pAssetsEditor;
	bool m_UpdateNeeded;
	
public:
	CSubItemList_ZoneTypeIndices(CGuiEditor* pAssetsEditor) :
		gui::CVScrollLayout(pAssetsEditor),
		m_pAssetsEditor(pAssetsEditor),
		m_UpdateNeeded(true)
	{
		SetBoxStyle(m_pAssetsEditor->m_Path_Box_SubList);
	}
	
	virtual void Update(bool ParentEnabled)
	{
		if(ParentEnabled && IsEnabled())
		{
			if(m_pAssetsEditor->GetEditedAssetPath().GetType() == CAsset_ZoneType::TypeId && m_AssetPath != m_pAssetsEditor->GetEditedAssetPath())
			{
				m_AssetPath = m_pAssetsEditor->GetEditedAssetPath();
				m_UpdateNeeded = true;
			}
			
			if(m_UpdateNeeded)
			{
				Clear();
				
				const CAsset_ZoneType* pZoneType = AssetsManager()->GetAsset<CAsset_ZoneType>(m_pAssetsEditor->GetEditedAssetPath());
				if(pZoneType)
				{
					CAsset_ZoneType::CIteratorIndex Iter;
					for(Iter = pZoneType->BeginIndex(); Iter != pZoneType->EndIndex(); ++Iter)
					{
						Add(new CSubItem(m_pAssetsEditor, *Iter, pZoneType->GetIndexDescription(*Iter), m_pAssetsEditor->m_Path_Sprite_IconZoneTiles), false);
					}
					
					m_UpdateNeeded = false;
				}
			}
		}
		
		gui::CVScrollLayout::Update(ParentEnabled);
	}
};

gui::CVScrollLayout* CAssetsInspector::CreateTab_ZoneType_Asset()
{
	gui::CVScrollLayout* pTab = new gui::CVScrollLayout(Context());
	pTab->Disable();
	AddTab(pTab, _LSTRING("Zone Type"), AssetsEditor()->m_Path_Sprite_IconZoneType);
	
	AddField_AssetProperties(pTab);
	
	pTab->Add(new CSubItemList_ZoneTypeIndices(AssetsEditor()), true);
	
	gui::CVListLayout* pEditor = new CSubItemEditor(AssetsEditor(), CAsset_ZoneType::TYPE_INDEX);
	pTab->Add(pEditor);
	
	AddField_Bool(pEditor, CAsset_ZoneType::INDEX_USED, _LSTRING("Is used"));
	AddField_Text(pEditor, CAsset_ZoneType::INDEX_DESCRIPTION, _LSTRING("Description"));
	AddField_Color(pEditor, CAsset_ZoneType::INDEX_COLOR, _LSTRING("Color"));
	
	return pTab;
}

/* ENTITY TYPE ********************************************************/

gui::CVScrollLayout* CAssetsInspector::CreateTab_EntityType_Asset()
{
	gui::CVScrollLayout* pTab = new gui::CVScrollLayout(Context());
	pTab->Disable();
	AddTab(pTab, _LSTRING("Entity Type"), AssetsEditor()->m_Path_Sprite_IconEntityType);
	
	AddField_AssetProperties(pTab);
	
	AddField_Asset(pTab, CAsset_EntityType::GIZMOPATH, CAsset_Sprite::TypeId, _LSTRING("Gizmo"));	
	AddField_Float(pTab, CAsset_EntityType::COLLISIONRADIUS, _LSTRING("Collision Radius"));	
	
	return pTab;
}

/* MATERIAL ***********************************************************/

class CNewSubItemButton_Material_Layer : public gui::CButton
{
protected:
	CGuiEditor* m_pAssetsEditor;
	CSubPath m_SubPath;
	int m_Type;
	
protected:
	virtual void MouseClickAction()
	{
		int Token = AssetsManager()->GenerateToken();
		int Id = AssetsManager()->AddSubItem(m_pAssetsEditor->GetEditedAssetPath(), m_SubPath, m_Type, Token);
		CSubPath SubPath = CAsset_Material::SubPath_Layer(Id);
		m_pAssetsEditor->SetEditedAsset(m_pAssetsEditor->GetEditedAssetPath(), SubPath);
		
		CAssetState* pState = AssetsManager()->GetAssetState(m_pAssetsEditor->GetEditedAssetPath());
		pState->m_NumUpdates++;
	}

public:
	CNewSubItemButton_Material_Layer(CGuiEditor* pAssetsEditor, int Type, CSubPath SubPath, const CLocalizableString& LString, CAssetPath IconPath) :
		gui::CButton(pAssetsEditor, LString, IconPath),
		m_pAssetsEditor(pAssetsEditor),
		m_SubPath(SubPath),
		m_Type(Type)
	{
		
	}
};

class CNewSubItemButton_Material_Sprite : public gui::CButton
{
protected:
	CGuiEditor* m_pAssetsEditor;
	
protected:
	virtual void MouseClickAction()
	{
		const CAsset_Material* pMaterial = AssetsManager()->GetAsset<CAsset_Material>(m_pAssetsEditor->GetEditedAssetPath());
		if(pMaterial && pMaterial->IsValidLayer(m_pAssetsEditor->GetEditedSubPath()))
		{
			int Token = AssetsManager()->GenerateToken();
			AssetsManager()->AddSubItem(m_pAssetsEditor->GetEditedAssetPath(), m_pAssetsEditor->GetEditedSubPath(), CAsset_Material::TYPE_LAYER_SPRITE, Token);
			
			CAssetState* pState = AssetsManager()->GetAssetState(m_pAssetsEditor->GetEditedAssetPath());
			pState->m_NumUpdates++;
		}
	}

public:
	CNewSubItemButton_Material_Sprite(CGuiEditor* pAssetsEditor) :
		gui::CButton(pAssetsEditor, _LSTRING("Add Sprite"), pAssetsEditor->m_Path_Sprite_IconSprite),
		m_pAssetsEditor(pAssetsEditor)
	{
		
	}
};

class CSubItemList_Material_Layer : public CSubItemList
{
protected:
	virtual void GenerateList()
	{
		Clear();
		const CAsset_Material* pLayer = AssetsManager()->GetAsset<CAsset_Material>(m_pAssetsEditor->GetEditedAssetPath());
		if(pLayer)
		{
			CLocalizableString LString_Layer(_("Layer {int:Id}"));
			CLocalizableString LString_Sprite(_("Sprite {int:Id}"));
			
			CAsset_Material::CIteratorLayer LayerIter;
			int LayerCounter = 1;
			for(LayerIter = pLayer->BeginLayer(); LayerIter != pLayer->EndLayer(); ++LayerIter)
			{
				gui::CExpand* pExpand = new gui::CExpand(Context());
				Add(pExpand, false);
				
				LString_Layer.ClearParameters();
				LString_Layer.AddInteger("Id", LayerCounter);
				pExpand->SetTitle(new CSubItem(m_pAssetsEditor, *LayerIter, LString_Layer, m_pAssetsEditor->m_Path_Sprite_IconLayer));
				LayerCounter++;
				
				const array< CAsset_Material::CSprite, allocator_copy<CAsset_Material::CSprite> >& SpriteArray = pLayer->GetLayerSpriteArray(*LayerIter);
				for(int i=0; i<SpriteArray.size(); i++)
				{
					CSubPath SpriteSubPath = CAsset_Material::SubPath_LayerSprite((*LayerIter).GetId(), i);
					LString_Sprite.ClearParameters();
					LString_Sprite.AddInteger("Id", i+1);
					pExpand->Add(new CSubItem(m_pAssetsEditor, SpriteSubPath, LString_Sprite, m_pAssetsEditor->m_Path_Sprite_IconSprite));
				}
			}
		}
	}

public:
	CSubItemList_Material_Layer(CGuiEditor* pAssetsEditor) :
		CSubItemList(pAssetsEditor, CAsset_Material::TypeId)
	{ }	
};

class 

gui::CVScrollLayout* CAssetsInspector::CreateTab_Material_Asset()
{
	gui::CVScrollLayout* pTab = new gui::CVScrollLayout(Context());
	pTab->Disable();
	AddTab(pTab, _LSTRING("Material"), AssetsEditor()->m_Path_Sprite_IconLineStyle);
	
	AddField_AssetProperties(pTab);
	
	AddField_Bool(pTab, CAsset_Material::TEXTUREENABLED, _LSTRING("Filling Enabled"));
	
	gui::CVListLayout* pTextureEditor = new CConditionalEditor(AssetsEditor(), CAsset_Material::TEXTUREENABLED);
	pTab->Add(pTextureEditor, false);
	AddField_Asset(pTextureEditor, CAsset_Material::TEXTUREPATH, CAsset_Image::TypeId, _LSTRING("Texture"));
	AddField_Color(pTextureEditor, CAsset_Material::TEXTURECOLOR, _LSTRING("Texture Color"));
	AddField_Vec2(pTextureEditor, CAsset_Material::TEXTURESIZE_X, CAsset_Material::TEXTURESIZE_Y, _LSTRING("Texture Size"));
	AddField_Angle(pTextureEditor, CAsset_Material::TEXTUREANGLE, _LSTRING("Texture Angle"));
	AddField_Float(pTextureEditor, CAsset_Material::TEXTURESPACING, _LSTRING("Texture Spacing"));
	
	pTab->AddSeparator();
	
	pTab->Add(new CNewSubItemButton_Material_Layer(AssetsEditor(), CAsset_Material::TYPE_LAYER, CSubPath::Null(), _LSTRING("New Layer"), AssetsEditor()->m_Path_Sprite_IconLayer), false);
	pTab->Add(new CSubItemList_Material_Layer(AssetsEditor()), true);
	
	gui::CVListLayout* pLayerEditor = new CSubItemEditor(AssetsEditor(), CAsset_Material::TYPE_LAYER);
	pTab->Add(pLayerEditor, false);
	
	AddField(pLayerEditor, new CNewSubItemButton_Material_Sprite(AssetsEditor()));
	AddField_Float(pLayerEditor, CAsset_Material::LAYER_SPACING, _LSTRING("Spacing"));
	
	{
		CMemberComboBox* pComboBox = new CMemberComboBox(AssetsEditor(), CAsset_Material::LAYER_REPEATTYPE);
		pComboBox->Add(_LSTRING("Repeated"), AssetsEditor()->m_Path_Sprite_IconMatLayerRepeat);
		pComboBox->Add(_LSTRING("Stretched"), AssetsEditor()->m_Path_Sprite_IconMatLayerStretch);
		AddField(pLayerEditor, pComboBox, _LSTRING("Sprite arrangment"));
	}
	
	gui::CVListLayout* pSpriteEditor = new CSubItemEditor(AssetsEditor(), CAsset_Material::TYPE_LAYER_SPRITE);
	pTab->Add(pSpriteEditor, false);	
	
	AddField_Asset(pSpriteEditor, CAsset_Material::LAYER_SPRITE_PATH, CAsset_Sprite::TypeId, _LSTRING("Sprite"));
	AddField_Vec2(pSpriteEditor, CAsset_Material::LAYER_SPRITE_POSITION_X, CAsset_Material::LAYER_SPRITE_POSITION_Y, _LSTRING("Position"));
	AddField_Vec2(pSpriteEditor, CAsset_Material::LAYER_SPRITE_SIZE_X, CAsset_Material::LAYER_SPRITE_SIZE_Y, _LSTRING("Size"));
	AddField_Color(pSpriteEditor, CAsset_Material::LAYER_SPRITE_COLOR, _LSTRING("Color"));
	
	{
		CMemberComboBox* pComboBox = new CMemberComboBox(AssetsEditor(), CAsset_Material::LAYER_SPRITE_ALIGNMENT);
		pComboBox->Add(_LSTRING("Line"));
		pComboBox->Add(_LSTRING("World"));
		AddField(pSpriteEditor, pComboBox, _LSTRING("Alignment"));
	}
	
	AddField_Flag(pSpriteEditor, CAsset_Material::LAYER_SPRITE_FLAGS, CAsset_Material::SPRITEFLAG_VFLIP, _LSTRING("Horizontal Flip"));
	AddField_Flag(pSpriteEditor, CAsset_Material::LAYER_SPRITE_FLAGS, CAsset_Material::SPRITEFLAG_HFLIP, _LSTRING("Vertical Flip"));
	AddField_Flag(pSpriteEditor, CAsset_Material::LAYER_SPRITE_FLAGS, CAsset_Material::SPRITEFLAG_ROTATION, _LSTRING("Rotation of 90°"));
	
	return pTab;
}

/* GUI RECT STYLE *****************************************************/

gui::CVScrollLayout* CAssetsInspector::CreateTab_GuiRectStyle_Asset()
{
	gui::CVScrollLayout* pTab = new gui::CVScrollLayout(Context());
	pTab->Disable();
	AddTab(pTab, _LSTRING("Rectangle Style (GUI)"), AssetsEditor()->m_Path_Sprite_IconGuiRect);
	
	AddField_AssetProperties(pTab);
	
	return pTab;
}

gui::CVScrollLayout* CAssetsInspector::CreateTab_GuiLineStyle_Asset()
{
	gui::CVScrollLayout* pTab = new gui::CVScrollLayout(Context());
	pTab->Disable();
	AddTab(pTab, _LSTRING("Line Style (GUI)"), AssetsEditor()->m_Path_Sprite_IconGuiLine);
	
	//~ AddField_Asset(pTab, CAsset_GuiLineStyle::RECTPATH, CAsset_GuiRectStyle::TypeId, "Rectangle");
	
	AddField_AssetProperties(pTab);
	
	return pTab;
}

gui::CVScrollLayout* CAssetsInspector::CreateTab_GuiBoxStyle_Asset()
{
	gui::CVScrollLayout* pTab = new gui::CVScrollLayout(Context());
	pTab->Disable();
	AddTab(pTab, _LSTRING("Box Style (GUI)"), AssetsEditor()->m_Path_Sprite_IconGuiBox);
	
	AddField_AssetProperties(pTab);
	
	AddField_Asset(pTab, CAsset_GuiBoxStyle::RECTPATH, CAsset_GuiRectStyle::TypeId, _LSTRING("Rectangle"));
	AddField_Integer(pTab, CAsset_GuiBoxStyle::MINWIDTH, _LSTRING("Width"));
	AddField_Integer(pTab, CAsset_GuiBoxStyle::MINHEIGHT, _LSTRING("Height"));
	AddField_Integer(pTab, CAsset_GuiBoxStyle::MARGIN, _LSTRING("Margin"));
	AddField_Integer(pTab, CAsset_GuiBoxStyle::PADDING, _LSTRING("Padding"));
	AddField_Integer(pTab, CAsset_GuiBoxStyle::SPACING, _LSTRING("Spacing"));
	
	return pTab;
}

gui::CVScrollLayout* CAssetsInspector::CreateTab_GuiLabelStyle_Asset()
{
	gui::CVScrollLayout* pTab = new gui::CVScrollLayout(Context());
	pTab->Disable();
	AddTab(pTab, _LSTRING("Label Style (GUI)"), AssetsEditor()->m_Path_Sprite_IconGuiLabel);
	
	AddField_AssetProperties(pTab);
	
	AddField_Asset(pTab, CAsset_GuiLabelStyle::RECTPATH, CAsset_GuiRectStyle::TypeId, _LSTRING("Rectangle"));
	AddField_Integer(pTab, CAsset_GuiLabelStyle::MINWIDTH, _LSTRING("Width"));
	AddField_Integer(pTab, CAsset_GuiLabelStyle::MINHEIGHT, _LSTRING("Height"));
	AddField_Integer(pTab, CAsset_GuiLabelStyle::MARGIN, _LSTRING("Margin"));
	AddField_Integer(pTab, CAsset_GuiLabelStyle::PADDING, _LSTRING("Padding"));
	AddField_Integer(pTab, CAsset_GuiLabelStyle::SPACING, _LSTRING("Spacing"));
	AddField_Integer(pTab, CAsset_GuiLabelStyle::FONTSIZE, _LSTRING("Font Size"));
	
	return pTab;
}

gui::CVScrollLayout* CAssetsInspector::CreateTab_GuiButtonStyle_Asset()
{
	gui::CVScrollLayout* pTab = new gui::CVScrollLayout(Context());
	pTab->Disable();
	AddTab(pTab, _LSTRING("Button Style (GUI)"), AssetsEditor()->m_Path_Sprite_IconGuiButton);
	
	AddField_AssetProperties(pTab);
	
	AddField_Asset(pTab, CAsset_GuiButtonStyle::IDLESTYLEPATH, CAsset_GuiLabelStyle::TypeId, _LSTRING("Idle Style"));
	AddField_Asset(pTab, CAsset_GuiButtonStyle::MOUSEOVERSTYLEPATH, CAsset_GuiLabelStyle::TypeId, _LSTRING("Mouseover Style"));
	AddField_Asset(pTab, CAsset_GuiButtonStyle::READONLYSTYLEPATH, CAsset_GuiLabelStyle::TypeId, _LSTRING("Read Only Style"));
	
	return pTab;
}

gui::CVScrollLayout* CAssetsInspector::CreateTab_GuiIntEditStyle_Asset()
{
	gui::CVScrollLayout* pTab = new gui::CVScrollLayout(Context());
	pTab->Disable();
	AddTab(pTab, _LSTRING("Integer Edit Style (GUI)"), AssetsEditor()->m_Path_Sprite_IconGuiButton);
	
	AddField_AssetProperties(pTab);
	
	AddField_Asset(pTab, CAsset_GuiIntEditStyle::INCREASEBUTTONSTYLEPATH, CAsset_GuiButtonStyle::TypeId, _LSTRING("Increase Button"));
	AddField_Asset(pTab, CAsset_GuiIntEditStyle::DECREASEBUTTONSTYLEPATH, CAsset_GuiButtonStyle::TypeId, _LSTRING("Decrease Button"));
	
	return pTab;
}

gui::CVScrollLayout* CAssetsInspector::CreateTab_GuiToggleStyle_Asset()
{
	gui::CVScrollLayout* pTab = new gui::CVScrollLayout(Context());
	pTab->Disable();
	AddTab(pTab, _LSTRING("Toggle Style (GUI)"), AssetsEditor()->m_Path_Sprite_IconGuiToggle);
	
	AddField_AssetProperties(pTab);
	
	AddField_Asset(pTab, CAsset_GuiToggleStyle::IDLETRUESTYLEPATH, CAsset_GuiLabelStyle::TypeId, _LSTRING("Idle Style (true)"));
	AddField_Asset(pTab, CAsset_GuiToggleStyle::MOUSEOVERTRUESTYLEPATH, CAsset_GuiLabelStyle::TypeId, _LSTRING("Mouseover Style (true)"));
	AddField_Asset(pTab, CAsset_GuiToggleStyle::IDLEFALSESTYLEPATH, CAsset_GuiLabelStyle::TypeId, _LSTRING("Idle Style (false)"));
	AddField_Asset(pTab, CAsset_GuiToggleStyle::MOUSEOVERFALSESTYLEPATH, CAsset_GuiLabelStyle::TypeId, _LSTRING("Mouseover Style (false)"));
	
	return pTab;
}

gui::CVScrollLayout* CAssetsInspector::CreateTab_GuiSliderStyle_Asset()
{
	gui::CVScrollLayout* pTab = new gui::CVScrollLayout(Context());
	pTab->Disable();
	AddTab(pTab, _LSTRING("Slider Style"), AssetsEditor()->m_Path_Sprite_IconGuiSlider);
	
	AddField_AssetProperties(pTab);
	
	return pTab;
}

gui::CVScrollLayout* CAssetsInspector::CreateTab_GuiScrollbarStyle_Asset()
{
	gui::CVScrollLayout* pTab = new gui::CVScrollLayout(Context());
	pTab->Disable();
	AddTab(pTab, _LSTRING("Scrollbar Style"), AssetsEditor()->m_Path_Sprite_IconGuiScrollbar);
	
	AddField_AssetProperties(pTab);
	
	return pTab;
}

gui::CVScrollLayout* CAssetsInspector::CreateTab_GuiTabsStyle_Asset()
{
	gui::CVScrollLayout* pTab = new gui::CVScrollLayout(Context());
	pTab->Disable();
	AddTab(pTab, _LSTRING("Tabs Style"), AssetsEditor()->m_Path_Sprite_IconGuiTabs);
	
	AddField_AssetProperties(pTab);
	
	return pTab;
}

void CAssetsInspector::AddField(gui::CVListLayout* pList, gui::CWidget* pWidget, const CLocalizableString& Text)
{
	gui::CHListLayout* pLayout = new gui::CHListLayout(Context());
	pList->Add(pLayout, false);
	
	gui::CLabel* pLabel = new gui::CLabel(Context(), Text);
	pLayout->Add(pLabel, true);
	
	pLayout->Add(pWidget, true);
}

void CAssetsInspector::AddField(gui::CVListLayout* pList, gui::CWidget* pWidget)
{
	pList->Add(pWidget, false);
}

void CAssetsInspector::AddField_AssetProperties(gui::CVScrollLayout* pTab)
{
	AddField_Text(pTab, CAsset::NAME, _LSTRING("Name"));
	
	pTab->AddSeparator();
}

/* TEXT EDIT **********************************************************/

class CMemberTextEdit : public gui::CAbstractTextEdit
{
protected:
	CGuiEditor* m_pAssetsEditor;
	int m_Member;
	
	virtual void SaveFromTextBuffer()
	{
		AssetsManager()->SetAssetValue<const char*>(
			m_pAssetsEditor->GetEditedAssetPath(),
			m_pAssetsEditor->GetEditedSubPath(),
			m_Member,
			GetText()
		);
	}
	
	virtual void CopyToTextBuffer()
	{
		const char* pName = AssetsManager()->GetAssetValue<const char*>(
			m_pAssetsEditor->GetEditedAssetPath(),
			m_pAssetsEditor->GetEditedSubPath(),
			m_Member,
			NULL
		);
		
		if(pName)
		{
			if(m_Text != pName)
				SetText(pName);
		}
		else
			SetText("");
	}
	
public:
	CMemberTextEdit(CGuiEditor* pAssetsEditor, int Member) :
		gui::CAbstractTextEdit(pAssetsEditor),
		m_pAssetsEditor(pAssetsEditor),
		m_Member(Member)
	{ }
	
	virtual void Update(bool ParentEnabled)
	{
		if(IsEnabled() && ParentEnabled)
		{
			if(!AssetsManager()->IsValidPackage(m_pAssetsEditor->GetEditedPackageId()) || AssetsManager()->IsReadOnlyPackage(m_pAssetsEditor->GetEditedPackageId()))
				Editable(false);
			else
				Editable(true);
		}
		
		gui::CAbstractTextEdit::Update(ParentEnabled);
	}
};

void CAssetsInspector::AddField_Text(gui::CVListLayout* pList, int Member, const CLocalizableString& Text)
{
	CMemberTextEdit* pWidget = new CMemberTextEdit(
		m_pAssetsEditor,
		Member
	);
	
	AddField(pList, pWidget, Text);
}

/* INTEGER EDIT *******************************************************/
	
class CMemberIntegerEdit : public gui::CAbstractIntegerEdit
{
protected:
	CGuiEditor* m_pAssetsEditor;
	int m_Member;
	bool m_NoEdit;
	
	virtual int GetValue() const
	{
		return m_pAssetsEditor->AssetsManager()->GetAssetValue<int>(
			m_pAssetsEditor->GetEditedAssetPath(),
			m_pAssetsEditor->GetEditedSubPath(),
			m_Member,
			0
		);
	}
	
	virtual void SetValue(int Value)
	{
		m_pAssetsEditor->AssetsManager()->SetAssetValue<int>(
			m_pAssetsEditor->GetEditedAssetPath(),
			m_pAssetsEditor->GetEditedSubPath(),
			m_Member,
			Value
		);
	}
	
public:
	CMemberIntegerEdit(CGuiEditor* pAssetsEditor, int Member, bool NoEdit = false) :
		gui::CAbstractIntegerEdit(pAssetsEditor),
		m_pAssetsEditor(pAssetsEditor),
		m_Member(Member),
		m_NoEdit(NoEdit)
	{
		if(m_NoEdit)
			Editable(false);
	}
	
	virtual void Update(bool ParentEnabled)
	{
		if(!m_NoEdit && IsEnabled() && ParentEnabled)
		{
			if(!AssetsManager()->IsValidPackage(m_pAssetsEditor->GetEditedPackageId()) || AssetsManager()->IsReadOnlyPackage(m_pAssetsEditor->GetEditedPackageId()))
				Editable(false);
			else
				Editable(true);
		}
		
		gui::CAbstractIntegerEdit::Update(ParentEnabled);
	}
};

void CAssetsInspector::AddField_Integer(gui::CVListLayout* pList, int Member, const CLocalizableString& Text)
{
	CMemberIntegerEdit* pWidget = new CMemberIntegerEdit(
		m_pAssetsEditor,
		Member
	);
	
	AddField(pList, pWidget, Text);
}

void CAssetsInspector::AddField_Integer_NoEdit(gui::CVListLayout* pList, int Member, const CLocalizableString& Text)
{
	CMemberIntegerEdit* pWidget = new CMemberIntegerEdit(
		m_pAssetsEditor,
		Member,
		true
	);
	
	AddField(pList, pWidget, Text);
}

/* BOOL EDIT **********************************************************/
	
class CMemberBoolEdit : public gui::CAbstractToggle
{
protected:
	CGuiEditor* m_pAssetsEditor;
	int m_Member;
	
	virtual bool GetValue()
	{
		return m_pAssetsEditor->AssetsManager()->GetAssetValue<bool>(
			m_pAssetsEditor->GetEditedAssetPath(),
			m_pAssetsEditor->GetEditedSubPath(),
			m_Member,
			0
		);
	}
	
	virtual void SetValue(bool Value)
	{
		m_pAssetsEditor->AssetsManager()->SetAssetValue<bool>(
			m_pAssetsEditor->GetEditedAssetPath(),
			m_pAssetsEditor->GetEditedSubPath(),
			m_Member,
			Value
		);
	}
	
public:
	CMemberBoolEdit(CGuiEditor* pAssetsEditor, int Member) :
		gui::CAbstractToggle(pAssetsEditor),
		m_pAssetsEditor(pAssetsEditor),
		m_Member(Member)
	{ }
};

void CAssetsInspector::AddField_Bool(gui::CVListLayout* pList, int Member, const CLocalizableString& Text)
{
	CMemberBoolEdit* pWidget = new CMemberBoolEdit(
		m_pAssetsEditor,
		Member
	);
	
	AddField(pList, pWidget, Text);
}

/* FLAG EDIT **********************************************************/
	
class CMemberFlagEdit : public gui::CAbstractToggle
{
protected:
	CGuiEditor* m_pAssetsEditor;
	int m_Member;
	int m_Mask;
	
	virtual bool GetValue()
	{
		return m_pAssetsEditor->AssetsManager()->GetAssetValue<int>(
			m_pAssetsEditor->GetEditedAssetPath(),
			m_pAssetsEditor->GetEditedSubPath(),
			m_Member,
			0
		) & m_Mask;
	}
	
	virtual void SetValue(bool Value)
	{
		int Flags = m_pAssetsEditor->AssetsManager()->GetAssetValue<int>(
			m_pAssetsEditor->GetEditedAssetPath(),
			m_pAssetsEditor->GetEditedSubPath(),
			m_Member,
			0
		);
		
		if(Value)
			Flags |= m_Mask;
		else
			Flags &= ~m_Mask;
		
		m_pAssetsEditor->AssetsManager()->SetAssetValue<int>(
			m_pAssetsEditor->GetEditedAssetPath(),
			m_pAssetsEditor->GetEditedSubPath(),
			m_Member,
			Flags
		);
	}
	
public:
	CMemberFlagEdit(CGuiEditor* pAssetsEditor, int Member, int Mask) :
		gui::CAbstractToggle(pAssetsEditor),
		m_pAssetsEditor(pAssetsEditor),
		m_Member(Member),
		m_Mask(Mask)
	{ }
};

void CAssetsInspector::AddField_Flag(gui::CVListLayout* pList, int Member, int Mask, const CLocalizableString& Text)
{
	CMemberFlagEdit* pWidget = new CMemberFlagEdit(
		m_pAssetsEditor,
		Member,
		Mask
	);
	
	AddField(pList, pWidget, Text);
}

/* FLOAT EDIT *********************************************************/
	
class CMemberFloatEdit : public gui::CAbstractFloatEdit
{
protected:
	CGuiEditor* m_pAssetsEditor;
	int m_Member;
	
	virtual float GetValue() const
	{
		return m_pAssetsEditor->AssetsManager()->GetAssetValue<float>(
			m_pAssetsEditor->GetEditedAssetPath(),
			m_pAssetsEditor->GetEditedSubPath(),
			m_Member,
			0
		);
	}
	
	virtual void SetValue(float Value)
	{
		m_pAssetsEditor->AssetsManager()->SetAssetValue<float>(
			m_pAssetsEditor->GetEditedAssetPath(),
			m_pAssetsEditor->GetEditedSubPath(),
			m_Member,
			Value
		);
	}
	
public:
	CMemberFloatEdit(CGuiEditor* pAssetsEditor, int Member) :
		gui::CAbstractFloatEdit(pAssetsEditor),
		m_pAssetsEditor(pAssetsEditor),
		m_Member(Member)
	{ }
	
	virtual void Update(bool ParentEnabled)
	{
		if(IsEnabled() && ParentEnabled)
		{
			if(!AssetsManager()->IsValidPackage(m_pAssetsEditor->GetEditedPackageId()) || AssetsManager()->IsReadOnlyPackage(m_pAssetsEditor->GetEditedPackageId()))
				Editable(false);
			else
				Editable(true);
		}
		
		gui::CAbstractFloatEdit::Update(ParentEnabled);
	}
};

void CAssetsInspector::AddField_Float(gui::CVListLayout* pList, int Member, const CLocalizableString& Text)
{
	CMemberFloatEdit* pWidget = new CMemberFloatEdit(
		m_pAssetsEditor,
		Member
	);
	
	AddField(pList, pWidget, Text);
}

void CAssetsInspector::AddField_Vec2(gui::CVListLayout* pList, int Member, int Member2, const CLocalizableString& Text)
{
	gui::CHListLayout* pLayout = new gui::CHListLayout(Context());
	
	pLayout->Add(new CMemberFloatEdit(
		m_pAssetsEditor,
		Member
	), true);
	pLayout->Add(new CMemberFloatEdit(
		m_pAssetsEditor,
		Member2
	), true);
	
	AddField(pList, pLayout, Text);
}

/* FLOAT EDIT *********************************************************/
	
class CMemberAngleEdit : public gui::CAbstractFloatEdit
{
protected:
	CGuiEditor* m_pAssetsEditor;
	int m_Member;
	
	virtual float GetValue() const
	{
		return 180.0f*m_pAssetsEditor->AssetsManager()->GetAssetValue<float>(
			m_pAssetsEditor->GetEditedAssetPath(),
			m_pAssetsEditor->GetEditedSubPath(),
			m_Member,
			0
		)/Pi;
	}
	
	virtual void SetValue(float Value)
	{
		m_pAssetsEditor->AssetsManager()->SetAssetValue<float>(
			m_pAssetsEditor->GetEditedAssetPath(),
			m_pAssetsEditor->GetEditedSubPath(),
			m_Member,
			Value * Pi/180.0f
		);
	}
	
public:
	CMemberAngleEdit(CGuiEditor* pAssetsEditor, int Member) :
		gui::CAbstractFloatEdit(pAssetsEditor),
		m_pAssetsEditor(pAssetsEditor),
		m_Member(Member)
	{ }
};

void CAssetsInspector::AddField_Angle(gui::CVListLayout* pList, int Member, const CLocalizableString& Text)
{
	CMemberAngleEdit* pWidget = new CMemberAngleEdit(
		m_pAssetsEditor,
		Member
	);
	
	AddField(pList, pWidget, Text);
}

/* COLOR EDIT *********************************************************/
	
class CMemberColorEdit : public gui::CAbstractColorEdit
{
protected:
	CGuiEditor* m_pAssetsEditor;
	int m_Member;
	
	virtual vec4 GetValue() const
	{
		return m_pAssetsEditor->AssetsManager()->GetAssetValue<vec4>(
			m_pAssetsEditor->GetEditedAssetPath(),
			m_pAssetsEditor->GetEditedSubPath(),
			m_Member,
			vec4(1.0f, 1.0f, 1.0f, 1.0f)
		);
	}
	
	virtual void SetValue(vec4 Value)
	{
		m_pAssetsEditor->AssetsManager()->SetAssetValue<vec4>(
			m_pAssetsEditor->GetEditedAssetPath(),
			m_pAssetsEditor->GetEditedSubPath(),
			m_Member,
			Value
		);
	}
	
public:
	CMemberColorEdit(CGuiEditor* pAssetsEditor, int Member) :
		gui::CAbstractColorEdit(pAssetsEditor),
		m_pAssetsEditor(pAssetsEditor),
		m_Member(Member)
	{ }
};

void CAssetsInspector::AddField_Color(gui::CVListLayout* pList, int Member, const CLocalizableString& Text)
{
	CMemberColorEdit* pWidget = new CMemberColorEdit(
		m_pAssetsEditor,
		Member
	);
	
	AddField(pList, pWidget, Text);
}

/* ASSET EDIT *********************************************************/

class CMemberAssetEdit : public gui::CButton
{
public:
	class CPopup : public gui::CPopup
	{
	public:
		class CItem : public gui::CButton
		{
		protected:
			CAssetPath m_AssetPath;
			CPopup* m_pPopup;
		
		protected:
			virtual void MouseClickAction()
			{
				m_pPopup->SetValue(m_AssetPath);
			}
			
		public:
			CItem(CPopup* pPopup, CAssetPath AssetPath, bool CheckTileCompatibility) :
				gui::CButton(pPopup->Context(), ""),
				m_AssetPath(AssetPath),
				m_pPopup(pPopup)
			{
				if(m_AssetPath.IsNull())
				{
					SetIcon(m_pPopup->m_pAssetsEditor->m_Path_Sprite_IconNone);
					SetText(_LSTRING("None"));
				}
				else
				{
					SetIcon(m_pPopup->m_pAssetsEditor->GetItemIcon(m_AssetPath, CSubPath::Null()));
					SetText(m_pPopup->m_pAssetsEditor->GetItemName(m_AssetPath, CSubPath::Null()));
				}
				
				if(CheckTileCompatibility)
				{
					const CAsset_Image* pImage = AssetsManager()->GetAsset<CAsset_Image>(m_AssetPath);
					if(!pImage || !pImage->GetTilingEnabled())
						Editable(false);
				}
			}
			
			virtual void Update(bool ParentEnabled)
			{
				if(ParentEnabled)
				{
					if(m_pPopup->GetValue() == m_AssetPath)
						SetButtonStyle(m_pPopup->m_pAssetsEditor->m_Path_Button_ListItemHL);
					else
						SetButtonStyle(m_pPopup->m_pAssetsEditor->m_Path_Button_ListItem);
				}
				
				gui::CButton::Update(ParentEnabled);
			}
		};
	
	protected:
		CGuiEditor* m_pAssetsEditor;
		int m_Member;
		int m_AssetType;
		bool m_CheckTileCompatibility;
	
	public:
		CPopup(CGuiEditor* pAssetsEditor, int Member, int AssetType, gui::CRect ParentRect, int CheckTileCompatibility) :
			gui::CPopup(pAssetsEditor, ParentRect, 250, 400, gui::CPopup::ALIGNMENT_SIDE),
			m_pAssetsEditor(pAssetsEditor),
			m_Member(Member),
			m_AssetType(AssetType),
			m_CheckTileCompatibility(CheckTileCompatibility)
		{
			
			gui::CVScrollLayout* pLayout = new gui::CVScrollLayout(Context());
			Add(pLayout);
			
			SetBoxStyle(m_pAssetsEditor->m_Path_Box_Dialog);
			pLayout->Add(new CItem(this, CAssetPath::Null(), false), false);
			
			#define MACRO_ASSETTYPE(Name) case CAsset_##Name::TypeId:\
				{\
					gui::CExpand* pExpand = new gui::CExpand(Context());\
					pLayout->Add(pExpand);\
					pExpand->SetTitle(new gui::CLabel(Context(), AssetsManager()->GetPackageName(m_pAssetsEditor->GetEditedPackageId()), m_pAssetsEditor->m_Path_Sprite_IconFolder));\
					for(int i=0; i<AssetsManager()->GetNumAssets<CAsset_##Name>(m_pAssetsEditor->GetEditedPackageId()); i++)\
						pExpand->Add(new CItem(this, CAssetPath(CAsset_##Name::TypeId, m_pAssetsEditor->GetEditedPackageId(), i), m_CheckTileCompatibility));\
				}\
				for(int p=0; p<AssetsManager()->GetNumPackages(); p++)\
				{\
					if(p != m_pAssetsEditor->GetEditedPackageId() && str_comp(AssetsManager()->GetPackageName(p), "gui_editor") != 0)\
					{\
						gui::CExpand* pExpand = new gui::CExpand(Context());\
						pLayout->Add(pExpand);\
						pExpand->SetTitle(new gui::CLabel(Context(), AssetsManager()->GetPackageName(p), m_pAssetsEditor->m_Path_Sprite_IconFolder));\
						for(int i=0; i<AssetsManager()->GetNumAssets<CAsset_##Name>(p); i++)\
							pExpand->Add(new CItem(this, CAssetPath(CAsset_##Name::TypeId, p, i), m_CheckTileCompatibility));\
					}\
				}\
				break;
			
			switch(m_AssetType)
			{
				#include <generated/assets/assetsmacro.h>
			}
			
			#undef TU_MACRO_ASSETTYPE
		}
		
		CAssetPath GetValue()
		{
			return m_pAssetsEditor->AssetsManager()->GetAssetValue<CAssetPath>(
				m_pAssetsEditor->GetEditedAssetPath(),
				m_pAssetsEditor->GetEditedSubPath(),
				m_Member,
				CAssetPath::Null()
			);
		}
		
		void SetValue(CAssetPath Value)
		{
			m_pAssetsEditor->AssetsManager()->SetAssetValue<CAssetPath>(
				m_pAssetsEditor->GetEditedAssetPath(),
				m_pAssetsEditor->GetEditedSubPath(),
				m_Member,
				Value
			);
		}
	
		virtual int GetInputToBlock() { return CGui::BLOCKEDINPUT_ALL; }
	};

protected:
	CGuiEditor* m_pAssetsEditor;
	int m_Member;
	int m_AssetType;
	bool m_CheckTileCompatibility;
	
protected:
	virtual void MouseClickAction()
	{
		Context()->DisplayPopup(new CPopup(m_pAssetsEditor, m_Member, m_AssetType, m_DrawRect, m_CheckTileCompatibility));
	}

public:
	CMemberAssetEdit(CGuiEditor* pAssetsEditor, int Member, int AssetType) :
		gui::CButton(pAssetsEditor, "", CAssetPath::Null()),
		m_pAssetsEditor(pAssetsEditor),
		m_Member(Member),
		m_AssetType(AssetType),
		m_CheckTileCompatibility(false)
	{
		
	}
	
	virtual void Update(bool ParentEnabled)
	{
		if(IsEnabled() && ParentEnabled)
		{
			if(!AssetsManager()->IsValidPackage(m_pAssetsEditor->GetEditedPackageId()) || AssetsManager()->IsReadOnlyPackage(m_pAssetsEditor->GetEditedPackageId()))
				Editable(false);
			else
				Editable(true);
				
			CAssetPath Value = m_pAssetsEditor->AssetsManager()->GetAssetValue<CAssetPath>(
				m_pAssetsEditor->GetEditedAssetPath(),
				m_pAssetsEditor->GetEditedSubPath(),
				m_Member,
				CAssetPath::Null()
			);
			
			if(Value.IsNull())
			{
				SetIcon(m_pAssetsEditor->m_Path_Sprite_IconNone);
				SetText(_LSTRING("None"));
			}
			else
			{
				SetIcon(m_pAssetsEditor->GetItemIcon(Value, CSubPath::Null()));
				SetText(m_pAssetsEditor->GetItemName(Value, CSubPath::Null()));
			}
		}
		
		gui::CButton::Update(ParentEnabled);
	}
	
	void CheckTileCompatibility()
	{
		m_CheckTileCompatibility = true;
	}
};

void CAssetsInspector::AddField_Asset(gui::CVListLayout* pList, int Member, int AssetType, const CLocalizableString& Text)
{
	CMemberAssetEdit* pWidget = new CMemberAssetEdit(
		m_pAssetsEditor,
		Member,
		AssetType
	);
	
	AddField(pList, pWidget, Text);
}

void CAssetsInspector::AddField_ImageTiles(gui::CVListLayout* pList, int Member, int AssetType, const CLocalizableString& Text)
{
	CMemberAssetEdit* pWidget = new CMemberAssetEdit(
		m_pAssetsEditor,
		Member,
		AssetType
	);
	pWidget->CheckTileCompatibility();
	
	AddField(pList, pWidget, Text);
}

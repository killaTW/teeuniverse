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

#include <shared/system/debug.h>
#include <shared/system/string.h>
#include <shared/kernel.h>
#include <shared/components/assetsmanager.h>
#include <shared/components/storage.h>
#include <client/loading_tools.h>

#include <cstdlib>

#define CREATE_SPRITE_PATH(path, packageid, name, image, x, y, w, h) {\
	CAsset_Sprite* pSprite = pKernel->AssetsManager()->NewAsset_Hard<CAsset_Sprite>(&path, packageid);\
	pSprite->SetName(name);\
	pSprite->SetImagePath(image);\
	pSprite->SetX(x);\
	pSprite->SetY(y);\
	pSprite->SetWidth(w);\
	pSprite->SetHeight(h);\
}

int main(int argc, char* argv[])
{
	CSharedKernel* pKernel = new CSharedKernel();
	if(!pKernel->Init(argc, (const char**) argv))
	{
		dbg_msg("Kernel", "unable to initialize shared kernel");
		exit(EXIT_FAILURE);
	}
	
	int PackageId = -1;
	
	/* UNIV TEEWORLDS */
	PackageId = pKernel->AssetsManager()->NewPackage("univ_teeworlds");
	pKernel->AssetsManager()->SetPackageAuthor(PackageId, "necropotame");
	pKernel->AssetsManager()->SetPackageCredits(PackageId, "TeeWorlds");
	pKernel->AssetsManager()->SetPackageLicense(PackageId, "CC-BY-SA 3.0");
	pKernel->AssetsManager()->SetPackageVersion(PackageId, "0.0.1");
	
	CAssetPath ImageEntitiesPath = CreateNewImage(pKernel, PackageId, "entities", "datasrc/images/univ_teeworlds/entities.png", CStorage::TYPE_ABSOLUTE, 4, 4);
	pKernel->AssetsManager()->SetAssetValue_Hard<int>(ImageEntitiesPath, CSubPath::Null(), CAsset_Image::TEXELSIZE, 768);
	
	//Physics
	{
		CAssetPath AssetPath;
		CSubPath SubPath;
		
		CAsset_ZoneType* pAsset = pKernel->AssetsManager()->NewAsset_Hard<CAsset_ZoneType>(&AssetPath, PackageId);
		pAsset->SetName("physics");
		
		SubPath = CAsset_ZoneType::SubPath_Index(pAsset->AddIndex());
		pAsset->SetIndexDescription(SubPath, "Air");
		pAsset->SetIndexColor(SubPath, vec4(1.0f, 1.0f, 1.0f, 0.0f));
		
		SubPath = CAsset_ZoneType::SubPath_Index(pAsset->AddIndex());
		pAsset->SetIndexDescription(SubPath, "Hookable Ground");
		pAsset->SetIndexColor(SubPath, vec4(1.0f, 1.0f, 1.0f, 1.0f));
		
		SubPath = CAsset_ZoneType::SubPath_Index(pAsset->AddIndex());
		pAsset->SetIndexDescription(SubPath, "Death");
		pAsset->SetIndexColor(SubPath, vec4(1.0f, 0.5f, 0.5f, 1.0f));
		
		SubPath = CAsset_ZoneType::SubPath_Index(pAsset->AddIndex());
		pAsset->SetIndexDescription(SubPath, "Unhookable Ground");
		pAsset->SetIndexColor(SubPath, vec4(228.0f/255.0f, 255.0f/255.0f, 0.0f/255.0f, 1.0f));
	}
	
	//EntityType, Spawn
	{
		CAssetPath GizmoPath;
		CAssetPath AssetPath;
		
		CREATE_SPRITE_PATH(GizmoPath, PackageId, "gizmoSpawn", ImageEntitiesPath, 0, 3, 1, 1);
		
		CAsset_EntityType* pAsset = pKernel->AssetsManager()->NewAsset_Hard<CAsset_EntityType>(&AssetPath, PackageId);
		pAsset->SetName("spawn");
		pAsset->SetCollisionRadius(64.0f);
		pAsset->SetGizmoPath(GizmoPath);
	}
	//EntityType, Blue Spawn
	{
		CAssetPath GizmoPath;
		CAssetPath AssetPath;
		
		CREATE_SPRITE_PATH(GizmoPath, PackageId, "gizmoBlueSpawn", ImageEntitiesPath, 1, 3, 1, 1);
		
		CAsset_EntityType* pAsset = pKernel->AssetsManager()->NewAsset_Hard<CAsset_EntityType>(&AssetPath, PackageId);
		pAsset->SetName("blueSpawn");
		pAsset->SetCollisionRadius(64.0f);
		pAsset->SetGizmoPath(GizmoPath);
	}
	//EntityType, Red Spawn
	{
		CAssetPath GizmoPath;
		CAssetPath AssetPath;
		
		CREATE_SPRITE_PATH(GizmoPath, PackageId, "gizmoRedSpawn", ImageEntitiesPath, 2, 3, 1, 1);
		
		CAsset_EntityType* pAsset = pKernel->AssetsManager()->NewAsset_Hard<CAsset_EntityType>(&AssetPath, PackageId);
		pAsset->SetName("redSpawn");
		pAsset->SetCollisionRadius(64.0f);
		pAsset->SetGizmoPath(GizmoPath);
	}
	//EntityType, Shotgun
	{
		CAssetPath GizmoPath;
		CAssetPath AssetPath;
		
		CREATE_SPRITE_PATH(GizmoPath, PackageId, "gizmoShotgun", ImageEntitiesPath, 0, 2, 1, 1);
		
		CAsset_EntityType* pAsset = pKernel->AssetsManager()->NewAsset_Hard<CAsset_EntityType>(&AssetPath, PackageId);
		pAsset->SetName("shotgun");
		pAsset->SetCollisionRadius(20.0f);
		pAsset->SetGizmoPath(GizmoPath);
	}
	//EntityType, Grenade
	{
		CAssetPath GizmoPath;
		CAssetPath AssetPath;
		
		CREATE_SPRITE_PATH(GizmoPath, PackageId, "gizmoGrenade", ImageEntitiesPath, 1, 2, 1, 1);
		
		CAsset_EntityType* pAsset = pKernel->AssetsManager()->NewAsset_Hard<CAsset_EntityType>(&AssetPath, PackageId);
		pAsset->SetName("grenade");
		pAsset->SetCollisionRadius(20.0f);
		pAsset->SetGizmoPath(GizmoPath);
	}
	//EntityType, Rifle
	{
		CAssetPath GizmoPath;
		CAssetPath AssetPath;
		
		CREATE_SPRITE_PATH(GizmoPath, PackageId, "gizmoRifle", ImageEntitiesPath, 2, 2, 1, 1);
		
		CAsset_EntityType* pAsset = pKernel->AssetsManager()->NewAsset_Hard<CAsset_EntityType>(&AssetPath, PackageId);
		pAsset->SetName("laserRifle");
		pAsset->SetCollisionRadius(20.0f);
		pAsset->SetGizmoPath(GizmoPath);
	}
	//EntityType, Gun
	{
		CAssetPath GizmoPath;
		CAssetPath AssetPath;
		
		CREATE_SPRITE_PATH(GizmoPath, PackageId, "gizmoGun", ImageEntitiesPath, 0, 1, 1, 1);
		
		CAsset_EntityType* pAsset = pKernel->AssetsManager()->NewAsset_Hard<CAsset_EntityType>(&AssetPath, PackageId);
		pAsset->SetName("gun");
		pAsset->SetCollisionRadius(20.0f);
		pAsset->SetGizmoPath(GizmoPath);
	}
	//EntityType, Hammer
	{
		CAssetPath GizmoPath;
		CAssetPath AssetPath;
		
		CREATE_SPRITE_PATH(GizmoPath, PackageId, "gizmoHammer", ImageEntitiesPath, 1, 1, 1, 1);
		
		CAsset_EntityType* pAsset = pKernel->AssetsManager()->NewAsset_Hard<CAsset_EntityType>(&AssetPath, PackageId);
		pAsset->SetName("hammer");
		pAsset->SetCollisionRadius(20.0f);
		pAsset->SetGizmoPath(GizmoPath);
	}
	//EntityType, Ninja
	{
		CAssetPath GizmoPath;
		CAssetPath AssetPath;
		
		CREATE_SPRITE_PATH(GizmoPath, PackageId, "gizmoNinja", ImageEntitiesPath, 2, 1, 1, 1);
		
		CAsset_EntityType* pAsset = pKernel->AssetsManager()->NewAsset_Hard<CAsset_EntityType>(&AssetPath, PackageId);
		pAsset->SetName("ninja");
		pAsset->SetCollisionRadius(20.0f);
		pAsset->SetGizmoPath(GizmoPath);
	}
	//EntityType, Red Flag
	{
		CAssetPath GizmoPath;
		CAssetPath AssetPath;
		
		CREATE_SPRITE_PATH(GizmoPath, PackageId, "gizmoRedFlag", ImageEntitiesPath, 3, 0, 1, 2);
		
		CAsset_EntityType* pAsset = pKernel->AssetsManager()->NewAsset_Hard<CAsset_EntityType>(&AssetPath, PackageId);
		pAsset->SetName("redFlag");
		pAsset->SetCollisionRadius(42.0f);
		pAsset->SetGizmoPath(GizmoPath);
	}
	//EntityType, Blue Flag
	{
		CAssetPath GizmoPath;
		CAssetPath AssetPath;
		
		CREATE_SPRITE_PATH(GizmoPath, PackageId, "gizmoBlueFlag", ImageEntitiesPath, 3, 2, 1, 2);
		
		CAsset_EntityType* pAsset = pKernel->AssetsManager()->NewAsset_Hard<CAsset_EntityType>(&AssetPath, PackageId);
		pAsset->SetName("blueFlag");
		pAsset->SetCollisionRadius(42.0f);
		pAsset->SetGizmoPath(GizmoPath);
	}
	//EntityType, Heart
	{
		CAssetPath GizmoPath;
		CAssetPath AssetPath;
		
		CREATE_SPRITE_PATH(GizmoPath, PackageId, "gizmoHeart", ImageEntitiesPath, 0, 0, 1, 1);
		
		CAsset_EntityType* pAsset = pKernel->AssetsManager()->NewAsset_Hard<CAsset_EntityType>(&AssetPath, PackageId);
		pAsset->SetName("heart");
		pAsset->SetCollisionRadius(20.0f);
		pAsset->SetGizmoPath(GizmoPath);
	}
	//EntityType, Armor
	{
		CAssetPath GizmoPath;
		CAssetPath AssetPath;
		
		CREATE_SPRITE_PATH(GizmoPath, PackageId, "gizmoArmor", ImageEntitiesPath, 1, 0, 1, 1);
		
		CAsset_EntityType* pAsset = pKernel->AssetsManager()->NewAsset_Hard<CAsset_EntityType>(&AssetPath, PackageId);
		pAsset->SetName("armor");
		pAsset->SetCollisionRadius(20.0f);
		pAsset->SetGizmoPath(GizmoPath);
	}
	
	pKernel->AssetsManager()->Save_AssetsFile(PackageId);
	
	/* ENV CLOUDS */
	PackageId = pKernel->AssetsManager()->NewPackage("env_clouds");
	pKernel->AssetsManager()->SetPackageAuthor(PackageId, "necropotame");
	pKernel->AssetsManager()->SetPackageCredits(PackageId, "TeeWorlds");
	pKernel->AssetsManager()->SetPackageLicense(PackageId, "CC-BY-SA 3.0");
	pKernel->AssetsManager()->SetPackageVersion(PackageId, "0.0.1");
	CreateNewImage(pKernel, PackageId, "cloud1", "datasrc/images/env_clouds/bg_cloud1.png", CStorage::TYPE_ABSOLUTE, 1, 1);
	CreateNewImage(pKernel, PackageId, "cloud2", "datasrc/images/env_clouds/bg_cloud2.png", CStorage::TYPE_ABSOLUTE, 1, 1);
	CreateNewImage(pKernel, PackageId, "cloud3", "datasrc/images/env_clouds/bg_cloud3.png", CStorage::TYPE_ABSOLUTE, 1, 1);
	pKernel->AssetsManager()->Save_AssetsFile(PackageId);
	
	/* ENV DESERT */
	{
		PackageId = pKernel->AssetsManager()->NewPackage("env_desert");
		pKernel->AssetsManager()->SetPackageAuthor(PackageId, "necropotame");
		pKernel->AssetsManager()->SetPackageCredits(PackageId, "TeeWorlds");
		pKernel->AssetsManager()->SetPackageLicense(PackageId, "CC-BY-SA 3.0");
		pKernel->AssetsManager()->SetPackageVersion(PackageId, "0.0.1");
		
		CAssetPath MainImagePath = CreateNewImage(pKernel, PackageId, "desertMain", "datasrc/images/env_desert/desert_main.png", CStorage::TYPE_ABSOLUTE, 16, 16, true, 1);
		CreateNewImage(pKernel, PackageId, "desertDoodads", "datasrc/images/env_desert/desert_doodads.png", CStorage::TYPE_ABSOLUTE, 16, 16, true, 1);
		CreateNewImage(pKernel, PackageId, "desertMountains1", "datasrc/images/env_desert/desert_mountains.png", CStorage::TYPE_ABSOLUTE, 1, 1);
		CreateNewImage(pKernel, PackageId, "desertMountains2", "datasrc/images/env_desert/desert_mountains2.png", CStorage::TYPE_ABSOLUTE, 1, 1);
		CreateNewImage(pKernel, PackageId, "desertSun", "datasrc/images/env_desert/desert_sun.png", CStorage::TYPE_ABSOLUTE, 1, 1);
		
		CAssetPath RedRockTopPath;
		CAssetPath YellowRockTopPath;
		CAssetPath BrownRockTopPath;
		CAssetPath UndergroundCablePath;
		CAssetPath UndergroundCable2Path;
		
		CREATE_SPRITE_PATH(RedRockTopPath, PackageId, "redRockTop", MainImagePath, 0, 1, 1, 1);
		CREATE_SPRITE_PATH(YellowRockTopPath, PackageId, "yellowRockTop", MainImagePath, 5, 0, 1, 1);
		CREATE_SPRITE_PATH(BrownRockTopPath, PackageId, "brownRockTop", MainImagePath, 0, 6, 1, 1);
		CREATE_SPRITE_PATH(UndergroundCablePath, PackageId, "undergroundCable", MainImagePath, 7, 7, 1, 1);
		CREATE_SPRITE_PATH(UndergroundCable2Path, PackageId, "undergroundCable2", MainImagePath, 8, 5, 1, 1);
		
		//Material: RedRock
		{
			CAssetPath MaterialPath;
			CAsset_Material* pAsset = pKernel->AssetsManager()->NewAsset_Hard<CAsset_Material>(&MaterialPath, PackageId);
			pAsset->SetName("redRock");
			pAsset->SetTextureEnabled(true);
			pAsset->SetTextureColor(vec4(188.0f/255.0f, 95.0f/255.0f, 53.0f/255.0f, 1.0f));
			pAsset->SetTextureSpacing(-16.0f);
			
			CSubPath LayerPath = CAsset_Material::SubPath_Layer(pKernel->AssetsManager()->AddSubItem_Hard(MaterialPath, CSubPath::Null(), CAsset_Material::TYPE_LAYER));
			pKernel->AssetsManager()->SetAssetValue_Hard<int>(MaterialPath, LayerPath, CAsset_Material::LAYER_REPEATTYPE, CAsset_Material::REPEATTYPE_STRETCH);
			
			CSubPath SpritePath = CAsset_Material::SubPath_LayerSprite(LayerPath.GetId(), pKernel->AssetsManager()->AddSubItem_Hard(MaterialPath, LayerPath, CAsset_Material::TYPE_LAYER_SPRITE));
			pKernel->AssetsManager()->SetAssetValue_Hard<CAssetPath>(MaterialPath, SpritePath, CAsset_Material::LAYER_SPRITE_PATH, RedRockTopPath);
			pKernel->AssetsManager()->SetAssetValue_Hard<vec2>(MaterialPath, SpritePath, CAsset_Material::LAYER_SPRITE_SIZE, 0.5f);
			pKernel->AssetsManager()->SetAssetValue_Hard<float>(MaterialPath, SpritePath, CAsset_Material::LAYER_SPRITE_POSITION_Y, -16.0f);
		}
		//Material: YellowRock
		{
			CAssetPath MaterialPath;
			CAsset_Material* pAsset = pKernel->AssetsManager()->NewAsset_Hard<CAsset_Material>(&MaterialPath, PackageId);
			pAsset->SetName("yellowRock");
			pAsset->SetTextureEnabled(true);
			pAsset->SetTextureColor(vec4(244.0f/255.0f, 209.0f/255.0f, 126.0f/255.0f, 1.0f));
			pAsset->SetTextureSpacing(-16.0f);
			
			CSubPath LayerPath = CAsset_Material::SubPath_Layer(pKernel->AssetsManager()->AddSubItem_Hard(MaterialPath, CSubPath::Null(), CAsset_Material::TYPE_LAYER));
			pKernel->AssetsManager()->SetAssetValue_Hard<int>(MaterialPath, LayerPath, CAsset_Material::LAYER_REPEATTYPE, CAsset_Material::REPEATTYPE_STRETCH);
			
			CSubPath SpritePath = CAsset_Material::SubPath_LayerSprite(LayerPath.GetId(), pKernel->AssetsManager()->AddSubItem_Hard(MaterialPath, LayerPath, CAsset_Material::TYPE_LAYER_SPRITE));
			pKernel->AssetsManager()->SetAssetValue_Hard<CAssetPath>(MaterialPath, SpritePath, CAsset_Material::LAYER_SPRITE_PATH, YellowRockTopPath);
			pKernel->AssetsManager()->SetAssetValue_Hard<vec2>(MaterialPath, SpritePath, CAsset_Material::LAYER_SPRITE_SIZE, 0.5f);
			pKernel->AssetsManager()->SetAssetValue_Hard<float>(MaterialPath, SpritePath, CAsset_Material::LAYER_SPRITE_POSITION_Y, -16.0f);
		}
		//Material: BrownRock
		{
			CAssetPath MaterialPath;
			CAsset_Material* pAsset = pKernel->AssetsManager()->NewAsset_Hard<CAsset_Material>(&MaterialPath, PackageId);
			pAsset->SetName("brownRock");
			pAsset->SetTextureEnabled(true);
			pAsset->SetTextureColor(vec4(129.0f/255.0f, 90.0f/255.0f, 71.0f/255.0f, 1.0f));
			pAsset->SetTextureSpacing(-16.0f);
			
			CSubPath LayerPath = CAsset_Material::SubPath_Layer(pKernel->AssetsManager()->AddSubItem_Hard(MaterialPath, CSubPath::Null(), CAsset_Material::TYPE_LAYER));
			pKernel->AssetsManager()->SetAssetValue_Hard<int>(MaterialPath, LayerPath, CAsset_Material::LAYER_REPEATTYPE, CAsset_Material::REPEATTYPE_STRETCH);
			
			CSubPath SpritePath = CAsset_Material::SubPath_LayerSprite(LayerPath.GetId(), pKernel->AssetsManager()->AddSubItem_Hard(MaterialPath, LayerPath, CAsset_Material::TYPE_LAYER_SPRITE));
			pKernel->AssetsManager()->SetAssetValue_Hard<CAssetPath>(MaterialPath, SpritePath, CAsset_Material::LAYER_SPRITE_PATH, BrownRockTopPath);
			pKernel->AssetsManager()->SetAssetValue_Hard<vec2>(MaterialPath, SpritePath, CAsset_Material::LAYER_SPRITE_SIZE, 0.5f);
			pKernel->AssetsManager()->SetAssetValue_Hard<float>(MaterialPath, SpritePath, CAsset_Material::LAYER_SPRITE_POSITION_Y, -16.0f);
		}
		//Material: Underground cable
		{
			CAssetPath MaterialPath;
			CAsset_Material* pAsset = pKernel->AssetsManager()->NewAsset_Hard<CAsset_Material>(&MaterialPath, PackageId);
			pAsset->SetName("undergroundCable");
			
			CSubPath LayerPath = CAsset_Material::SubPath_Layer(pKernel->AssetsManager()->AddSubItem_Hard(MaterialPath, CSubPath::Null(), CAsset_Material::TYPE_LAYER));
			pKernel->AssetsManager()->SetAssetValue_Hard<int>(MaterialPath, LayerPath, CAsset_Material::LAYER_REPEATTYPE, CAsset_Material::REPEATTYPE_STRETCH);
			
			CSubPath SpritePath;
			
			for(int i=0; i<2; i++)
			{
				SpritePath = CAsset_Material::SubPath_LayerSprite(LayerPath.GetId(), pKernel->AssetsManager()->AddSubItem_Hard(MaterialPath, LayerPath, CAsset_Material::TYPE_LAYER_SPRITE));
				pKernel->AssetsManager()->SetAssetValue_Hard<CAssetPath>(MaterialPath, SpritePath, CAsset_Material::LAYER_SPRITE_PATH, UndergroundCablePath);
				pKernel->AssetsManager()->SetAssetValue_Hard<vec2>(MaterialPath, SpritePath, CAsset_Material::LAYER_SPRITE_SIZE, 0.5f);
				pKernel->AssetsManager()->SetAssetValue_Hard<int>(MaterialPath, SpritePath, CAsset_Material::LAYER_SPRITE_FLAGS, CAsset_Material::SPRITEFLAG_ROTATION);
			}
			
			SpritePath = CAsset_Material::SubPath_LayerSprite(LayerPath.GetId(), pKernel->AssetsManager()->AddSubItem_Hard(MaterialPath, LayerPath, CAsset_Material::TYPE_LAYER_SPRITE));
			pKernel->AssetsManager()->SetAssetValue_Hard<CAssetPath>(MaterialPath, SpritePath, CAsset_Material::LAYER_SPRITE_PATH, UndergroundCable2Path);
			pKernel->AssetsManager()->SetAssetValue_Hard<vec2>(MaterialPath, SpritePath, CAsset_Material::LAYER_SPRITE_SIZE, 0.5f);
			
			for(int i=0; i<2; i++)
			{
				SpritePath = CAsset_Material::SubPath_LayerSprite(LayerPath.GetId(), pKernel->AssetsManager()->AddSubItem_Hard(MaterialPath, LayerPath, CAsset_Material::TYPE_LAYER_SPRITE));
				pKernel->AssetsManager()->SetAssetValue_Hard<CAssetPath>(MaterialPath, SpritePath, CAsset_Material::LAYER_SPRITE_PATH, UndergroundCablePath);
				pKernel->AssetsManager()->SetAssetValue_Hard<vec2>(MaterialPath, SpritePath, CAsset_Material::LAYER_SPRITE_SIZE, 0.5f);
				pKernel->AssetsManager()->SetAssetValue_Hard<int>(MaterialPath, SpritePath, CAsset_Material::LAYER_SPRITE_FLAGS, CAsset_Material::SPRITEFLAG_ROTATION);
			}
		}
		
		pKernel->AssetsManager()->Save_AssetsFile(PackageId);
	}
	
	/* ENV GENERIC */
	PackageId = pKernel->AssetsManager()->NewPackage("env_generic");
	pKernel->AssetsManager()->SetPackageAuthor(PackageId, "necropotame");
	pKernel->AssetsManager()->SetPackageCredits(PackageId, "TeeWorlds");
	pKernel->AssetsManager()->SetPackageLicense(PackageId, "CC-BY-SA 3.0");
	pKernel->AssetsManager()->SetPackageVersion(PackageId, "0.0.1");
	CreateNewImage(pKernel, PackageId, "genericSpikes", "datasrc/images/env_generic/generic_deathtiles.png", CStorage::TYPE_ABSOLUTE, 16, 16, true, 1);
	CreateNewImage(pKernel, PackageId, "genericUnhookable", "datasrc/images/env_generic/generic_unhookable.png", CStorage::TYPE_ABSOLUTE, 16, 16, true, 1);
	pKernel->AssetsManager()->Save_AssetsFile(PackageId);
	
	/* ENV GRASS */
	{
		PackageId = pKernel->AssetsManager()->NewPackage("env_grass");
		pKernel->AssetsManager()->SetPackageAuthor(PackageId, "necropotame");
		pKernel->AssetsManager()->SetPackageCredits(PackageId, "TeeWorlds");
		pKernel->AssetsManager()->SetPackageLicense(PackageId, "CC-BY-SA 3.0");
		pKernel->AssetsManager()->SetPackageVersion(PackageId, "0.0.1");
		CAssetPath MainImagePath = CreateNewImage(pKernel, PackageId, "grassMain", "datasrc/images/env_grass/grass_main.png", CStorage::TYPE_ABSOLUTE, 16, 16, true, 1);
		CreateNewImage(pKernel, PackageId, "grassDoodads", "datasrc/images/env_grass/grass_doodads.png", CStorage::TYPE_ABSOLUTE, 16, 16, true, 1);
		
		CAssetPath GrassPath;
		CAssetPath DirtPath;
		CAssetPath DarkDirtPath;
		CAssetPath DirtTransitionPath;
		
		CREATE_SPRITE_PATH(GrassPath, PackageId, "grass", MainImagePath, 0, 1, 1, 1);
		CREATE_SPRITE_PATH(DirtPath, PackageId, "dirt", MainImagePath, 4, 1, 1, 1);
		CREATE_SPRITE_PATH(DarkDirtPath, PackageId, "darkDirt", MainImagePath, 8, 1, 1, 1);
		CREATE_SPRITE_PATH(DirtTransitionPath, PackageId, "dirtTransition", MainImagePath, 8, 5, 1, 1);
		
		//Material: Grass
		{
			CAssetPath MaterialPath;
			CAsset_Material* pAsset = pKernel->AssetsManager()->NewAsset_Hard<CAsset_Material>(&MaterialPath, PackageId);
			pAsset->SetName("grass");
			pAsset->SetTextureEnabled(true);
			pAsset->SetTextureColor(vec4(161.0f/255.0f, 110.0f/255.0f, 54.0f/255.0f, 1.0f));
			pAsset->SetTextureSpacing(-16.0f);
			
			CSubPath LayerPath = CAsset_Material::SubPath_Layer(pKernel->AssetsManager()->AddSubItem_Hard(MaterialPath, CSubPath::Null(), CAsset_Material::TYPE_LAYER));
			pKernel->AssetsManager()->SetAssetValue_Hard<int>(MaterialPath, LayerPath, CAsset_Material::LAYER_REPEATTYPE, CAsset_Material::REPEATTYPE_STRETCH);
			
			CSubPath SpritePath = CAsset_Material::SubPath_LayerSprite(LayerPath.GetId(), pKernel->AssetsManager()->AddSubItem_Hard(MaterialPath, LayerPath, CAsset_Material::TYPE_LAYER_SPRITE));
			pKernel->AssetsManager()->SetAssetValue_Hard<CAssetPath>(MaterialPath, SpritePath, CAsset_Material::LAYER_SPRITE_PATH, GrassPath);
			pKernel->AssetsManager()->SetAssetValue_Hard<vec2>(MaterialPath, SpritePath, CAsset_Material::LAYER_SPRITE_SIZE, 0.5f);
			pKernel->AssetsManager()->SetAssetValue_Hard<float>(MaterialPath, SpritePath, CAsset_Material::LAYER_SPRITE_POSITION_Y, -16.0f);
		}
		
		//Material: Dirt
		{
			CAssetPath MaterialPath;
			CAsset_Material* pAsset = pKernel->AssetsManager()->NewAsset_Hard<CAsset_Material>(&MaterialPath, PackageId);
			pAsset->SetName("dirt");
			pAsset->SetTextureEnabled(true);
			pAsset->SetTextureColor(vec4(161.0f/255.0f, 110.0f/255.0f, 54.0f/255.0f, 1.0f));
			pAsset->SetTextureSpacing(-16.0f);
			
			CSubPath LayerPath = CAsset_Material::SubPath_Layer(pKernel->AssetsManager()->AddSubItem_Hard(MaterialPath, CSubPath::Null(), CAsset_Material::TYPE_LAYER));
			pKernel->AssetsManager()->SetAssetValue_Hard<int>(MaterialPath, LayerPath, CAsset_Material::LAYER_REPEATTYPE, CAsset_Material::REPEATTYPE_STRETCH);
			
			CSubPath SpritePath = CAsset_Material::SubPath_LayerSprite(LayerPath.GetId(), pKernel->AssetsManager()->AddSubItem_Hard(MaterialPath, LayerPath, CAsset_Material::TYPE_LAYER_SPRITE));
			pKernel->AssetsManager()->SetAssetValue_Hard<CAssetPath>(MaterialPath, SpritePath, CAsset_Material::LAYER_SPRITE_PATH, DirtPath);
			pKernel->AssetsManager()->SetAssetValue_Hard<vec2>(MaterialPath, SpritePath, CAsset_Material::LAYER_SPRITE_SIZE, 0.5f);
			pKernel->AssetsManager()->SetAssetValue_Hard<float>(MaterialPath, SpritePath, CAsset_Material::LAYER_SPRITE_POSITION_Y, -16.0f);
			pKernel->AssetsManager()->SetAssetValue_Hard<int>(MaterialPath, SpritePath, CAsset_Material::LAYER_SPRITE_FLAGS, CAsset_Material::SPRITEFLAG_ROTATION);
		}
		
		//Material: DarkDirt
		{
			CAssetPath MaterialPath;
			CAsset_Material* pAsset = pKernel->AssetsManager()->NewAsset_Hard<CAsset_Material>(&MaterialPath, PackageId);
			pAsset->SetName("darkDirt");
			pAsset->SetTextureEnabled(true);
			pAsset->SetTextureColor(vec4(98.0f/255.0f, 73.0f/255.0f, 45.0f/255.0f, 1.0f));
			pAsset->SetTextureSpacing(-16.0f);
			
			CSubPath LayerPath = CAsset_Material::SubPath_Layer(pKernel->AssetsManager()->AddSubItem_Hard(MaterialPath, CSubPath::Null(), CAsset_Material::TYPE_LAYER));
			pKernel->AssetsManager()->SetAssetValue_Hard<int>(MaterialPath, LayerPath, CAsset_Material::LAYER_REPEATTYPE, CAsset_Material::REPEATTYPE_STRETCH);
			
			CSubPath SpritePath = CAsset_Material::SubPath_LayerSprite(LayerPath.GetId(), pKernel->AssetsManager()->AddSubItem_Hard(MaterialPath, LayerPath, CAsset_Material::TYPE_LAYER_SPRITE));
			pKernel->AssetsManager()->SetAssetValue_Hard<CAssetPath>(MaterialPath, SpritePath, CAsset_Material::LAYER_SPRITE_PATH, DarkDirtPath);
			pKernel->AssetsManager()->SetAssetValue_Hard<vec2>(MaterialPath, SpritePath, CAsset_Material::LAYER_SPRITE_SIZE, 0.5f);
			pKernel->AssetsManager()->SetAssetValue_Hard<float>(MaterialPath, SpritePath, CAsset_Material::LAYER_SPRITE_POSITION_Y, -16.0f);
			pKernel->AssetsManager()->SetAssetValue_Hard<int>(MaterialPath, SpritePath, CAsset_Material::LAYER_SPRITE_FLAGS, CAsset_Material::SPRITEFLAG_ROTATION);
		}
		
		//Material: DirtTransition
		{
			CAssetPath MaterialPath;
			CAsset_Material* pAsset = pKernel->AssetsManager()->NewAsset_Hard<CAsset_Material>(&MaterialPath, PackageId);
			pAsset->SetName("dirtTransition");
			pAsset->SetTextureEnabled(true);
			pAsset->SetTextureColor(vec4(84.0f/255.0f, 62.0f/255.0f, 36.0f/255.0f, 1.0f));
			pAsset->SetTextureSpacing(-16.0f);
			
			CSubPath LayerPath = CAsset_Material::SubPath_Layer(pKernel->AssetsManager()->AddSubItem_Hard(MaterialPath, CSubPath::Null(), CAsset_Material::TYPE_LAYER));
			pKernel->AssetsManager()->SetAssetValue_Hard<int>(MaterialPath, LayerPath, CAsset_Material::LAYER_REPEATTYPE, CAsset_Material::REPEATTYPE_STRETCH);
			
			CSubPath SpritePath = CAsset_Material::SubPath_LayerSprite(LayerPath.GetId(), pKernel->AssetsManager()->AddSubItem_Hard(MaterialPath, LayerPath, CAsset_Material::TYPE_LAYER_SPRITE));
			pKernel->AssetsManager()->SetAssetValue_Hard<CAssetPath>(MaterialPath, SpritePath, CAsset_Material::LAYER_SPRITE_PATH, DirtTransitionPath);
			pKernel->AssetsManager()->SetAssetValue_Hard<vec2>(MaterialPath, SpritePath, CAsset_Material::LAYER_SPRITE_SIZE, 0.5f);
			pKernel->AssetsManager()->SetAssetValue_Hard<float>(MaterialPath, SpritePath, CAsset_Material::LAYER_SPRITE_POSITION_Y, -16.0f);
			pKernel->AssetsManager()->SetAssetValue_Hard<int>(MaterialPath, SpritePath, CAsset_Material::LAYER_SPRITE_FLAGS, CAsset_Material::SPRITEFLAG_ROTATION);
		}
		
		pKernel->AssetsManager()->Save_AssetsFile(PackageId);
	}
	
	/* ENV JUNGLE */
	PackageId = pKernel->AssetsManager()->NewPackage("env_jungle");
	pKernel->AssetsManager()->SetPackageAuthor(PackageId, "necropotame");
	pKernel->AssetsManager()->SetPackageCredits(PackageId, "TeeWorlds");
	pKernel->AssetsManager()->SetPackageLicense(PackageId, "CC-BY-SA 3.0");
	pKernel->AssetsManager()->SetPackageVersion(PackageId, "0.0.1");
	CreateNewImage(pKernel, PackageId, "jungleMain", "datasrc/images/env_jungle/jungle_main.png", CStorage::TYPE_ABSOLUTE, 16, 16, true, 1);
	CreateNewImage(pKernel, PackageId, "jungleDoodads", "datasrc/images/env_jungle/jungle_doodads.png", CStorage::TYPE_ABSOLUTE, 16, 16, true, 1);
	CreateNewImage(pKernel, PackageId, "jungleSpikes", "datasrc/images/env_jungle/jungle_deathtiles.png", CStorage::TYPE_ABSOLUTE, 16, 16, true, 1);
	CreateNewImage(pKernel, PackageId, "jungleUnhookable", "datasrc/images/env_jungle/jungle_unhookables.png", CStorage::TYPE_ABSOLUTE, 16, 16, true, 1);
	CreateNewImage(pKernel, PackageId, "jungleBackground", "datasrc/images/env_jungle/jungle_background.png", CStorage::TYPE_ABSOLUTE, 1, 1);
	CreateNewImage(pKernel, PackageId, "jungleMidground", "datasrc/images/env_jungle/jungle_midground.png", CStorage::TYPE_ABSOLUTE, 16, 16, true, 1);
	pKernel->AssetsManager()->Save_AssetsFile(PackageId);
	
	/* ENV MOON */
	PackageId = pKernel->AssetsManager()->NewPackage("env_moon");
	pKernel->AssetsManager()->SetPackageAuthor(PackageId, "necropotame");
	pKernel->AssetsManager()->SetPackageCredits(PackageId, "TeeWorlds");
	pKernel->AssetsManager()->SetPackageLicense(PackageId, "CC-BY-SA 3.0");
	pKernel->AssetsManager()->SetPackageVersion(PackageId, "0.0.1");
	CreateNewImage(pKernel, PackageId, "moon", "datasrc/images/env_moon/moon.png", CStorage::TYPE_ABSOLUTE, 1, 1);
	pKernel->AssetsManager()->Save_AssetsFile(PackageId);
	
	/* ENV MOUNTAINS */
	PackageId = pKernel->AssetsManager()->NewPackage("env_mountains");
	pKernel->AssetsManager()->SetPackageAuthor(PackageId, "necropotame");
	pKernel->AssetsManager()->SetPackageCredits(PackageId, "TeeWorlds");
	pKernel->AssetsManager()->SetPackageLicense(PackageId, "CC-BY-SA 3.0");
	pKernel->AssetsManager()->SetPackageVersion(PackageId, "0.0.1");
	CreateNewImage(pKernel, PackageId, "mountains", "datasrc/images/env_mountains/mountains.png", CStorage::TYPE_ABSOLUTE, 1, 1);
	pKernel->AssetsManager()->Save_AssetsFile(PackageId);
	
	/* ENV SNOW */
	PackageId = pKernel->AssetsManager()->NewPackage("env_snow");
	pKernel->AssetsManager()->SetPackageAuthor(PackageId, "necropotame");
	pKernel->AssetsManager()->SetPackageCredits(PackageId, "TeeWorlds");
	pKernel->AssetsManager()->SetPackageLicense(PackageId, "CC-BY-SA 3.0");
	pKernel->AssetsManager()->SetPackageVersion(PackageId, "0.0.1");
	CreateNewImage(pKernel, PackageId, "snow", "datasrc/images/env_snow/snow.png", CStorage::TYPE_ABSOLUTE, 1, 1);
	pKernel->AssetsManager()->Save_AssetsFile(PackageId);
	
	/* ENV STARS */
	PackageId = pKernel->AssetsManager()->NewPackage("env_stars");
	pKernel->AssetsManager()->SetPackageAuthor(PackageId, "necropotame");
	pKernel->AssetsManager()->SetPackageCredits(PackageId, "TeeWorlds");
	pKernel->AssetsManager()->SetPackageLicense(PackageId, "CC-BY-SA 3.0");
	pKernel->AssetsManager()->SetPackageVersion(PackageId, "0.0.1");
	CreateNewImage(pKernel, PackageId, "stars", "datasrc/images/env_stars/stars.png", CStorage::TYPE_ABSOLUTE, 2, 1);
	pKernel->AssetsManager()->Save_AssetsFile(PackageId);
	
	/* ENV SUN */
	PackageId = pKernel->AssetsManager()->NewPackage("env_sun");
	pKernel->AssetsManager()->SetPackageAuthor(PackageId, "necropotame");
	pKernel->AssetsManager()->SetPackageCredits(PackageId, "TeeWorlds");
	pKernel->AssetsManager()->SetPackageLicense(PackageId, "CC-BY-SA 3.0");
	pKernel->AssetsManager()->SetPackageVersion(PackageId, "0.0.1");
	CreateNewImage(pKernel, PackageId, "sun", "datasrc/images/env_sun/sun.png", CStorage::TYPE_ABSOLUTE, 1, 1);
	pKernel->AssetsManager()->Save_AssetsFile(PackageId);
	
	/* ENV WINTER */
	PackageId = pKernel->AssetsManager()->NewPackage("env_winter");
	pKernel->AssetsManager()->SetPackageAuthor(PackageId, "necropotame");
	pKernel->AssetsManager()->SetPackageCredits(PackageId, "TeeWorlds");
	pKernel->AssetsManager()->SetPackageLicense(PackageId, "CC-BY-SA 3.0");
	pKernel->AssetsManager()->SetPackageVersion(PackageId, "0.0.1");
	CreateNewImage(pKernel, PackageId, "winterMain", "datasrc/images/env_winter/winter_main.png", CStorage::TYPE_ABSOLUTE, 16, 16, true, 1);
	CreateNewImage(pKernel, PackageId, "winterDoodads", "datasrc/images/env_winter/winter_doodads.png", CStorage::TYPE_ABSOLUTE, 16, 16, true, 1);
	CreateNewImage(pKernel, PackageId, "winterMountains1", "datasrc/images/env_winter/winter_mountains.png", CStorage::TYPE_ABSOLUTE, 1, 1);
	CreateNewImage(pKernel, PackageId, "winterMountains2", "datasrc/images/env_winter/winter_mountains2.png", CStorage::TYPE_ABSOLUTE, 1, 1);
	CreateNewImage(pKernel, PackageId, "winterMountains3", "datasrc/images/env_winter/winter_mountains3.png", CStorage::TYPE_ABSOLUTE, 1, 1);
	pKernel->AssetsManager()->Save_AssetsFile(PackageId);
	
	/* SHUTDOWN */
	pKernel->Shutdown();
	delete pKernel;

	exit(EXIT_SUCCESS);
}

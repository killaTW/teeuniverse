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

#ifndef __SHARED_GEOMETRY_LINETESSELATION__
#define __SHARED_GEOMETRY_LINETESSELATION__

#include <shared/math/vector.h>
#include <shared/tl/array.h>
#include <shared/geometry/quad.h>
#include <generated/assets/maplayerobjects.h>

void TesselateBezierCurve(array<CBezierVertex>& BezierVertices, array<CLineVertex>& OutputVertices, float MinWidth);

void GenerateMaterialQuads(const class CAssetsManager* pAssetsManager, array<CTexturedQuad>& OutputQuads, const array<CLineVertex>& Vertices, const matrix2x2& Transform, vec2 ObjPosition, CAssetPath MaterialPath, bool Closed);
void GenerateMaterialCurve_Object(class CAssetsManager* pAssetsManager, float Time, array<CLineVertex>& OutputLines, const CAsset_MapLayerObjects::CObject& Object);
void GenerateMaterialQuads_Object(class CAssetsManager* pAssetsManager, float Time, array<CTexturedQuad>& OutputQuads, const CAsset_MapLayerObjects::CObject& Object);

//If the parameter "Closed" is true, the first and last vertices must be equals
template<typename VERTEX>
void ComputeLineNormals(array<VERTEX>& Vertices, bool Closed)
{
	int NumVertices = Vertices.size();
	for(int i=0; i<NumVertices; i++)
	{
		vec2 Position = Vertices[i].m_Position;
		vec2 DirLeft = 0.0f;
		vec2 DirRight = 0.0f;
		
		if(i>0)
			DirLeft = normalize(Position - Vertices[i-1].m_Position);
		else if(Closed && NumVertices >= 2)
			DirLeft = normalize(Position - Vertices[NumVertices-2].m_Position);
		
		if(i+1<NumVertices)
			DirRight = normalize(Vertices[i+1].m_Position - Position);
		else if(Closed && NumVertices >= 2)
			DirRight = normalize(Vertices[1].m_Position - Position);
		
		vec2 Dir = normalize(DirLeft + DirRight);	
		vec2 OrthoDir = ortho(Dir);
		
		float Length = 1.0f;
		if(length(DirLeft) > 0.0f)
			Length /= dot(OrthoDir, ortho(DirLeft));
		else if(length(DirRight))
			Length /= dot(OrthoDir, ortho(DirRight));
		
		Vertices[i].m_Thickness = OrthoDir * Length;
	}
}

#endif

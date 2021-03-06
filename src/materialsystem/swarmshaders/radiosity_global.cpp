/*
*	The following header has been retroactively applied to all
*	code modified for use in Alien Swarm Deferred in order to
*	comply with its original Apache License 2.0 specification:
*
* 	Copyright 2013 Kristjan Skutta
*
* 	Licensed under the Apache License, Version 2.0 (the "License");
* 	you may not use this file except in compliance with the License.
* 	You may obtain a copy of the License at
*
* 		http://www.apache.org/licenses/LICENSE-2.0
*
* 	Unless required by applicable law or agreed to in writing, software
* 	distributed under the License is distributed on an "AS IS" BASIS,
* 	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* 	See the License for the specific language governing permissions and
* 	limitations under the License.
*/


#include "deferred_includes.h"

#include "radiosity_gen_global_ps30.inc"
#include "radiosity_gen_vs30.inc"

BEGIN_VS_SHADER( RADIOSITY_GLOBAL, "" )
	BEGIN_SHADER_PARAMS

	END_SHADER_PARAMS

	SHADER_INIT_PARAMS()
	{
	}

	SHADER_INIT
	{
	}

	SHADER_FALLBACK
	{
		return 0;
	}

	SHADER_DRAW
	{
		SHADOW_STATE
		{
			pShaderShadow->SetDefaultState();

			pShaderShadow->EnableDepthTest( false );
			pShaderShadow->EnableDepthWrites( false );
			pShaderShadow->EnableAlphaWrites( false );

			pShaderShadow->EnableTexture( SHADER_SAMPLER0, true );
			pShaderShadow->EnableTexture( SHADER_SAMPLER1, true );
			pShaderShadow->EnableTexture( SHADER_SAMPLER2, true );

			pShaderShadow->VertexShaderVertexFormat( VERTEX_POSITION | VERTEX_TANGENT_S, 1, NULL, 0 );

			DECLARE_STATIC_VERTEX_SHADER( radiosity_gen_vs30 );
			SET_STATIC_VERTEX_SHADER( radiosity_gen_vs30 );

			DECLARE_STATIC_PIXEL_SHADER( radiosity_gen_global_ps30 );
			SET_STATIC_PIXEL_SHADER( radiosity_gen_global_ps30 );
		}
		DYNAMIC_STATE
		{
			const radiosityData_t &data = GetDeferredExt()->GetRadiosityData();
			const int iIndex = pShaderAPI->GetIntRenderingParameter( INT_RENDERPARM_DEFERRED_RADIOSITY_CASCADE );

			pShaderAPI->SetDefaultState();

			DECLARE_DYNAMIC_VERTEX_SHADER( radiosity_gen_vs30 );
			SET_DYNAMIC_VERTEX_SHADER( radiosity_gen_vs30 );

			DECLARE_DYNAMIC_PIXEL_SHADER( radiosity_gen_global_ps30 );
			SET_DYNAMIC_PIXEL_SHADER_COMBO( CASCADE, iIndex );
			SET_DYNAMIC_PIXEL_SHADER( radiosity_gen_global_ps30 );

			BindTexture( SHADER_SAMPLER0, GetDeferredExt()->GetTexture_ShadowRad_Ortho_Albedo() );
			BindTexture( SHADER_SAMPLER1, GetDeferredExt()->GetTexture_ShadowRad_Ortho_Normal() );
			BindTexture( SHADER_SAMPLER2, GetDeferredExt()->GetTexture_ShadowDepth_Ortho( 0 ) );

			Vector vecOrigin = data.vecOrigin[iIndex];
			float fl0[4] = { vecOrigin.x, vecOrigin.y, vecOrigin.z };
			pShaderAPI->SetPixelShaderConstant( 0, fl0 );

			CommitShadowProjectionConstants_Ortho_Single( pShaderAPI, iIndex, 1 );
		}

		Draw();
	}
END_SHADER
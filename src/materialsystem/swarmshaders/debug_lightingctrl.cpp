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

#include "defconstruct_vs30.inc"
#include "debug_lighting_ctrl_ps30.inc"

BEGIN_VS_SHADER( DEBUG_LIGHTING_CTRL, "" )
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

			pShaderShadow->EnableTexture( SHADER_SAMPLER0, true );
			pShaderShadow->EnableTexture( SHADER_SAMPLER1, true );

			pShaderShadow->VertexShaderVertexFormat( VERTEX_POSITION, 1, NULL, 0 );

			DECLARE_STATIC_VERTEX_SHADER( defconstruct_vs30 );
			SET_STATIC_VERTEX_SHADER_COMBO( USEWORLDTRANSFORM, 0 );
			SET_STATIC_VERTEX_SHADER_COMBO( SENDWORLDPOS, 0 );
			SET_STATIC_VERTEX_SHADER( defconstruct_vs30 );

			DECLARE_STATIC_PIXEL_SHADER( debug_lighting_ctrl_ps30 );
			SET_STATIC_PIXEL_SHADER( debug_lighting_ctrl_ps30 );
		}
		DYNAMIC_STATE
		{
			pShaderAPI->SetDefaultState();

			DECLARE_DYNAMIC_VERTEX_SHADER( defconstruct_vs30 );
			SET_DYNAMIC_VERTEX_SHADER( defconstruct_vs30 );

			DECLARE_DYNAMIC_PIXEL_SHADER( debug_lighting_ctrl_ps30 );
			SET_DYNAMIC_PIXEL_SHADER( debug_lighting_ctrl_ps30 );

			BindTexture( SHADER_SAMPLER0, GetDeferredExt()->GetTexture_Normals() );
			BindTexture( SHADER_SAMPLER1, GetDeferredExt()->GetTexture_Depth() );

			CommitBaseDeferredConstants_Frustum( pShaderAPI, VERTEX_SHADER_SHADER_SPECIFIC_CONST_0 );
			CommitBaseDeferredConstants_Origin( pShaderAPI, 0 );
		}

		Draw();
	}

END_SHADER
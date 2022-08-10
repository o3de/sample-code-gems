--------------------------------------------------------------------------------------
--
-- Copyright (c) Contributors to the Open 3D Engine Project.
-- For complete copyright and license terms please see the LICENSE at the root of this distribution.
--
-- SPDX-License-Identifier: Apache-2.0 OR MIT
--
--
--
----------------------------------------------------------------------------------------------------

function GetMaterialPropertyDependencies()
    return {"opacity.mode"}
end

OpacityMode_Opaque = 0
OpacityMode_Cutout = 1
OpacityMode_Blended = 2
OpacityMode_TintedTransparent = 3

function TryGetShaderByTag(context, shaderTag)
    if context:HasShaderWithTag(shaderTag) then
        return context:GetShaderByTag(shaderTag)
    else
        return nil
    end
end

function TrySetShaderEnabled(shader, enabled)
    if shader then 
        shader:SetEnabled(enabled)
    end
end

function Process(context)
    local opacityMode = context:GetMaterialPropertyValue_enum("opacity.mode")
    
    local forwardPassEDS = context:GetShaderByTag("ForwardPass_EDS")
    local depthPassWithPS = context:GetShaderByTag("DepthPass_WithPS")
    depthPassWithPS:SetEnabled(false)
    forwardPassEDS:SetEnabled(true)
    
    context:GetShaderByTag("DepthPassTransparentMin_WithPS"):SetEnabled(false)
    context:GetShaderByTag("DepthPassTransparentMax_WithPS"):SetEnabled(true)
end

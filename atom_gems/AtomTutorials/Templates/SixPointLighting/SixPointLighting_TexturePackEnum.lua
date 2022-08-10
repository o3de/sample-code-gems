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

TexturePackMode_TpLftRtBt_FrBck = 0;
TexturePackMode_RtLftTp_BtBckFr = 1;

function GetMaterialPropertyDependencies()
    return {"sixPointLighting.texturePackMode"}
end

function ProcessEditor(context)
    local texturePackMode = context:GetMaterialPropertyValue_enum("sixPointLighting.texturePackMode");

    -- Depending on which texture pack mode is being used, display the correct one
    if(texturePackMode == TexturePackMode_TpLftRtBt_FrBck) then
        -- TopLeftRightBack is the first texture, FrontBack is the second. Disable RightLeftTop and BottomBackFront.
        context:SetMaterialPropertyVisibility("sixPointLighting.TLRB", MaterialPropertyVisibility_Enabled)
    elseif(texturePackMode == TexturePackMode_RtLftTp_BtBckFr) then
        -- RightLeftTop is the first texture, BottomBackFront is the second. Disable TopLeftRightBack and FrontBack.
        context:SetMaterialPropertyVisibility("sixPointLighting.TLRB", MaterialPropertyVisibility_Hidden)
    end
end
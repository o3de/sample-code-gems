{
    "Source" : "./SixPointLighting_DepthPass_WithPS.azsl",

    "RasterState": { "CullMode": "None" },

    "DepthStencilState" : { 
        "Depth" : { "Enable" : true, "CompareFunc" : "GreaterEqual" }
    },

    "CompilerHints" : { 
        "DisableOptimizations" : false
    },

    "ProgramSettings" : 
    {
        "EntryPoints":
        [
            {
                "name": "MainVS",
                "type" : "Vertex"
            },
            {
                "name": "MainPS",
                "type": "Fragment"
            }
        ] 
    },

    "DrawList" : "depthTransparentMin"
}

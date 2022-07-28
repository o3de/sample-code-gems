{
    "Source" : "./SixPointLighting_DepthPass_WithPS.azsl",

    "RasterState": { "CullMode": "None" },

    "DepthStencilState" : { 
        // Note that we assuming reversed depth buffer, which normally means we 
        // are rendering with GreaterEqual. But in our case we want to render the maximum (furthest) values from the camera.
        "Depth" : { "Enable" : true, "CompareFunc" : "LessEqual" }
    },


    "CompilerHints" : {
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

    "DrawList" : "depthTransparentMax"
}

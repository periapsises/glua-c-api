ModuleRealm = {
    Client = 1,
    Server = 2,
    Both = 3,
}

ModuleArchitecture = {
    x86 = 1,
    x64 = 2,
    Both = 3,
}

local realms = {
    [ModuleRealm.Client] = {"Client"},
    [ModuleRealm.Server] = {"Server"},
    [ModuleRealm.Both] = {"Client", "Server"},
}

local architectures = {
    [ModuleArchitecture.x86] = {"x86"},
    [ModuleArchitecture.x64] = {"x64"},
    [ModuleArchitecture.Both] = {"x86", "x64"},
}

function CreateModuleWorkspace(workspaceName, moduleRealm, moduleArchitecture)
    workspace(workspaceName)

    language "C++"
    warnings "Extra"
    flags { "MultiProcessorCompile", "ShadowedVariables", "UndefinedIdentifiers" }
    characterset "MBCS"
    intrinsics "On"
    inlining "Auto"
    rtti "On"
    vectorextensions "SSE2"

    if moduleArchitecture == ModuleArchitecture.x86 then
        platforms "x86"
    elseif moduleArchitecture == ModuleArchitecture.x64 then
        platforms "x86_64"
    elseif moduleArchitecture == ModuleArchitecture.Both then
        platforms { "x86", "x86_64" }
    end

    filter "platforms:x86"
        architecture "x86"
    filter "platforms:x86_64"
        architecture "x86_64"
    filter {}

    local configs = {}

    for _, realm in ipairs(realms[moduleRealm]) do
        table.insert(configs, realm .. " Debug")
        table.insert(configs, realm .. " Release")
    end

    configurations(configs)

    filter "configurations:*Release*"
        linktimeoptimization "On"
        optimize "Full"
        symbols "Off"
        defines { "NDEBUG" }
        runtime "Release"
    
    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        defines {
            "_CRT_NONSTDC_NO_WARNINGS",
            "_CRT_SECURE_NO_WARNINGS",
            "STRICT",
        }
    
    filter "system:linux"
        cppdialect "GNU++17"
        pic "On"
        staticruntime "On"
        linkoptions "-Wl,--no-undefined"
    
    filter {}

    strictaliasing "Level3"
end

function CreateModuleProject(projectName)
    project(projectName)
    language "C++"
    kind "SharedLib"
    targetextension(".dll")

    local suffix

    filter "system:windows"
        suffix = "_win"
    filter "system:linux"
        suffix = "_linux"

    filter "configurations:*Client*"
        defines { "GMOD_MODULE_CLIENT" }
        targetprefix("gmcl_")
    filter "configurations:*Server*"
        defines { "GMOD_MODULE_SERVER" }
        targetprefix("gmsv_")
    
    filter "configurations:*x86*"
        defines { "GMOD_MODULE_X86" }
        if suffix == "_win" then
            suffix = suffix .. "32"
        end

    filter "configurations:*x64*"
        defines { "GMOD_MODULE_X64" }
        suffix = suffix .. "64"
    filter {}
end

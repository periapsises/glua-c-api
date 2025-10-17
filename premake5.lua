local realmPrefixes = {
    Client = "gmcl_",
    Server = "gmsv_",
}

local architecture

function SetupModule(settings)
    if not settings then settings = {} end

    local realms = settings.realms or { "Client", "Server" }
    local architectures = settings.architectures or { "x86", "x64" }

    local configs = {}

    for _, realm in ipairs(realms) do
        if not realmPrefixes[realm] then
            error("Invalid realm: " .. realm .. " in settings table. Only 'Client' and 'Server' are allowed.")
        end

        for _, architecture in ipairs(architectures) do
            table.insert(configs, realm .. "_" .. architecture .. "_Debug")
            table.insert(configs, realm .. "_" .. architecture .. "_Release")
        end
    end

    configurations(configs)

    for _, realm in ipairs(realms) do
        for _, architecture in ipairs(architectures) do
            local prefix = realmPrefixes[realm]
            local suffix

            filter "system:windows"
                suffix = architecture == "x86" and "_win32" or "_win64"
            filter "system:linux"
                suffix = architecture == "x86" and "_linux32" or "_linux"

            filter("configurations:*" .. realm .. "*" .. architecture .. "*")

            targetprefix(prefix)
            targetsuffix(suffix)
            targetextension(".dll")
            
            if realm == "Client" then
                defines { "GMOD_MODULE_CLIENT" }
            else
                defines { "GMOD_MODULE_SERVER" }
            end
        end
    end
end

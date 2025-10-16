#pragma once

namespace type
{
    enum
    {
        // Default Lua types
        None = -1,
        Nil,
        Bool,
        LightUserData,
        Number,
        String,
        Table,
        Function,
        UserData,
        Thread,

        // Garry's Mod specific types
        Entity,
        Vector,
        Angle,
        PhysObj,
        Save,
        Restore,
        DamageInfo,
        EffectData,
        MoveData,
        RecipientFilter,
        UserCmd,
        ScriptedVehicle,
        Material,
        Panel,
        Particle,
        ParticleEmitter,
        Texture,
        UserMsg,
        ConVar,
        IMesh,
        Matrix,
        Sound,
        PixelVisHandle,
        DLight,
        Video,
        File,
        Locomotion,
        Path,
        NavArea,
        SoundHandle,
        NavLadder,
        ParticleSystem,
        ProjectedTexture,
        PhysCollide,
        SurfaceInfo,

        TypeCount
    };
}

#ifndef MOD_API_H
#define MOD_API_H

#if RETRO_USE_MOD_LOADER
#include <vector>
#include <string>
#include <map>
#include "tinyxml2.h"

#include <functional>
#endif

namespace RSDK
{

#if RETRO_USE_MOD_LOADER

#define LEGACY_PLAYERNAME_COUNT (0x10)

extern std::map<uint32, uint32> superLevels;
extern int32 inheritLevel;

enum ModCallbackEvents {
    MODCB_ONGAMESTARTUP,
    MODCB_ONSTATICLOAD,
    MODCB_ONSTAGELOAD,
    MODCB_ONUPDATE,
    MODCB_ONLATEUPDATE,
    MODCB_ONSTATICUPDATE,
    MODCB_ONDRAW,
    MODCB_ONSTAGEUNLOAD,
    MODCB_ONSHADERLOAD,
    MODCB_ONVIDEOSKIPCB,
    MODCB_ONSCANLINECB,
    MODCB_MAX,
};

enum ModSuper {
    SUPER_UPDATE,
    SUPER_LATEUPDATE,
    SUPER_STATICUPDATE,
    SUPER_DRAW,
    SUPER_CREATE,
    SUPER_STAGELOAD,
    SUPER_EDITORDRAW,
    SUPER_EDITORLOAD,
    SUPER_SERIALIZE
};

enum ModFunctionTableIDs {
    ModTable_RegisterGlobals,
    ModTable_RegisterObject,
    ModTable_RegisterObjectSTD,
    ModTable_RegisterObjectHook,
    ModTable_FindObject,
    ModTable_GetGlobals,
    ModTable_Super,
    ModTable_LoadModInfo,
    ModTable_GetModPath,
    ModTable_GetModCount,
    ModTable_GetModIDByIndex,
    ModTable_ForeachModID,
    ModTable_AddModCallback,
    ModTable_AddModCallbackSTD,
    ModTable_AddPublicFunction,
    ModTable_GetPublicFunction,
    ModTable_GetSettingsBool,
    ModTable_GetSettingsInt,
    ModTable_GetSettingsFloat,
    ModTable_GetSettingsString,
    ModTable_SetSettingsBool,
    ModTable_SetSettingsInt,
    ModTable_SetSettingsFloat,
    ModTable_SetSettingsString,
    ModTable_SaveSettings,
    ModTable_GetConfigBool,
    ModTable_GetConfigInt,
    ModTable_GetConfigFloat,
    ModTable_GetConfigString,
    ModTable_ForeachConfig,
    ModTable_ForeachConfigCategory,
    ModTable_RegisterAchievement,
    ModTable_GetAchievementInfo,
    ModTable_GetAchievementIndexByID,
    ModTable_GetAchievementCount,
    ModTable_LoadShader,
    ModTable_StateMachineRun,
    ModTable_RegisterStateHook,
    ModTable_HandleRunState_HighPriority,
    ModTable_HandleRunState_LowPriority,

#if RETRO_MOD_LOADER_VER >= 2
    // Mod Settings (Part 2)
    ModTable_ForeachSetting,
    ModTable_ForeachSettingCategory, 

    // Files
    ModTable_ExcludeFile,
    ModTable_ExcludeAllFiles,
    ModTable_ReloadFile,
    ModTable_ReloadAllFiles,

    // Graphics
    ModTable_GetSpriteAnimation,
    ModTable_GetSpriteSurface,
    ModTable_GetPaletteBank,
    ModTable_GetActivePaletteBuffer,
    ModTable_GetRGB32To16Buffer,
    ModTable_GetBlendLookupTable,
    ModTable_GetSubtractLookupTable,
    ModTable_GetTintLookupTable,
    ModTable_GetMaskColor,
    ModTable_GetScanEdgeBuffer,
    ModTable_GetCamera,
    ModTable_GetShader,
    ModTable_GetModel,
    ModTable_GetScene3D,
    ModTable_DrawDynamicAniTile,

    // Audio
    ModTable_GetSfx,
    ModTable_GetChannel,

    // Objects/Entities
    ModTable_GetGroupEntities,

    // Collision
    ModTable_SetPathGripSensors,
    ModTable_FloorCollision,
    ModTable_LWallCollision,
    ModTable_RoofCollision,
    ModTable_RWallCollision,
    ModTable_FindFloorPosition,
    ModTable_FindLWallPosition,
    ModTable_FindRoofPosition,
    ModTable_FindRWallPosition,
    ModTable_CopyCollisionMask,
    ModTable_GetCollisionInfo,
#endif

    ModTable_Count
};

typedef void (*ModCallback)(void *data);
typedef std::function<void(void *data)> ModCallbackSTD;

typedef bool (*modLink)(GameInfo *, const char *);
typedef std::function<bool(GameInfo *, const char *)> modLinkSTD;

struct ModPublicFunctionInfo {
    std::string name;
    void *ptr;
};

struct ObjectHook {
    RETRO_HASH_MD5(hash);
    Object **staticVars;
};

struct ModVersionInfo {
    uint8 engineVer;
    uint8 gameVer;
    uint8 modLoaderVer;
    // uint8 engineVer; // 5, 4 or 3
};

struct ModSVInfo {
    std::string name;
    Object **staticVars;
    uint32 size;
};

struct ModInfo {
    std::string path;
    std::string id;
    std::string name;
    std::string desc;
    std::string author;
    std::string version;
    bool active;
    bool redirectSaveRAM;
    bool disableGameLogic;
    bool forceScripts;
    int32 targetVersion;
    int32 forceVersion;
    std::map<std::string, std::string> fileMap;
    std::vector<std::string> excludedFiles;
    std::vector<ModPublicFunctionInfo> functionList;
    std::vector<Link::Handle> modLogicHandles;
    std::vector<modLinkSTD> linkModLogic;
    void (*unloadMod)();
    std::map<std::string, std::map<std::string, std::string>> settings;
    std::map<std::string, std::map<std::string, std::string>> config;

    // mapped to base hash
    std::map<uint32 *, ModSVInfo> staticVars;
};

struct StateHook {
    void (*state)();
    bool32 (*hook)(bool32 skippedState);
    bool32 priority;
};

struct ModSettings {
    int32 activeMod         = -1;
    bool32 redirectSaveRAM  = false;
    bool32 disableGameLogic = false;

#if RETRO_REV0U
    int32 versionOverride = 0;
    bool32 forceScripts   = false;

    char playerNames[LEGACY_PLAYERNAME_COUNT][0x20];
    int32 playerCount = 0;
#endif
};

extern ModSettings modSettings;
extern std::vector<ModInfo> modList;
extern std::vector<ModCallbackSTD> modCallbackList[MODCB_MAX];
extern std::vector<StateHook> stateHookList;
extern std::vector<ObjectHook> objectHookList;
extern ModVersionInfo targetModVersion;

extern char customUserFileDir[0x100];

extern void *modFunctionTable[ModTable_Count];
extern int32 currentObjectID;

extern ModInfo *currentMod;

inline void SetActiveMod(int32 id) { modSettings.activeMod = id; }

void InitModAPI();
void UnloadMods();
void LoadMods(bool newOnly = false);
bool32 LoadMod(ModInfo *info, std::string modsPath, std::string folder, bool32 active);
void SaveMods();
void SortMods();
void LoadModSettings();
void ApplyModChanges();

inline std::vector<ModInfo *> ActiveMods()
{
    SortMods();
    std::vector<ModInfo *> ret;
    for (int32 m = 0; m < modList.size(); ++m) {
        if (!modList[m].active)
            break;
        ret.push_back(&modList.at(m));
    }
    return ret;
}

bool32 ScanModFolder(ModInfo *info, const char *targetFile = nullptr);
inline void RefreshModFolders()
{
    int32 activeModCount = (int32)ActiveMods().size();
    for (int32 m = 0; m < activeModCount; ++m) {
        ScanModFolder(&modList[m]);
    }
}

void RunModCallbacks(int32 callbackID, void *data);

// Mod API
#if RETRO_REV0U
void ModRegisterGlobalVariables(const char *globalsPath, void **globals, uint32 size);

void ModRegisterObject(Object **staticVars, Object **modStaticVars, const char *name, uint32 entityClassSize, uint32 staticClassSize,
                       uint32 modClassSize, void (*update)(), void (*lateUpdate)(), void (*staticUpdate)(), void (*draw)(), void (*create)(void *),
                       void (*stageLoad)(), void (*editorDraw)(), void (*editorLoad)(), void (*serialize)(), void (*staticLoad)(Object *),
                       const char *inherited);

void ModRegisterObject_STD(Object **staticVars, Object **modStaticVars, const char *name, uint32 entityClassSize, uint32 staticClassSize,
                           uint32 modClassSize, std::function<void()> update, std::function<void()> lateUpdate, std::function<void()> staticUpdate,
                           std::function<void()> draw, std::function<void(void *)> create, std::function<void()> stageLoad,
                           std::function<void()> editorDraw, std::function<void()> editorLoad, std::function<void()> serialize,
                           std::function<void(Object *)> staticLoad, const char *inherited);
#else
void ModRegisterGlobalVariables(const char *globalsPath, void **globals, uint32 size);

void ModRegisterObject(Object **staticVars, Object **modStaticVars, const char *name, uint32 entityClassSize, uint32 staticClassSize,
                       uint32 modClassSize, void (*update)(), void (*lateUpdate)(), void (*staticUpdate)(), void (*draw)(), void (*create)(void *),
                       void (*stageLoad)(), void (*editorDraw)(), void (*editorLoad)(), void (*serialize)(), const char *inherited);

void ModRegisterObject_STD(Object **staticVars, Object **modStaticVars, const char *name, uint32 entityClassSize, uint32 staticClassSize,
                           uint32 modClassSize, std::function<void()> update, std::function<void()> lateUpdate, std::function<void()> staticUpdate,
                           std::function<void()> draw, std::function<void(void *)> create, std::function<void()> stageLoad,
                           std::function<void()> editorDraw, std::function<void()> editorLoad, std::function<void()> serialize,
                           const char *inherited);
#endif

void ModRegisterObjectHook(Object **staticVars, const char *staticName);
Object *ModFindObject(const char *name);

void *GetGlobals();

bool32 LoadModInfo(const char *folder, String *name, String *description, String *version, bool32 *active);
void GetModPath(const char *id, String *result);
int32 GetModCount(bool32 active);
const char *GetModIDByIndex(uint32 index);
bool32 ForeachModID(String *id);

void AddModCallback(int32 callbackID, ModCallback callback);
void AddModCallback_STD(int32 callbackID, ModCallbackSTD callback);
void AddPublicFunction(const char *functionName, void *functionPtr);
void *GetPublicFunction(const char *id, const char *functionName);

bool32 GetSettingsBool(const char *id, const char *key, bool32 fallback);
int32 GetSettingsInteger(const char *id, const char *key, int32 fallback);
float GetSettingsFloat(const char *id, const char *key, float fallback);
void GetSettingsString(const char *id, const char *key, String *result, const char *fallback);

void SetSettingsBool(const char *key, bool32 val);
void SetSettingsInteger(const char *key, int32 val);
void SetSettingsFloat(const char *key, float val);
void SetSettingsString(const char *key, String *val);

void SaveSettings();

bool32 GetConfigBool(const char *key, bool32 fallback);
int32 GetConfigInteger(const char *key, int32 fallback);
float GetConfigFloat(const char *key, float fallback);
void GetConfigString(const char *key, String *result, const char *fallback);
bool32 ForeachConfig(String *config);
bool32 ForeachConfigCategory(String *category);
#if RETRO_MOD_LOADER_VER >= 2
bool32 ForeachSetting(const char *id, String *setting);
bool32 ForeachSettingCategory(const char *id, String *category);
#endif

void Super(int32 objectID, ModSuper callback, void *data);

void GetAchievementInfo(uint32 id, String *name, String *description, String *identifer, bool32 *achieved);
int32 GetAchievementIndexByID(const char *id);
int32 GetAchievementCount();

void StateMachineRun(void (*state)());
bool32 HandleRunState_HighPriority(void *state);
void HandleRunState_LowPriority(void *state, bool32 skipState);
void RegisterStateHook(void (*state)(), bool32 (*hook)(bool32 skippedState), bool32 priority);


#if RETRO_MOD_LOADER_VER >= 2

// Files
bool32 ExcludeFile(const char *id, const char *path);
bool32 ExcludeAllFiles(const char *id);
bool32 ReloadFile(const char *id, const char *path);
bool32 ReloadAllFiles(const char *id);

// Graphics
inline void *GetSpriteAnimation(uint16 id)
{
    if (id >= SPRFILE_COUNT)
        return NULL;
    return &spriteAnimationList[id];
}
inline void *GetSpriteSurface(uint16 id)
{
    if (id >= SURFACE_COUNT)
        return NULL;
    return &gfxSurface[id];
}
inline uint16 *GetPaletteBank(uint8 id)
{
    if (id >= PALETTE_BANK_COUNT)
        return NULL;
    return fullPalette[id];
}
inline uint8 *GetActivePaletteBuffer() { return gfxLineBuffer; }
inline void GetRGB32To16Buffer(uint16 **rgb32To16_R, uint16 **rgb32To16_G, uint16 **rgb32To16_B)
{
    if (rgb32To16_R)
        *rgb32To16_R = RSDK::rgb32To16_R;

    if (rgb32To16_G)
        *rgb32To16_G = RSDK::rgb32To16_G;

    if (rgb32To16_B)
        *rgb32To16_B = RSDK::rgb32To16_B;
}
inline uint16 *GetBlendLookupTable() { return blendLookupTable; }
inline uint16 *GetSubtractLookupTable() { return subtractLookupTable; }
inline color GetMaskColor() { return maskColor; }
inline void *GetScanEdgeBuffer() { return scanEdgeBuffer; }
inline void *GetCamera(uint8 id)
{
    if (id >= CAMERA_COUNT)
        return NULL;
    return &cameras[id];
}
inline void *GetShader(uint8 id)
{
    if (id >= SHADER_COUNT)
        return NULL;
    return &shaderList[id];
}
inline void *GetModel(uint16 id)
{
    if (id >= MODEL_COUNT)
        return NULL;
    return &modelList[id];
}
inline void *GetScene3D(uint16 id)
{
    if (id >= SCENE3D_COUNT)
        return NULL;
    return &scene3DList[id];
}

// Audio
inline void *GetSfxEntry(uint16 id)
{
    if (id >= SFX_COUNT)
        return NULL;
    return &sfxList[id];
}
inline void *GetChannel(uint8 id)
{
    if (id >= CHANNEL_COUNT)
        return NULL;
    return &channels[id];
}

// Objects/Entities
bool32 GetGroupEntities(uint16 group, void **entity);
#endif

#endif

#if RETRO_REV0U
#include "Legacy/ModAPILegacy.hpp"
#endif

} // namespace RSDK

#endif // !MOD_API_H

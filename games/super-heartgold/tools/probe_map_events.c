/* Diagnostic only: include unchanged upstream unit to exercise its static parser. */
#include "map_events.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do { if (!(x)) { fprintf(stderr,"check line %d\n",__LINE__); abort(); } } while (0)
#ifdef HG_SCRIPT_DISPATCH
#include "script_dispatch_cases.h"
#ifdef HG_MOVEMENT_REQUEST
#include <setjmp.h>
#include "map_object_movement_commands.h"
#include "movement_request_cases.h"
static jmp_buf movement_boundary;
static LocalMapObject *expected_movement_object;
static const u8 *expected_movement_data;
static unsigned movement_requests;
static void prepare_movement_save(FieldSystem *field);
BOOL ScrCmd_ApplyMovement(ScriptContext *ctx);
#ifdef HG_MOVEMENT_TASK
#include "system.h"
#include "sys_task.h"
#include "map_object_movement_commands_internal.h"
struct System gSystem;
static EventObjectMovementMan *created_movement;
BOOL HG_MovementMatches(EventObjectMovementMan *,LocalMapObject *,const u8 *);
void *Heap_AllocAtEnd(enum HeapID heap,u32 size) { return Heap_Alloc(heap,size); }
void Heap_FreeExplicit(enum HeapID heap,void *ptr) { (void)heap; Heap_Free(ptr); }
#else
EventObjectMovementMan *EventObjectMovementMan_Create(LocalMapObject *object,const MovementScriptCommand *data) {
    CHECK(object==expected_movement_object && (const u8 *)data==expected_movement_data);
    movement_requests++; longjmp(movement_boundary,1);
}
#endif
void FollowMonDraw_ClearBob(LocalMapObject *object) { (void)object; CHECK(0); }
void _ScheduleObjectEventMovement(FieldSystem *field,EventObjectMovementMan *movement,MovementScriptCommand *commands) {
#ifdef HG_MOVEMENT_TASK
    CHECK(field && commands==NULL);
    CHECK(HG_MovementMatches(movement,expected_movement_object,expected_movement_data));
    created_movement=movement; movement_requests++; longjmp(movement_boundary,1);
#else
    (void)field; (void)movement; (void)commands; CHECK(0);
#endif
}
#endif
#ifdef HG_INACTIVE_FOLLOWER
#include "party.h"
#include "script_pokemon_util.h"
BOOL ScrCmd_FollowMonResetMovement(ScriptContext *ctx);
Party *SaveArray_Party_Get(SaveData *save) { (void)save; CHECK(0); return NULL; }
int CountAlivePokemon(Party *party) { (void)party; CHECK(0); return 0; }
#ifndef HG_LOCK_ALL
LocalMapObject *MapObjectManager_GetFirstActiveObjectByID(MapObjectManager *manager,u32 id) {
    (void)manager; (void)id; CHECK(0); return NULL;
}
#endif
void FollowPlayerMovement_Reset(LocalMapObject *object) { (void)object; CHECK(0); }
#ifdef HG_LOCK_ALL
BOOL ScrCmd_LockAll(ScriptContext *ctx);
BOOL ScrCmd_LockLastTalked(ScriptContext *ctx) { (void)ctx; CHECK(0); return FALSE; }
static ScriptEnvironment script_environment;
#endif
#endif
static unsigned dispatched, blocked_opcode;
static BOOL stop_at_unintegrated_command(ScriptContext *ctx) {
    CHECK(dispatched==0);
    blocked_opcode=ctx->script_ptr[-2] | (ctx->script_ptr[-1]<<8);
    dispatched++;
    StopScript(ctx);
    return TRUE;
}
static void verify_script_dispatch(const char *directory) {
#ifdef HG_MOVEMENT_TASK
    void *queue_memory=Heap_Alloc(0,SysTaskQueue_GetArenaSize(8));
    gSystem.mainTaskQueue=SysTaskQueue_PlacementNew(8,queue_memory);
#endif
    unsigned char bytecode[DISPATCH_BANK_SIZE];
    char path[4096]; CHECK(snprintf(path,sizeof(path),"%s/initial-house.bytecode",directory)<sizeof(path));
    FILE *f=fopen(path,"rb"); CHECK(f);
    CHECK(fread(bytecode,1,sizeof(bytecode),f)==sizeof(bytecode)); CHECK(fgetc(f)==EOF); fclose(f);
    ScrCmdFunc table[DISPATCH_COMMAND_COUNT];
    for (unsigned i=0;i<DISPATCH_COMMAND_COUNT;i++) table[i]=stop_at_unintegrated_command;
    for (unsigned i=0;i<2;i++) {
        ScriptContext ctx; memset(&ctx,0xA5,sizeof(ctx));
        InitScriptContext(&ctx,table,DISPATCH_COMMAND_COUNT);
        CHECK(ctx.mode==SCRIPT_MODE_STOPPED && ctx.stackDepth==0 && ctx.script_ptr==NULL);
        CHECK(!RunScriptCommand(&ctx));
        CHECK(SetupBytecodeScript(&ctx,bytecode));
        ScriptRunByIndex(&ctx,dispatch_cases[i].script-1);
        CHECK(ctx.script_ptr==bytecode+dispatch_cases[i].offset);
        dispatched=0; blocked_opcode=0xFFFF;
#ifdef HG_INACTIVE_FOLLOWER
        FieldSystem field={0}; ctx.fieldSystem=&field;
        table[dispatch_cases[i].opcode]=ScrCmd_FollowMonResetMovement;
        CHECK(RunScriptCommand(&ctx));
        CHECK(dispatched==0 && ctx.mode==SCRIPT_MODE_BYTECODE);
        CHECK(ctx.script_ptr==bytecode+dispatch_cases[i].offset+2);
        CHECK(field.followMon.active==0);
#ifdef HG_LOCK_ALL
        LocalMapObject objects[3]={0}; MapObjectManager manager={0};
        unsigned original_flags[3]={MAPOBJECTFLAG_ACTIVE,0,MAPOBJECTFLAG_ACTIVE | MAPOBJECTFLAG_SINGLE_MOVEMENT};
        for (unsigned n=0;n<3;n++) objects[n].flags=original_flags[n];
        manager.objectCount=3; manager.activeCount=2; manager.objects=objects;
        manager.fieldSystem=&field; field.mapObjectManager=&manager;
#ifdef HG_MOVEMENT_TASK
        manager.priority=10;
        for (unsigned n=0;n<3;n++) objects[n].manager=&manager;
#endif
        memset(&script_environment,0,sizeof(script_environment));
        field.taskman=(TaskManager *)&script_environment;
        unsigned lock_opcode=ctx.script_ptr[0] | (ctx.script_ptr[1]<<8);
        table[lock_opcode]=ScrCmd_LockAll;
        CHECK(RunScriptCommand(&ctx));
        CHECK(!dispatched && ctx.mode==SCRIPT_MODE_BYTECODE);
        CHECK(ctx.script_ptr==bytecode+dispatch_cases[i].offset+4);
        for (unsigned n=0;n<3;n++)
            CHECK(objects[n].flags==(original_flags[n] | (n==1 ? 0 : MAPOBJECTFLAG_MOVEMENT_PAUSED)));
        MapObjectManager_UnpauseAllMovement(&manager);
        for (unsigned n=0;n<3;n++) CHECK(objects[n].flags==original_flags[n]);
#endif
        unsigned expected_opcode=ctx.script_ptr[0] | (ctx.script_ptr[1]<<8);
#else
        unsigned expected_opcode=dispatch_cases[i].opcode;
#endif
#ifdef HG_MOVEMENT_REQUEST
        objects[0].id=255; objects[2].id=0;
        prepare_movement_save(&field);
        expected_movement_object=&objects[0]; expected_movement_data=bytecode+movement_targets[i];
        movement_requests=0; table[expected_opcode]=ScrCmd_ApplyMovement;
        if (setjmp(movement_boundary)==0) { RunScriptCommand(&ctx); CHECK(0); }
        CHECK(movement_requests==1);
#ifdef HG_MOVEMENT_TASK
        CHECK(script_environment.activeMovementCounter==1);
        CHECK(gSystem.mainTaskQueue->activeCount==1);
        CHECK(SysTask_GetPriority((SysTask *)created_movement)==9);
        for (unsigned tick=0;tick<16;tick++) {
            SysTaskQueue_RunTasks(gSystem.mainTaskQueue);
            CHECK(!EventObjectMovementMan_IsFinish(created_movement));
        }
        CHECK(objects[0].movementCmd==(expected_movement_data[0] | (expected_movement_data[1]<<8)));
        CHECK(!MapObject_IsMovementPaused(&objects[0]));
        /* Explicit cancellation: never report absent step execution as success. */
        MapObject_ClearHeldMovement(&objects[0]);
        EventObjectMovementMan_Delete(created_movement); created_movement=NULL;
        script_environment.activeMovementCounter=0;
        CHECK(gSystem.mainTaskQueue->activeCount==0);
#else
        CHECK(script_environment.activeMovementCounter==0);
#endif
        StopScript(&ctx);
#else
        CHECK(RunScriptCommand(&ctx));
        CHECK(dispatched==1 && blocked_opcode==expected_opcode);
#endif
        CHECK(ctx.mode==SCRIPT_MODE_STOPPED && !RunScriptCommand(&ctx));
    }
#ifdef HG_MOVEMENT_TASK
    CHECK(gSystem.mainTaskQueue->activeCount==0);
    gSystem.mainTaskQueue=NULL; Heap_Free(queue_memory);
    puts("Original movement tasks created and ticked; unfinished without step executor; cancelled explicitly");
#elif defined(HG_MOVEMENT_REQUEST)
    puts("ApplyMovement: object and signed relative target verified; trapped before creating tasks");
#elif defined(HG_LOCK_ALL)
    puts("original VM: first two commands executed twice; stopped before ApplyMovement; zero scripts completed");
#elif defined(HG_INACTIVE_FOLLOWER)
    puts("original VM: inactive-follower command executed twice; stopped before LockAll; zero scripts completed");
#else
    puts("original VM: two real script entries dispatched; stopped at unintegrated command, not completed");
#endif
}
#endif
void GF_AssertFail(void) { abort(); }
void SIM_handleAssertionFailureMsg(const char *file,unsigned line,const char *fmt,...) {
    (void)fmt; fprintf(stderr,"assert %s:%u\n",file,line); abort();
}
#ifdef HG_FIELD_LOAD
#include "field_load_cases.h"
#ifdef HG_SCRIPT_SELECTION
#include "save_vars_flags.h"
static SaveVarsFlags save_vars;
#ifdef HG_MOVEMENT_REQUEST
static void prepare_movement_save(FieldSystem *field) { field->saveData=(SaveData *)&save_vars; Save_VarsFlags_Init(&save_vars); }
#endif
void *SaveArray_Get(SaveData *save,int id) {
    CHECK(save==(SaveData *)&save_vars && id==SAVE_FLAGS); return &save_vars;
}
void *TaskManager_GetEnvironment(TaskManager *task) {
#ifdef HG_LOCK_ALL
    CHECK(task==(TaskManager *)&script_environment); return &script_environment;
#else
    (void)task; CHECK(0); return NULL; /* Special variables are outside this test. */
#endif
}
static void verify_script_selection(FieldSystem *field,unsigned map) {
    field->saveData=(SaveData *)&save_vars;
    Save_VarsFlags_Init(&save_vars);
    u8 *header=MapEvents_GetScriptHeader(field);
    /* Oracle independently decoded from the private headers by the runner. */
    for (unsigned value=0;value<5;value++) {
        CHECK(FieldSystem_VarSet(field,0x4106,value));
        CHECK(FieldSystem_VarGet(field,0x4106)==value);
        CHECK(FieldSystem_VarGet(field,3)==3);
        u16 expected=map==63 ? (value==0 ? 1 : value==3 ? 7 : 0xFFFF) : 0xFFFF;
        CHECK(GetMapSceneScriptId(field,header,1)==expected);
        CHECK(MapSceneScriptCheck(field)==(expected!=0xFFFF));
        field->warpResetSuppressed=1;
        CHECK(!MapSceneScriptCheck(field));
        field->warpResetSuppressed=0;
    }
    for (unsigned type=2;type<=4;type++) CHECK(GetMapLoadScriptId(header,type)==0xFFFF);
    CHECK(!FieldSystem_VarSet(field,3,99));
}
#endif
static const char *resource_dir;
static unsigned allocations, reads;
static const FieldLoadCase *find_case(u32 map) {
    for (unsigned i=0;i<2;i++) if (field_load_cases[i].map==map) return &field_load_cases[i];
    CHECK(0); return NULL;
}
u16 MapHeader_GetEventsBank(u32 map) { return find_case(map)->events; }
u16 MapHeader_GetScriptHeaderBank(u32 map) { return find_case(map)->script; }
BOOL MapHeader_HasWildEncounters(u32 map) { find_case(map); return FALSE; }
u8 MapHeader_GetWildEncounterBank(u32 map) { (void)map; CHECK(0); return 0; }
void *Heap_Alloc(enum HeapID heap,u32 size) {
    (void)heap; void *p=malloc(size); CHECK(p); memset(p,0xA5,size); allocations++; return p;
}
void Heap_Free(void *p) { CHECK(p && allocations); allocations--; free(p); }
static FILE *open_member(NarcId narc,s32 member) {
    const char *suffix;
    if (narc==NARC_fielddata_eventdata_zone_event) suffix="bin";
    else { CHECK(narc==NARC_fielddata_script_scr_seq); suffix="script"; }
    BOOL allowed=FALSE;
    for (unsigned i=0;i<2;i++)
        if (member==(narc==NARC_fielddata_eventdata_zone_event ? field_load_cases[i].events : field_load_cases[i].script)) allowed=TRUE;
    CHECK(allowed);
    char path[4096]; CHECK(snprintf(path,sizeof(path),"%s/%ld.%s",resource_dir,(long)member,suffix)<sizeof(path));
    FILE *f=fopen(path,"rb"); CHECK(f); return f;
}
u32 GetNarcMemberSizeByIdPair(NarcId narc,s32 member) {
    FILE *f=open_member(narc,member); CHECK(!fseek(f,0,SEEK_END));
    long size=ftell(f); CHECK(size>0); fclose(f); return size;
}
void ReadWholeNarcMemberByIdPair(void *dest,NarcId narc,s32 member) {
    u32 size=GetNarcMemberSizeByIdPair(narc,member);
    CHECK(size < (narc==NARC_fielddata_eventdata_zone_event ? 0x800 : 0x100));
    FILE *f=open_member(narc,member); CHECK(fread(dest,1,size,f)==size); fclose(f); reads++;
}
static void verify_field_load(const char *directory) {
    resource_dir=directory;
    for (unsigned cycle=0;cycle<64;cycle++) {
        FieldSystem field={0}; Field_AllocateMapEvents(&field,0); CHECK(allocations==1);
        for (unsigned i=0;i<2;i++) {
            const FieldLoadCase *c=&field_load_cases[i];
            memset(field.mapEvents,0xA5,sizeof(*field.mapEvents));
            Field_InitMapEvents(&field,c->map);
            CHECK(allocations==1);
            const unsigned char *enc=(void *)MapEvents_GetLoadedEncTable(&field);
            for (unsigned j=0;j<sizeof(EncounterData);j++) CHECK(enc[j]==0);
            unsigned char *script=MapEvents_GetScriptHeader(&field);
            for (unsigned j=c->script_size;j<0x100;j++) CHECK(script[j]==0);
#ifdef HG_SCRIPT_SELECTION
            verify_script_selection(&field,c->map);
#endif
            char path[4096]; CHECK(snprintf(path,sizeof(path),"%s/%u.loaded",directory,c->map)<sizeof(path));
            FILE *out=fopen(path,"wb"); CHECK(out);
            CHECK(fwrite(field.mapEvents->event_data,1,c->event_size,out)==c->event_size);
            CHECK(fwrite(script,1,c->script_size,out)==c->script_size); CHECK(!fclose(out));
        }
        Field_FreeMapEvents(&field); field.mapEvents=NULL; CHECK(!allocations);
    }
    CHECK(reads==256);
    puts("Field_InitMapEvents: 128 room loads, 64 allocations/releases passed");
}
#endif
static void section(FILE *out, u32 count, const void *data, size_t size) {
    CHECK((count == 0) == (data == NULL));
    CHECK(fwrite(&count,4,1,out)==1);
    if (count) CHECK(fwrite(data,size,count,out)==count);
}
int main(int argc,char **argv) {
    CHECK(argc==3);
    _Static_assert(sizeof(BgEvent)==20,"BG disk ABI");
    _Static_assert(sizeof(ObjectEvent)==32,"Object disk ABI");
    _Static_assert(sizeof(WarpEvent)==12,"Warp disk ABI");
    _Static_assert(sizeof(CoordEvent)==16,"Coord disk ABI");
    FieldSystem field={0}; MapEvents events; field.mapEvents=&events;
    unsigned members=strtoul(argv[2],NULL,10);
    for (unsigned n=0;n<members;n++) {
        char path[4096]; CHECK(snprintf(path,sizeof(path),"%s/%u.bin",argv[1],n)<sizeof(path));
        FILE *in=fopen(path,"rb"); CHECK(in);
        memset(&events,0xA5,sizeof(events));
        size_t length=fread(events.event_data,1,sizeof(events.event_data),in);
        CHECK(length>=16 && length<sizeof(events.event_data) && !ferror(in)); fclose(in);
        /* Inputs are independently bounded by the Python runner before execution. */
        MapEvents_ComputeRamHeader(&events);
        CHECK(snprintf(path,sizeof(path),"%s/%u.out",argv[1],n)<sizeof(path));
        FILE *out=fopen(path,"wb"); CHECK(out);
        section(out,Field_GetNumBgEvents(&field),Field_GetBgEvents(&field),sizeof(BgEvent));
        section(out,Field_GetNumObjectEvents(&field),Field_GetObjectEvents(&field),sizeof(ObjectEvent));
        section(out,events.num_warp_events,Field_GetWarpEventI(&field,0),sizeof(WarpEvent));
        section(out,Field_GetNumCoordEvents(&field),Field_GetCoordEvents(&field),sizeof(CoordEvent));
        CHECK(!fclose(out));
        CHECK(!Field_GetWarpEventI(&field,events.num_warp_events));
        CHECK(!Field_GetWarpEventI(&field,0xFFFFFFFF));
        CHECK(Field_GetWarpEventAtXYPos(&field,-1,-1)==-1);
        for (unsigned i=0;i<events.num_warp_events;i++) {
            const WarpEvent *w=Field_GetWarpEventI(&field,i);
            int first=0;
            while (events.warp_events[first].x!=w->x || events.warp_events[first].z!=w->z) first++;
            CHECK(Field_GetWarpEventAtXYPos(&field,w->x,w->z)==first);
        }
    }
#ifdef HG_FIELD_LOAD
    verify_field_load(argv[1]);
#endif
#ifdef HG_SCRIPT_DISPATCH
    verify_script_dispatch(argv[1]);
#ifdef HG_MOVEMENT_TASK
    CHECK(allocations==0);
#endif
#endif
    puts("original event parser/getters/warp lookup passed"); return 0;
}

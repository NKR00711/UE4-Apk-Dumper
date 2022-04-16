//
// Created by Ascarre on 08-04-2022.
//

#ifndef UE4_SDK_GENERATOR_MAIN_H
#define UE4_SDK_GENERATOR_MAIN_H

#include "Dumper.h"
#include "Canvas.h"

bool StringsDumped = false, ObjectsDumped = false, SdkDumped = false, SdkWDumped = false;

void RenderHacks(Canvas Draw, int SWidth, int SHeight) {
    Draw.DrawText(Color(255,40,40), "Dumper Made By Ascarre", Vector2(SWidth / 8, SHeight / 8), 25);

    ProcessId = FindProcessId(Offsets::ProcessName);
    ModuleBase = GetModuleBase(ProcessId, Offsets::ModuleName);
    ModuleEnd = GetModuleEnd(ProcessId, Offsets::ModuleName);

    if (isStringDump && StringsDumped == false) {
        StringsDumped = true;
        DumpStrings(ModuleBase);
    }

    if (isObjectsDump && ObjectsDumped == false) {
        ObjectsDumped = true;
        DumpObjects(ModuleBase);
    }

    if (isSdkDump && SdkDumped == false) {
        SdkDumped = true;
        DumpSDK(ModuleBase);
    }

    if (isSdkWDump && SdkWDumped == false) {
        SdkWDumped = true;
        DumpSDKW(ModuleBase);
    }
}

#endif //UE4_SDK_GENERATOR_MAIN_H

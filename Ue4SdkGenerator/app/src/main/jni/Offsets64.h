//
// Created by Ascarre on 08-04-2022.
//

#ifndef UE4_SDK_GENERATOR_OFFSETS64_H
#define UE4_SDK_GENERATOR_OFFSETS64_H

bool isStringDump = false, isObjectsDump = false, isSdkDump = false, isSdkWDump = false;

namespace Offsets {

    const char* ProcessName;
    const char* ModuleName = "libUE4.so";

    enum Offsets {
        PointerSize = 0x8,
        NameString = 0x10,

        GNames = 0x120,
        Vtable = 0x110,

        GUObjectArray = 0x120,
        TUObjectArray = 0x10,
        NumElements = 0xC,
        ClassPrivate = 0x10,
        FNameIndex = 0x18,

        GWorld = 0x120,
        GWorldPointer = 0x3C,
        PersistentLevel = 0x30,
        Actors = 0x98,
        ActorsCount = 0xA0,

        OuterPrivate = 0x20,

        Next = 0x28,

        SuperStruct = 0x30,
        Children = 0x38,

        FunctionFlags = 0x88,
        Func = 0xB0,

        ElementSize = 0x34,
        PropertyFlags = 0x38,
        OffsetInternal = 0x44,

        ByteOffset = 0x71,
        ByteMask = 0x72,
        FieldMask = 0x73,

        PropertyClass = 0x70,

        MetaClass = 0x78,

        InterfaceClass = 0x78,

        InnerProperty = 0x70,

        KeyProp = 0x70,
        ValueProp = 0x78,

        ElementProp = 0x70,

        Struct = 0x70,
    };
}

#endif //UE4_SDK_GENERATOR_OFFSETS64_H

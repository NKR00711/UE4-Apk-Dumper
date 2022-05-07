//
// Created by Ascarre on 07-05-2022.
//

#ifndef UE4_SDK_GENERATOR_OFFSETS_H
#define UE4_SDK_GENERATOR_OFFSETS_H

bool isStringDump = false, isObjectsDump = false, isSdkDump = false, isSdkWDump = false;

namespace Offsets {

    const char* ProcessName;
    const char* ModuleName = "libUE4.so";

    enum Offsets {
        PointerSize = 0x4,
        NameString = 0x8,

        GNames = 0x75EFD9C,//0x75EFD9C
        Vtable = 0x80,

        GUObjectArray = 0x777FE70,//0x777FE70
        TUObjectArray = 0x10,
        NumElements = 0x8,
        ClassPrivate = 0xC,
        FNameIndex = 0x10,
        OuterPrivate = 0x18,

        GWorld = 0x78EFCB4,
        GWorldPointer = 0x3C,
        PersistentLevel = 0x20,
        Actors = 0x70,
        ActorsCount = 0x74,

        Next = 0x1C,

        SuperStruct = 0x20,
        Children = 0x24,

        FunctionFlags = 0x58,
        Func = 0x74,

        ElementSize = 0x24,
        PropertyFlags = 0x28,
        OffsetInternal = 0x34,

        ByteOffset = 0x51,
        ByteMask = 0x52,
        FieldMask = 0x53,

        PropertyClass = 0x50,

        MetaClass = 0x54,

        InterfaceClass = 0x54,

        InnerProperty = 0x50,

        KeyProp = 0x50,
        ValueProp = 0x54,

        ElementProp = 0x50,

        Struct = 0x50,
    };
}

#endif //UE4_SDK_GENERATOR_OFFSETS_H

//
// Created by Ascarre on 07-05-2022.
//

#ifndef UE4_SDK_GENERATOR_STRUCTURES_H
#define UE4_SDK_GENERATOR_STRUCTURES_H

#if defined(__LP64__)
#include "Offsets64.h"

#else

#include "Offsets.h"

#endif

typedef signed int int32;
typedef unsigned int uint32;
typedef unsigned char uint8;
typedef unsigned long long uint64;

uint32 MAX_SIZE = 100;

string GetFNameFromID(uintptr_t ModuleBase, uint32 index) {
    uintptr_t GNames = Read<uintptr_t>(Read<uintptr_t>(ModuleBase + Offsets::GNames) + Offsets::Vtable);
    //uintptr_t GNames = Read<uintptr_t>(ModuleBase + Offsets::GNames);
    uintptr_t FNameEntryArr = Read<uintptr_t>(GNames + ((index / 0x4000) * Offsets::PointerSize));
    uintptr_t FNameEntry = Read<uintptr_t>(FNameEntryArr + ((index % 0x4000) * Offsets::PointerSize));
    return ReadString(FNameEntry + Offsets::NameString, MAX_SIZE);
}

struct UObject {
    static uintptr_t getClass(uintptr_t object) {//UClass*
        return Read<uintptr_t>(object + Offsets::ClassPrivate);
    }

    static uint32 getNameID(uintptr_t object) {
        return Read<uint32>(object + Offsets::FNameIndex);
    }

    static uintptr_t getOuter(uintptr_t object) {//UObject*
        return Read<uintptr_t>(object + Offsets::OuterPrivate);
    }

    static string getName(uintptr_t ModuleBase, uintptr_t object) {
        return GetFNameFromID(ModuleBase, getNameID(object));
    }

    static string getClassName(uintptr_t ModuleBase, uintptr_t object) {
        return getName(ModuleBase, getClass(object));
    }

    static bool isValid(uintptr_t object) {
        return (object != 0 && getNameID(object) != 0 && getClass(object) != 0);
    }
};

struct UField {
    static uintptr_t getNext(uintptr_t field) {//UField*
        return Read<uintptr_t>(field + Offsets::Next);
    }
};

struct UStruct {
    static uintptr_t getSuperClass(uintptr_t structz) {//UStruct*
        return Read<uintptr_t>(structz + Offsets::SuperStruct);
    }

    static uintptr_t getChildren(uintptr_t structz) {//UField*
        return Read<uintptr_t>(structz + Offsets::Children);
    }

    static string getStructClassPath(uintptr_t ModuleBase, uintptr_t clazz) {
        string classname = UObject::getName(ModuleBase,clazz);

        uintptr_t superclass = getSuperClass(clazz);
        while (superclass) {
            classname += ".";
            classname += UObject::getName(ModuleBase, superclass);

            superclass = getSuperClass(superclass);
        }

        return classname;
    }

    static bool isValid(uintptr_t clazz) {
        return (clazz > 0 && UObject::getNameID(clazz) > 0 && UObject::getClass(clazz) > 0);
    }
};

struct UFunction {
    static int32 getFunctionFlags(uintptr_t func) {
        return Read<int32>(func + Offsets::FunctionFlags);
    }

    static uintptr_t getFunc(uintptr_t func) {
        return Read<uintptr_t>(func + Offsets::Func);
    }
};

struct UProperty {
    static int32 getElementSize(uintptr_t prop) {
        return Read<int32>(prop + Offsets::ElementSize);
    }

    static uint64 getPropertyFlags(uintptr_t prop) {
        return Read<uint64>(prop + Offsets::PropertyFlags);
    }

    static int32 getOffset(uintptr_t prop) {
        return Read<int32>(prop + Offsets::OffsetInternal);
    }
};

struct UBoolProperty {
    static uint8 getByteOffset(uintptr_t prop) {
        return Read<uint8>(prop + Offsets::ByteOffset);
    }

    static uint8 getByteMask(uintptr_t prop) {
        return Read<uint8>(prop + Offsets::ByteMask);
    }

    static uint8 getFieldMask(uintptr_t prop) {
        return Read<uint8>(prop + Offsets::FieldMask);
    }
};

struct UObjectProperty {
    static uintptr_t getPropertyClass(uintptr_t prop) {//class UClass*
        return Read<uintptr_t>(prop + Offsets::PropertyClass);
    }
};

struct UClassProperty {
    static uintptr_t getMetaClass(uintptr_t prop) {//class UClass*
        return Read<uintptr_t>(prop + Offsets::MetaClass);
    }
};

struct UInterfaceProperty {
    static uintptr_t getInterfaceClass(uintptr_t prop) {//class UClass*
        return Read<uintptr_t>(prop + Offsets::InterfaceClass);
    }
};

struct UArrayProperty {
    static uintptr_t getInner(uintptr_t prop) {//UProperty*
        return Read<uintptr_t>(prop + Offsets::InnerProperty);
    }
};

struct UMapProperty {
    static uintptr_t getKeyProp(uintptr_t prop) {//UProperty*
        return Read<uintptr_t>(prop + Offsets::KeyProp);
    }

    static uintptr_t getValueProp(uintptr_t prop) {//UProperty*
        return Read<uintptr_t>(prop + Offsets::ValueProp);
    }
};

struct USetProperty {
    static uintptr_t getElementProp(uintptr_t prop) {//UProperty*
        return Read<uintptr_t>(prop + Offsets::ElementProp);
    }
};

struct UStructProperty {
    static uintptr_t getStruct(uintptr_t prop) {//UStruct*
        return Read<uintptr_t>(prop + Offsets::Struct);
    }
};

#endif //UE4_SDK_GENERATOR_STRUCTURES_H

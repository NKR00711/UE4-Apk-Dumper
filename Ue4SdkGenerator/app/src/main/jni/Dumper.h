//
// Created by Ascarre on 08-04-2022.
//

#ifndef UE4_SDK_GENERATOR_DUMPER_H
#define UE4_SDK_GENERATOR_DUMPER_H

#include <iostream>
#include <fstream>
#include <list>

#include "Memory.h"
#include <vector>

#include "Structures.h"

#if defined(__LP64__)
#include "Offsets64.h"

#else

#include "Offsets.h"

#endif

typedef unsigned int uint32;
typedef signed int int32;

uint32 GNameLimit = 170000;
uint32 classCount = 0;

string OutPutPathDirectory("/sdcard/DumpedGames/");

vector<uint32> structIDMap;

bool isStartWith(string str, const char *check) {
    return (str.rfind(check, 0) == 0);
}

bool isEqual(string s1, const char *check) {
    string s2(check);
    return (s1 == s2);
}

bool isContain(string str, string check) {
    size_t found = str.find(check);
    return (found != string::npos);
}

bool isScanned(uint32 id) {
    for (int i = 0; i < structIDMap.size(); i++) {
        if (structIDMap[i] == id) { return true; }
    }
    return false;
}

void DumpStrings(uintptr_t ModuleBase) {
    uint32 count = 0;
    ofstream gname(OutPutPathDirectory + Offsets::ProcessName + "/NamesDump.txt", ofstream::out);
    if (gname.is_open()) {
        for (uint32 i = 0; i < GNameLimit; i++) {
            string s = GetFNameFromID(ModuleBase, i);
            if (!s.empty()) {
                gname << "[" << i << "]: " << s << endl;
                count++;
            }
        }
        gname.close();
    }
}

int32 GetObjectCount(uintptr_t ModuleBase) {
    return Read<int32>((ModuleBase + Offsets::GUObjectArray) + Offsets::TUObjectArray + Offsets::NumElements);
}

uintptr_t GetUObjectFromID(uintptr_t ModuleBase, uint32 index) {
    uintptr_t FUObjectArray = (ModuleBase + Offsets::GUObjectArray);
    uintptr_t TUObjectArray = Read<uintptr_t>(FUObjectArray + Offsets::TUObjectArray);
    return Read<uintptr_t>(TUObjectArray + (index * Offsets::NumElements));
}

void DumpObjects(uintptr_t ModuleBase) {
    uint32 count = 0;
    ofstream obj(OutPutPathDirectory + Offsets::ProcessName + "/Objects.txt", ofstream::out);
    if (obj.is_open()) {
        for (int32 i = 0; i < GetObjectCount(ModuleBase); i++) {
            uintptr_t UObjectArray = GetUObjectFromID(ModuleBase, i);

            uintptr_t Class = Read<uintptr_t>(UObjectArray + Offsets::ClassPrivate);
            uint32 NameId = Read<uint32>(UObjectArray + Offsets::FNameIndex);
            string Name = GetFNameFromID(ModuleBase, NameId);
            uint32 ClassNameId = Read<uint32>(Class + Offsets::FNameIndex);
            string ClassName = GetFNameFromID(ModuleBase, ClassNameId);

            if (UObjectArray != 0 && NameId != 0 && Class != 0) {
                obj << setbase(10) << "[" << i << "]:" << endl;
                obj << "Name: " << Name.c_str() << endl;
                obj << "Class: " << ClassName.c_str() << endl;
                obj << "ObjectPtr: 0x" << setbase(16) << UObjectArray << endl;
                obj << "ClassPtr: 0x" << setbase(16) << Class << endl;
                obj << endl;
                count++;
            }
        }
        obj.close();
    }
}

string resolveProp(uintptr_t ModuleBase, list<uintptr_t> &recurrce, uintptr_t prop) {
    if (prop) {
        string cname = UObject::getClassName(ModuleBase, prop);

        if (isEqual(cname, "ObjectProperty") || isEqual(cname, "WeakObjectProperty")
            || isEqual(cname, "LazyObjectProperty") || isEqual(cname, "AssetObjectProperty")
            || isEqual(cname, "SoftObjectProperty")) {
            uintptr_t propertyClass = UObjectProperty::getPropertyClass(prop);
            recurrce.push_back(propertyClass);
            return UObject::getName(ModuleBase, propertyClass) + "*";
        } else if (isEqual(cname, "ClassProperty") || isEqual(cname, "AssetClassProperty") ||
                   isEqual(cname, "SoftClassProperty")) {
            uintptr_t metaClass = UClassProperty::getMetaClass(prop);
            recurrce.push_back(metaClass);
            return "class " + UObject::getName(ModuleBase, metaClass);
        } else if (isEqual(cname, "InterfaceProperty")) {
            uintptr_t interfaceClass = UInterfaceProperty::getInterfaceClass(prop);
            recurrce.push_back(interfaceClass);
            return "interface class" + UObject::getName(ModuleBase, interfaceClass);
        } else if (isEqual(cname, "StructProperty")) {
            uintptr_t Struct = UStructProperty::getStruct(prop);
            recurrce.push_back(Struct);
            return UObject::getName(ModuleBase, Struct);
        } else if (isEqual(cname, "ArrayProperty")) {
            return resolveProp(ModuleBase, recurrce, UArrayProperty::getInner(prop)) + "[]";
        } else if (isEqual(cname, "SetProperty")) {
            return "<" + resolveProp(ModuleBase, recurrce, USetProperty::getElementProp(prop)) + ">";
        } else if (isEqual(cname, "MapProperty")) {
            return "<" + resolveProp(ModuleBase, recurrce, UMapProperty::getKeyProp(prop)) + "," +
                   resolveProp(ModuleBase, recurrce, UMapProperty::getValueProp(prop)) + ">";
        } else if (isEqual(cname, "BoolProperty")) {
            return "bool";
        } else if (isEqual(cname, "ByteProperty")) {
            return "byte";
        } else if (isEqual(cname, "IntProperty")) {
            return "int";
        } else if (isEqual(cname, "Int8Property")) {
            return "int8";
        } else if (isEqual(cname, "Int16Property")) {
            return "int16";
        } else if (isEqual(cname, "Int64Property")) {
            return "int64";
        } else if (isEqual(cname, "UInt16Property")) {
            return "uint16";
        } else if (isEqual(cname, "UInt32Property")) {
            return "uint32";
        } else if (isEqual(cname, "UInt64Property")) {
            return "uint64";
        } else if (isEqual(cname, "DoubleProperty")) {
            return "double";
        } else if (isEqual(cname, "FloatProperty")) {
            return "float";
        } else if (isEqual(cname, "EnumProperty")) {
            return "enum";
        } else if (isEqual(cname, "StrProperty")) {
            return "FString";
        } else if (isEqual(cname, "TextProperty")) {
            return "FText";
        } else if (isEqual(cname, "NameProperty")) {
            return "FName";
        } else if (isEqual(cname, "DelegateProperty") ||
                   isEqual(cname, "MulticastDelegateProperty")) {
            return "delegate";
        } else {
            return UObject::getName(ModuleBase, prop) + "(" + cname + ")";
        }
    }
    return "NULL";
}

list<uintptr_t> writeStructChild(uintptr_t ModuleBase, ofstream &sdk, uintptr_t child) {
    list<uintptr_t> recurrce;
    while (child) {
        uintptr_t prop = child;
        string oname = UObject::getName(ModuleBase, prop);
        string cname = UObject::getClassName(ModuleBase, prop);

        if (isEqual(cname, "ObjectProperty") || isEqual(cname, "WeakObjectProperty") || isEqual(cname, "LazyObjectProperty") || isEqual(cname, "AssetObjectProperty") || isEqual(cname, "SoftObjectProperty")) {
            uintptr_t propertyClass = UObjectProperty::getPropertyClass(prop);

            sdk << "\t" << UObject::getName(ModuleBase, propertyClass) << "* " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: " << setbase(10) << UProperty::getElementSize(prop) << "]" << endl;

            recurrce.push_back(propertyClass);
        } else if (isEqual(cname, "ClassProperty") || isEqual(cname, "AssetClassProperty") ||
                   isEqual(cname, "SoftClassProperty")) {
            uintptr_t metaClass = UClassProperty::getMetaClass(prop);

            sdk << "\tclass " << UObject::getName(ModuleBase, metaClass) << "* " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: " << setbase(10) << UProperty::getElementSize(prop) << "]" << endl;

            recurrce.push_back(metaClass);
        } else if (isEqual(cname, "InterfaceProperty")) {
            uintptr_t interfaceClass = UInterfaceProperty::getInterfaceClass(prop);

            sdk << "\tinterface class " << UObject::getName(ModuleBase, interfaceClass) << "* " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: " << setbase(10) << UProperty::getElementSize(prop) << "]" << endl;
        } else if (isEqual(cname, "StructProperty")) {
            uintptr_t Struct = UStructProperty::getStruct(prop);

            sdk << "\t" << UObject::getName(ModuleBase, Struct) << " " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: " << setbase(10) << UProperty::getElementSize(prop) << "]" << endl;

            recurrce.push_back(Struct);
        } else if (isEqual(cname, "ArrayProperty")) {
            sdk << "\t" << resolveProp(ModuleBase, recurrce, UArrayProperty::getInner(prop)) << "[] " << oname
                << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: " << setbase(10) << UProperty::getElementSize(prop) << "]" << endl;
        } else if (isEqual(cname, "SetProperty")) {
            sdk << "\t<" << resolveProp(ModuleBase, recurrce, USetProperty::getElementProp(prop)) << "> "
                << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: " << setbase(10) << UProperty::getElementSize(prop) << "]" << endl;
        } else if (isEqual(cname, "MapProperty")) {
            sdk << "\t<" << resolveProp(ModuleBase, recurrce, UMapProperty::getKeyProp(prop)) << ","
                << resolveProp(ModuleBase, recurrce, UMapProperty::getValueProp(prop)) << "> " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: " << setbase(10) << UProperty::getElementSize(prop) << "]" << endl;
        } else if (isEqual(cname, "BoolProperty")) {
            sdk << "\tbool " << oname << ";"
                << setbase(10) << "//(ByteOffset: " << (int) UBoolProperty::getByteOffset(prop)
                << ", ByteMask: " << (int) UBoolProperty::getByteMask(prop)
                << ", FieldMask: " << (int) UBoolProperty::getFieldMask(prop) << ")"
                << "[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: " << setbase(10) << UProperty::getElementSize(prop) << "]" << endl;
        } else if (isEqual(cname, "ByteProperty")) {
            sdk << "\tbyte " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: " << setbase(10) << UProperty::getElementSize(prop) << "]" << endl;
        } else if (isEqual(cname, "IntProperty")) {
            sdk << "\tint " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: " << setbase(10) << UProperty::getElementSize(prop) << "]" << endl;
        } else if (isEqual(cname, "Int8Property")) {
            sdk << "\tint8 " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: " << setbase(10) << UProperty::getElementSize(prop) << "]" << endl;
        } else if (isEqual(cname, "Int16Property")) {
            sdk << "\tint16 " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: " << setbase(10) << UProperty::getElementSize(prop) << "]" << endl;
        } else if (isEqual(cname, "Int64Property")) {
            sdk << "\tint64 " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: " << setbase(10) << UProperty::getElementSize(prop) << "]" << endl;
        } else if (isEqual(cname, "UInt16Property")) {
            sdk << "\tuint16 " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: " << setbase(10) << UProperty::getElementSize(prop) << "]" << endl;
        } else if (isEqual(cname, "UInt32Property")) {
            sdk << "\tuint32 " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: " << setbase(10) << UProperty::getElementSize(prop) << "]" << endl;
        } else if (isEqual(cname, "UInt64Property")) {
            sdk << "\tuint64 " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: " << setbase(10) << UProperty::getElementSize(prop) << "]" << endl;
        } else if (isEqual(cname, "DoubleProperty")) {
            sdk << "\tdouble " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: " << setbase(10) << UProperty::getElementSize(prop) << "]" << endl;
        } else if (isEqual(cname, "FloatProperty")) {
            sdk << "\tfloat " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: " << setbase(10) << UProperty::getElementSize(prop) << "]" << endl;
        } else if (isEqual(cname, "EnumProperty")) {
            sdk << "\tenum " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: " << setbase(10) << UProperty::getElementSize(prop) << "]" << endl;
        } else if (isEqual(cname, "StrProperty")) {
            sdk << "\tFString " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: " << setbase(10) << UProperty::getElementSize(prop) << "]" << endl;
        } else if (isEqual(cname, "TextProperty")) {
            sdk << "\tFText " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: " << setbase(10) << UProperty::getElementSize(prop) << "]" << endl;
        } else if (isEqual(cname, "NameProperty")) {
            sdk << "\tFName " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: " << setbase(10) << UProperty::getElementSize(prop) << "]" << endl;
        } else if (isEqual(cname, "DelegateProperty") ||
                   isEqual(cname, "MulticastDelegateProperty")) {
            sdk << "\tdelegate " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: " << setbase(10) << UProperty::getElementSize(prop) << "]" << endl;
        } else if (isStartWith(cname, "Function") || isEqual(cname, "DelegateFunction")) {
            string returnVal = "void";
            string params = "";

            uintptr_t funcParam = UStruct::getChildren(prop);
            while (funcParam) {
                uint64 PropertyFlags = UProperty::getPropertyFlags(funcParam);

                if ((PropertyFlags & 0x0000000000000400) == 0x0000000000000400) {
                    returnVal = resolveProp(ModuleBase, recurrce, funcParam);
                } else {
                    if ((PropertyFlags & 0x0000000000000100) == 0x0000000000000100) {
                        params += "out ";
                    }

                    if ((PropertyFlags & 0x0000000000000002) == 0x0000000000000002) {
                        params += "const ";
                    }
                    params += resolveProp(ModuleBase, recurrce, funcParam);
                    params += " ";
                    params += UObject::getName(ModuleBase, funcParam);
                    params += ", ";
                }

                funcParam = UField::getNext(funcParam);
            }

            if (!params.empty()) {
                params.pop_back();
                params.pop_back();
            }

            sdk << "\t";

            int32 FunctionFlags = UFunction::getFunctionFlags(prop);

            if ((FunctionFlags & 0x00002000) == 0x00002000) {
                sdk << "static" << " ";
            }
            sdk << returnVal << " " << oname << "(" << params << ");"
                << "// 0x" << setbase(16) << (UFunction::getFunc(prop) - ModuleBase) << endl;
        } else {
            sdk << "\t" << cname << " " << oname << ";"
                << "//[Size: " << setbase(10) << UProperty::getElementSize(prop) << "]" << endl;
        }

        child = UField::getNext(child);
    }
    return recurrce;
}

void writeStruct(uintptr_t ModuleBase, ofstream &sdk, uintptr_t clazz) {
    list<uintptr_t> recurrce;

    uintptr_t currStruct = clazz;
    while (UStruct::isValid(currStruct)) {
        string name = UObject::getName(ModuleBase, currStruct);
        if (isEqual(name, "None") || isContain(name, "_png") || name.empty()) {
            break;
        }

        uint32 NameID = UObject::getNameID(currStruct);
        if (!isScanned(NameID)) {
            structIDMap.push_back(NameID);
            sdk << "Class: " << UStruct::getStructClassPath(ModuleBase, currStruct) << endl;
            recurrce.merge(writeStructChild(ModuleBase, sdk, UStruct::getChildren(currStruct)));
            sdk << "\n--------------------------------" << endl;
            classCount++;
        }

        currStruct = UStruct::getSuperClass(currStruct);
    }

    for (auto it = recurrce.begin(); it != recurrce.end(); ++it)
        writeStruct(ModuleBase, sdk, *it);
}

void DumpSDK(uintptr_t ModuleBase) {
    ofstream sdk(OutPutPathDirectory + Offsets::ProcessName + "/SDK.txt", ofstream::out);
    if (sdk.is_open()) {
        for (int32 i = 0; i < GetObjectCount(ModuleBase); i++) {
            uintptr_t uobj = GetUObjectFromID(ModuleBase, i);
            if (UObject::isValid(uobj)) {
                writeStruct(ModuleBase, sdk, UObject::getClass(uobj));
            }
        }
        sdk.close();
    }
}

void DumpSDKW(uintptr_t ModuleBase) {
    ofstream sdk(OutPutPathDirectory + Offsets::ProcessName + "/SDKW.txt", ofstream::out);
    if (sdk.is_open()) {
        uintptr_t GWorld = Read<uintptr_t>(Read<uintptr_t>(ModuleBase + Offsets::GWorld) + Offsets::GWorldPointer);

        if (UObject::isValid(GWorld)) {
            writeStruct(ModuleBase, sdk, UObject::getClass(GWorld));
            uintptr_t PersistentLevel = Read<uintptr_t>(GWorld + Offsets::PersistentLevel);
            uintptr_t Actors = Read<uintptr_t>(PersistentLevel + Offsets::Actors);
            int ActorsCount = Read<int>(PersistentLevel + Offsets::ActorsCount);
            for (int i = 0; i < ActorsCount; i++) {
                uintptr_t actor =  Read<uintptr_t>(Actors + (i * Offsets::PointerSize));
                if (UObject::isValid(actor)) {
                    writeStruct(ModuleBase, sdk, UObject::getClass(actor));
                }
            }
        }
        sdk.close();
    }
}

#endif //UE4_SDK_GENERATOR_DUMPER_H

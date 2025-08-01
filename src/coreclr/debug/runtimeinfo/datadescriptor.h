// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
//
// No include guards. This file is included multiple times.

// The format is:
// CDAC_BASELINE("string") baseline data contract that the runtime should follow. "empty" is reasonable
// CDAC_TYPES_BEGIN()
// ... <types> ...
// CDAC_TYPES_END()
// CDAC_GLOBALS_BEGIN()
// ... <globals> ...
// CDAC_GLOBALS_END()
//
// In <types> the format is:
// CDAC_TYPE_BEGIN(cdacTypeIdentifier) // defined a new data descriptor named cdacIdentifier
//
// CDAC_TYPE_SIZE(k) -or- CDAC_TYPE_INDETERMINATE(cdacTypeIdentifier) specifies that the type has
//   size k (bytes - usually sizeof(SomeNativeType)) or specify that the type's size is not provided
//   It is important that CDAC_TYPE_SIZE or CDAC_TYPE_INDETERMINATE immediately follows
//   CDAC_TYPE_BEGIN
//
// CDAC_TYPE_FIELD(cdacTypeIdentifier, cdacFieldTypeIdentifier, cdacFieldName, k) specifies the
//   field of "cdacTypeIdentifier" that has name cdacFieldName and has the type
//   "cdacFieldtypeIdentifier" located at offset k in the type layout.  k is usually
//   offsetof(SomeClass, m_FieldName) if the field is public
//
//     if the field is private, the convention is that SomeClass declares a friend struct
//     cdac_data<T> and provides a specialization of cdac_data<T> with a public constexpr
//     size_t member that holds the offset:
//
//     class MyClass {
//       private:
//         void* m_myField;
//         friend template<typename T> cdac_data<T>;
//     };
//     template<> struct cdac_data<MyClass> {
//       static constexpr size_t MyField = offsetof(MyClass, m_myField);
//     };
//
//     then the field layout can be specified as
//     CDAC_TYPE_FIELD(MyClassLayout, pointer, MyField, cdac_data<MyClass>::MyField)
//  There can be zero or more CDAC_TYPE_FIELD entries per type layout
//  For types mapping to managed objects, use exact managed type field names in the descriptor, as
//  field names often can't change due to binary serialization or implicit diagnostic contracts
//
// CDAC_TYPE_END(cdacTypeIdentifier)  specifies the end of the type layout for cdacTypeIdentifier
//
// In <globals> the format is:
//
// CDAC_GLOBAL(cdacGlobalName, cdacTypeIdentifier, value)
// or
// CDAC_GLOBAL_POINTER(cdacGlobalName, cdacTypeIdentifier, address)
//
// Zero or more globals can be defined
//
// if a global is given with CDAC_GLOBAL(), `value` should be a constexpr uint64_t (or convertible
// to uint64_t) for example, it can be a literal constant or a preprocessor definition
//
// if a global is a CDAC_GLOBAL_POINTER(), address should be a constexpr pointer or a constexpr
// uintptr_t
//
//
//
// This file is compiled using the target architecture.  Preprocessor defines for the target
// platform will be available.  It is ok to use `#ifdef`.

#ifndef CDAC_BASELINE
#define CDAC_BASELINE(identifier)
#endif
#ifndef CDAC_TYPES_BEGIN
#define CDAC_TYPES_BEGIN()
#endif
#ifndef CDAC_TYPE_BEGIN
#define CDAC_TYPE_BEGIN(tyname)
#endif
#ifndef CDAC_TYPE_SIZE
#define CDAC_TYPE_SIZE(k)
#endif
#ifndef CDAC_TYPE_INDETERMINATE
#define CDAC_TYPE_INDETERMINATE(tyname)
#endif
#ifndef CDAC_TYPE_FIELD
#define CDAC_TYPE_FIELD(tyname,fieldtyname,fieldname,off)
#endif
#ifndef CDAC_TYPE_END
#define CDAC_TYPE_END(tyname)
#endif
#ifndef CDAC_TYPES_END
#define CDAC_TYPES_END()
#endif
#ifndef CDAC_GLOBALS_BEGIN
#define CDAC_GLOBALS_BEGIN()
#endif
#ifndef CDAC_GLOBAL
#define CDAC_GLOBAL(globalname,tyname,val)
#endif
#ifndef CDAC_GLOBAL_POINTER
#define CDAC_GLOBAL_POINTER(globalname,addr)
#endif
#ifndef CDAC_GLOBAL_STRING
#define CDAC_GLOBAL_STRING(globalname,stringval)
#endif
#ifndef CDAC_GLOBALS_END
#define CDAC_GLOBALS_END()
#endif

CDAC_BASELINE("empty")
CDAC_TYPES_BEGIN()

CDAC_TYPE_BEGIN(Thread)
CDAC_TYPE_INDETERMINATE(Thread)
CDAC_TYPE_FIELD(Thread, /*uint32*/, Id, cdac_data<Thread>::Id)
CDAC_TYPE_FIELD(Thread, /*nuint*/, OSId, cdac_data<Thread>::OSId)
CDAC_TYPE_FIELD(Thread, /*uint32*/, State, cdac_data<Thread>::State)
CDAC_TYPE_FIELD(Thread, /*uint32*/, PreemptiveGCDisabled, cdac_data<Thread>::PreemptiveGCDisabled)
CDAC_TYPE_FIELD(Thread, /*pointer*/, RuntimeThreadLocals, cdac_data<Thread>::RuntimeThreadLocals)
CDAC_TYPE_FIELD(Thread, /*pointer*/, Frame, cdac_data<Thread>::Frame)
CDAC_TYPE_FIELD(Thread, /*pointer*/, ExceptionTracker, cdac_data<Thread>::ExceptionTracker)
CDAC_TYPE_FIELD(Thread, GCHandle, GCHandle, cdac_data<Thread>::ExposedObject)
CDAC_TYPE_FIELD(Thread, GCHandle, LastThrownObject, cdac_data<Thread>::LastThrownObject)
CDAC_TYPE_FIELD(Thread, pointer, LinkNext, cdac_data<Thread>::Link)
#ifndef TARGET_UNIX
CDAC_TYPE_FIELD(Thread, /*pointer*/, TEB, cdac_data<Thread>::TEB)
#endif
CDAC_TYPE_END(Thread)

CDAC_TYPE_BEGIN(ThreadStore)
CDAC_TYPE_INDETERMINATE(ThreadStore)
CDAC_TYPE_FIELD(ThreadStore, /*SLink*/, FirstThreadLink, cdac_data<ThreadStore>::FirstThreadLink)
CDAC_TYPE_FIELD(ThreadStore, /*int32*/, ThreadCount, cdac_data<ThreadStore>::ThreadCount)
CDAC_TYPE_FIELD(ThreadStore, /*int32*/, UnstartedCount, cdac_data<ThreadStore>::UnstartedCount)
CDAC_TYPE_FIELD(ThreadStore, /*int32*/, BackgroundCount, cdac_data<ThreadStore>::BackgroundCount)
CDAC_TYPE_FIELD(ThreadStore, /*int32*/, PendingCount, cdac_data<ThreadStore>::PendingCount)
CDAC_TYPE_FIELD(ThreadStore, /*int32*/, DeadCount, cdac_data<ThreadStore>::DeadCount)
CDAC_TYPE_END(ThreadStore)

CDAC_TYPE_BEGIN(RuntimeThreadLocals)
CDAC_TYPE_INDETERMINATE(RuntimeThreadLocals)
CDAC_TYPE_FIELD(RuntimeThreadLocals, /*EEAllocContext*/, AllocContext, offsetof(RuntimeThreadLocals, alloc_context))
CDAC_TYPE_END(RuntimeThreadLocals)

CDAC_TYPE_BEGIN(EEAllocContext)
CDAC_TYPE_INDETERMINATE(EEAllocContext)
CDAC_TYPE_FIELD(EEAllocContext, /*GCAllocContext*/, GCAllocationContext, offsetof(ee_alloc_context, m_GCAllocContext))
CDAC_TYPE_END(EEAllocContext)

CDAC_TYPE_BEGIN(GCAllocContext)
CDAC_TYPE_INDETERMINATE(GCAllocContext)
CDAC_TYPE_FIELD(GCAllocContext, /*pointer*/, Pointer, offsetof(gc_alloc_context, alloc_ptr))
CDAC_TYPE_FIELD(GCAllocContext, /*pointer*/, Limit, offsetof(gc_alloc_context, alloc_limit))
CDAC_TYPE_END(GCAllocContext)

// Exception

// Use exact managed type field names for the descriptor as field names often can't change due to binary serialization or implicit diagnostic contracts
CDAC_TYPE_BEGIN(Exception)
CDAC_TYPE_INDETERMINATE(Exception)
CDAC_TYPE_FIELD(Exception, /*pointer*/, _message, cdac_data<ExceptionObject>::_message)
CDAC_TYPE_FIELD(Exception, /*pointer*/, _innerException, cdac_data<ExceptionObject>::_innerException)
CDAC_TYPE_FIELD(Exception, /*pointer*/, _stackTrace, cdac_data<ExceptionObject>::_stackTrace)
CDAC_TYPE_FIELD(Exception, /*pointer*/, _watsonBuckets, cdac_data<ExceptionObject>::_watsonBuckets)
CDAC_TYPE_FIELD(Exception, /*pointer*/, _stackTraceString, cdac_data<ExceptionObject>::_stackTraceString)
CDAC_TYPE_FIELD(Exception, /*pointer*/, _remoteStackTraceString, cdac_data<ExceptionObject>::_remoteStackTraceString)
CDAC_TYPE_FIELD(Exception, /*int32*/, _HResult, cdac_data<ExceptionObject>::_HResult)
CDAC_TYPE_FIELD(Exception, /*int32*/, _xcode, cdac_data<ExceptionObject>::_xcode)
CDAC_TYPE_END(Exception)

CDAC_TYPE_BEGIN(ExceptionInfo)
CDAC_TYPE_INDETERMINATE(ExceptionInfo)
CDAC_TYPE_FIELD(ExceptionInfo, /*pointer*/, ThrownObject, offsetof(ExInfo, m_hThrowable))
CDAC_TYPE_FIELD(PreviousNestedInfo, /*pointer*/, PreviousNestedInfo, offsetof(ExInfo, m_pPrevNestedInfo))
CDAC_TYPE_END(ExceptionInfo)


CDAC_TYPE_BEGIN(GCHandle)
CDAC_TYPE_SIZE(sizeof(OBJECTHANDLE))
CDAC_TYPE_END(GCHandle)

// Object

CDAC_TYPE_BEGIN(Object)
CDAC_TYPE_INDETERMINATE(Object)
CDAC_TYPE_FIELD(Object, /*pointer*/, m_pMethTab, cdac_data<Object>::m_pMethTab)
CDAC_TYPE_END(Object)

CDAC_TYPE_BEGIN(String)
CDAC_TYPE_INDETERMINATE(String)
CDAC_TYPE_FIELD(String, /*pointer*/, m_FirstChar, cdac_data<StringObject>::m_FirstChar)
CDAC_TYPE_FIELD(String, /*uint32*/, m_StringLength, cdac_data<StringObject>::m_StringLength)
CDAC_TYPE_END(String)

CDAC_TYPE_BEGIN(Array)
CDAC_TYPE_SIZE(sizeof(ArrayBase))
CDAC_TYPE_FIELD(Array, /*pointer*/, m_NumComponents, cdac_data<ArrayBase>::m_NumComponents)
CDAC_TYPE_END(Array)

CDAC_TYPE_BEGIN(InteropSyncBlockInfo)
CDAC_TYPE_INDETERMINATE(InteropSyncBlockInfo)
#ifdef FEATURE_COMINTEROP
CDAC_TYPE_FIELD(InteropSyncBlockInfo, /*pointer*/, CCW, cdac_data<InteropSyncBlockInfo>::CCW)
CDAC_TYPE_FIELD(InteropSyncBlockInfo, /*pointer*/, RCW, cdac_data<InteropSyncBlockInfo>::RCW)
#endif // FEATURE_COMINTEROP
CDAC_TYPE_END(InteropSyncBlockInfo)

CDAC_TYPE_BEGIN(SyncBlock)
CDAC_TYPE_INDETERMINATE(SyncBlock)
CDAC_TYPE_FIELD(SyncBlock, /*pointer*/, InteropInfo, cdac_data<SyncBlock>::InteropInfo)
CDAC_TYPE_END(SyncBlock)

CDAC_TYPE_BEGIN(SyncTableEntry)
CDAC_TYPE_SIZE(sizeof(SyncTableEntry))
CDAC_TYPE_FIELD(SyncTableEntry, /*pointer*/, SyncBlock, offsetof(SyncTableEntry, m_SyncBlock))
CDAC_TYPE_END(SyncTableEntry)

// Loader

CDAC_TYPE_BEGIN(Module)
CDAC_TYPE_INDETERMINATE(Module)
CDAC_TYPE_FIELD(Module, /*pointer*/, Assembly, cdac_data<Module>::Assembly)
CDAC_TYPE_FIELD(Module, /*pointer*/, PEAssembly, cdac_data<Module>::PEAssembly)
CDAC_TYPE_FIELD(Module, /*pointer*/, Base, cdac_data<Module>::Base)
CDAC_TYPE_FIELD(Module, /*uint32*/, Flags, cdac_data<Module>::Flags)
CDAC_TYPE_FIELD(Module, /*pointer*/, LoaderAllocator, cdac_data<Module>::LoaderAllocator)
CDAC_TYPE_FIELD(Module, /*pointer*/, DynamicMetadata, cdac_data<Module>::DynamicMetadata)
CDAC_TYPE_FIELD(Module, /*pointer*/, Path, cdac_data<Module>::Path)
CDAC_TYPE_FIELD(Module, /*pointer*/, FileName, cdac_data<Module>::FileName)
CDAC_TYPE_FIELD(Module, /*pointer*/, ReadyToRunInfo, cdac_data<Module>::ReadyToRunInfo)
CDAC_TYPE_FIELD(Module, /*pointer*/, GrowableSymbolStream, cdac_data<Module>::GrowableSymbolStream)

CDAC_TYPE_FIELD(Module, /*pointer*/, FieldDefToDescMap, cdac_data<Module>::FieldDefToDescMap)
CDAC_TYPE_FIELD(Module, /*pointer*/, ManifestModuleReferencesMap, cdac_data<Module>::ManifestModuleReferencesMap)
CDAC_TYPE_FIELD(Module, /*pointer*/, MemberRefToDescMap, cdac_data<Module>::MemberRefToDescMap)
CDAC_TYPE_FIELD(Module, /*pointer*/, MethodDefToDescMap, cdac_data<Module>::MethodDefToDescMap)
CDAC_TYPE_FIELD(Module, /*pointer*/, TypeDefToMethodTableMap, cdac_data<Module>::TypeDefToMethodTableMap)
CDAC_TYPE_FIELD(Module, /*pointer*/, TypeRefToMethodTableMap, cdac_data<Module>::TypeRefToMethodTableMap)
CDAC_TYPE_FIELD(Module, /*pointer*/, MethodDefToILCodeVersioningStateMap, cdac_data<Module>::MethodDefToILCodeVersioningStateMap)
CDAC_TYPE_END(Module)

CDAC_TYPE_BEGIN(ModuleLookupMap)
CDAC_TYPE_FIELD(ModuleLookupMap, /*pointer*/, TableData, offsetof(LookupMapBase, pTable))
CDAC_TYPE_FIELD(ModuleLookupMap, /*pointer*/, Next, offsetof(LookupMapBase, pNext))
CDAC_TYPE_FIELD(ModuleLookupMap, /*uint32*/, Count, offsetof(LookupMapBase, dwCount))
CDAC_TYPE_FIELD(ModuleLookupMap, /*nuint*/, SupportedFlagsMask, offsetof(LookupMapBase, supportedFlags))
CDAC_TYPE_END(ModuleLookupMap)

CDAC_TYPE_BEGIN(Assembly)
CDAC_TYPE_INDETERMINATE(Assembly)
#ifdef FEATURE_COLLECTIBLE_TYPES
CDAC_TYPE_FIELD(Assembly, /*uint8*/, IsCollectible, cdac_data<Assembly>::IsCollectible)
#endif
CDAC_TYPE_FIELD(Assembly, /*pointer*/, Module, cdac_data<Assembly>::Module)
CDAC_TYPE_FIELD(Assembly, /*pointer*/, Error, cdac_data<Assembly>::Error)
CDAC_TYPE_FIELD(Assembly, /*uint32*/, NotifyFlags, cdac_data<Assembly>::NotifyFlags)
CDAC_TYPE_FIELD(Assembly, /*uint32*/, Level, cdac_data<Assembly>::Level)
CDAC_TYPE_END(Assembly)

CDAC_TYPE_BEGIN(LoaderAllocator)
CDAC_TYPE_INDETERMINATE(LoaderAllocator)
CDAC_TYPE_FIELD(LoaderAllocator, /*uint32*/, ReferenceCount, cdac_data<LoaderAllocator>::ReferenceCount)
CDAC_TYPE_END(LoaderAllocator)

CDAC_TYPE_BEGIN(PEAssembly)
CDAC_TYPE_INDETERMINATE(PEAssembly)
CDAC_TYPE_FIELD(PEAssembly, /*pointer*/, PEImage, cdac_data<PEAssembly>::PEImage)
CDAC_TYPE_END(PEAssembly)

CDAC_TYPE_BEGIN(PEImage)
CDAC_TYPE_INDETERMINATE(PEImage)
CDAC_TYPE_FIELD(PEImage, /*pointer*/, LoadedImageLayout, cdac_data<PEImage>::LoadedImageLayout)
CDAC_TYPE_FIELD(PEImage, /*ProbeExtensionResult*/, ProbeExtensionResult, cdac_data<PEImage>::ProbeExtensionResult)
CDAC_TYPE_END(PEImage)

CDAC_TYPE_BEGIN(PEImageLayout)
CDAC_TYPE_FIELD(PEImageLayout, /*pointer*/, Base, cdac_data<PEImageLayout>::Base)
CDAC_TYPE_FIELD(PEImageLayout, /*uint32*/, Size, cdac_data<PEImageLayout>::Size)
CDAC_TYPE_FIELD(PEImageLayout, /*uint32*/, Flags, cdac_data<PEImageLayout>::Flags)
CDAC_TYPE_END(PEImageLayout)

CDAC_TYPE_BEGIN(CGrowableSymbolStream)
CDAC_TYPE_INDETERMINATE(CGrowableSymbolStream)
CDAC_TYPE_FIELD(CGrowableSymbolStream, /*pointer*/, Buffer, cdac_data<CGrowableStream>::Buffer)
CDAC_TYPE_FIELD(CGrowableSymbolStream, /*uint32*/, Size, cdac_data<CGrowableStream>::Size)
CDAC_TYPE_END(CGrowableSymbolStream)

CDAC_TYPE_BEGIN(ProbeExtensionResult)
CDAC_TYPE_INDETERMINATE(ProbeExtensionResult)
CDAC_TYPE_FIELD(ProbeExtensionResult, /*int32*/, Type, offsetof(ProbeExtensionResult, Type))
CDAC_TYPE_END(ProbeExtensionResult)

CDAC_TYPE_BEGIN(AppDomain)
CDAC_TYPE_INDETERMINATE(AppDomain)
CDAC_TYPE_FIELD(AppDomain, /*pointer*/, RootAssembly, cdac_data<AppDomain>::RootAssembly)
CDAC_TYPE_FIELD(AppDomain, /*DomainAssemblyList*/, DomainAssemblyList, cdac_data<AppDomain>::DomainAssemblyList)
CDAC_TYPE_END(AppDomain)

CDAC_TYPE_BEGIN(ArrayListBase)
CDAC_TYPE_INDETERMINATE(ArrayListBase)
CDAC_TYPE_FIELD(ArrayListBase, /*uint32*/, Count, cdac_data<ArrayListBase>::Count)
CDAC_TYPE_FIELD(ArrayListBase, /*pointer*/, FirstBlock, cdac_data<ArrayListBase>::FirstBlock)
CDAC_TYPE_END(ArrayListBase)

CDAC_TYPE_BEGIN(ArrayListBlock)
CDAC_TYPE_INDETERMINATE(ArrayListBlock)
CDAC_TYPE_FIELD(ArrayListBlock, /*pointer*/, Next, cdac_data<ArrayListBase>::Next)
CDAC_TYPE_FIELD(ArrayListBlock, /*uint32*/, Size, cdac_data<ArrayListBase>::Size)
CDAC_TYPE_FIELD(ArrayListBlock, /*pointer*/, ArrayStart, cdac_data<ArrayListBase>::ArrayStart)
CDAC_TYPE_END(ArrayListBlock)

// RuntimeTypeSystem

CDAC_TYPE_BEGIN(MethodTable)
CDAC_TYPE_SIZE(sizeof(MethodTable))
CDAC_TYPE_FIELD(MethodTable, /*uint32*/, MTFlags, cdac_data<MethodTable>::MTFlags)
CDAC_TYPE_FIELD(MethodTable, /*uint32*/, BaseSize, cdac_data<MethodTable>::BaseSize)
CDAC_TYPE_FIELD(MethodTable, /*uint32*/, MTFlags2, cdac_data<MethodTable>::MTFlags2)
CDAC_TYPE_FIELD(MethodTable, /*nuint*/, EEClassOrCanonMT, cdac_data<MethodTable>::EEClassOrCanonMT)
CDAC_TYPE_FIELD(MethodTable, /*pointer*/, Module, cdac_data<MethodTable>::Module)
CDAC_TYPE_FIELD(MethodTable, /*pointer*/, ParentMethodTable, cdac_data<MethodTable>::ParentMethodTable)
CDAC_TYPE_FIELD(MethodTable, /*uint16*/, NumInterfaces, cdac_data<MethodTable>::NumInterfaces)
CDAC_TYPE_FIELD(MethodTable, /*uint16*/, NumVirtuals, cdac_data<MethodTable>::NumVirtuals)
CDAC_TYPE_FIELD(MethodTable, /*pointer*/, PerInstInfo, cdac_data<MethodTable>::PerInstInfo)
CDAC_TYPE_FIELD(MethodTable, /*pointer*/, AuxiliaryData, cdac_data<MethodTable>::AuxiliaryData)
CDAC_TYPE_END(MethodTable)

CDAC_TYPE_BEGIN(MethodTableAuxiliaryData)
CDAC_TYPE_INDETERMINATE(MethodTableAuxiliaryData)
CDAC_TYPE_FIELD(MethodTableAuxiliaryData, /*pointer*/, LoaderModule, offsetof(MethodTableAuxiliaryData, m_pLoaderModule))
CDAC_TYPE_FIELD(MethodTableAuxiliaryData, /*int16*/, OffsetToNonVirtualSlots, offsetof(MethodTableAuxiliaryData, m_offsetToNonVirtualSlots))
CDAC_TYPE_END(MethodTableAuxiliaryData)

CDAC_TYPE_BEGIN(EEClass)
CDAC_TYPE_INDETERMINATE(EEClass)
CDAC_TYPE_FIELD(EEClass, /*pointer*/, MethodTable, cdac_data<EEClass>::MethodTable)
CDAC_TYPE_FIELD(EEClass, /*uint16*/, NumMethods, cdac_data<EEClass>::NumMethods)
CDAC_TYPE_FIELD(EEClass, /*uint32*/, CorTypeAttr, cdac_data<EEClass>::CorTypeAttr)
CDAC_TYPE_FIELD(EEClass, /*uint8*/, InternalCorElementType, cdac_data<EEClass>::InternalCorElementType)
CDAC_TYPE_FIELD(EEClass, /*uint16*/, NumNonVirtualSlots, cdac_data<EEClass>::NumNonVirtualSlots)
CDAC_TYPE_END(EEClass)

CDAC_TYPE_BEGIN(ArrayClass)
CDAC_TYPE_INDETERMINATE(ArrayClass)
CDAC_TYPE_FIELD(ArrayClass, /*uint8*/, Rank, cdac_data<ArrayClass>::Rank)
CDAC_TYPE_END(ArrayClass)

CDAC_TYPE_BEGIN(GenericsDictInfo)
CDAC_TYPE_INDETERMINATE(GenericsDictInfo)
CDAC_TYPE_FIELD(GenericsDictInfo, /*uint16*/, NumDicts, offsetof(GenericsDictInfo, m_wNumDicts))
CDAC_TYPE_FIELD(GenericsDictInfo, /*uint16*/, NumTypeArgs, offsetof(GenericsDictInfo, m_wNumTyPars))
CDAC_TYPE_END(GenericsDictInfo)

CDAC_TYPE_BEGIN(TypeDesc)
CDAC_TYPE_INDETERMINATE(TypeDesc)
CDAC_TYPE_FIELD(TypeDesc, /*uint32*/, TypeAndFlags, cdac_data<TypeDesc>::TypeAndFlags)
CDAC_TYPE_END(TypeDesc)

CDAC_TYPE_BEGIN(ParamTypeDesc)
CDAC_TYPE_INDETERMINATE(ParamTypeDesc)
CDAC_TYPE_FIELD(ParamTypeDesc, /*pointer*/, TypeArg, cdac_data<ParamTypeDesc>::TypeArg)
CDAC_TYPE_END(ParamTypeDesc)

CDAC_TYPE_BEGIN(TypeVarTypeDesc)
CDAC_TYPE_INDETERMINATE(TypeVarTypeDesc)
CDAC_TYPE_FIELD(TypeVarTypeDesc, /*pointer*/, Module, cdac_data<TypeVarTypeDesc>::Module)
CDAC_TYPE_FIELD(TypeVarTypeDesc, /*uint32*/, Token, cdac_data<TypeVarTypeDesc>::Token)
CDAC_TYPE_END(TypeVarTypeDesc)

CDAC_TYPE_BEGIN(FnPtrTypeDesc)
CDAC_TYPE_INDETERMINATE(FnPtrTypeDesc)
CDAC_TYPE_FIELD(FnPtrTypeDesc, /*uint32*/, NumArgs, cdac_data<FnPtrTypeDesc>::NumArgs)
CDAC_TYPE_FIELD(FnPtrTypeDesc, /*uint32*/, CallConv, cdac_data<FnPtrTypeDesc>::CallConv)
CDAC_TYPE_FIELD(FnPtrTypeDesc, /*uint32*/, RetAndArgTypes, cdac_data<FnPtrTypeDesc>::RetAndArgTypes)
CDAC_TYPE_FIELD(FnPtrTypeDesc, /*pointer*/, LoaderModule, cdac_data<FnPtrTypeDesc>::LoaderModule)
CDAC_TYPE_END(FnPtrTypeDesc)

CDAC_TYPE_BEGIN(DynamicMetadata)
CDAC_TYPE_FIELD(DynamicMetadata, /*uint32*/, Size, cdac_data<DynamicMetadata>::Size)
CDAC_TYPE_FIELD(DynamicMetadata, /*inline byte array*/, Data, cdac_data<DynamicMetadata>::Data)
CDAC_TYPE_END(DynamicMetadata)

#ifdef STRESS_LOG
CDAC_TYPE_BEGIN(StressLog)
CDAC_TYPE_SIZE(sizeof(StressLog))
CDAC_TYPE_FIELD(StressLog, /* uint32 */, LoggedFacilities, cdac_offsets<StressLog>::facilitiesToLog)
CDAC_TYPE_FIELD(StressLog, /* uint32 */, Level, cdac_offsets<StressLog>::levelToLog)
CDAC_TYPE_FIELD(StressLog, /* uint32 */, MaxSizePerThread, cdac_offsets<StressLog>::MaxSizePerThread)
CDAC_TYPE_FIELD(StressLog, /* uint32 */, MaxSizeTotal, cdac_offsets<StressLog>::MaxSizeTotal)
CDAC_TYPE_FIELD(StressLog, /* uint32 */, TotalChunks, cdac_offsets<StressLog>::totalChunk)
CDAC_TYPE_FIELD(StressLog, /* pointer */, Logs, cdac_offsets<StressLog>::logs)
CDAC_TYPE_FIELD(StressLog, /* uint64 */, TickFrequency, cdac_offsets<StressLog>::tickFrequency)
CDAC_TYPE_FIELD(StressLog, /* uint64 */, StartTimestamp, cdac_offsets<StressLog>::startTimeStamp)
CDAC_TYPE_FIELD(StressLog, /* nuint */, ModuleOffset, cdac_offsets<StressLog>::moduleOffset)
CDAC_TYPE_END(StressLog)

CDAC_TYPE_BEGIN(StressLogModuleDesc)
CDAC_TYPE_SIZE(cdac_offsets<StressLog>::ModuleDesc::type_size)
CDAC_TYPE_FIELD(StressLogModuleDesc, pointer, BaseAddress, cdac_offsets<StressLog>::ModuleDesc::baseAddress)
CDAC_TYPE_FIELD(StressLogModuleDesc, nuint, Size, cdac_offsets<StressLog>::ModuleDesc::size)
CDAC_TYPE_END(StressLogModuleDesc)

CDAC_TYPE_BEGIN(ThreadStressLog)
CDAC_TYPE_INDETERMINATE(ThreadStressLog)
CDAC_TYPE_FIELD(ThreadStressLog, /* pointer */, Next, cdac_offsets<ThreadStressLog>::next)
CDAC_TYPE_FIELD(ThreadStressLog, uint64, ThreadId, cdac_offsets<ThreadStressLog>::threadId)
CDAC_TYPE_FIELD(ThreadStressLog, uint8, WriteHasWrapped, cdac_offsets<ThreadStressLog>::writeHasWrapped)
CDAC_TYPE_FIELD(ThreadStressLog, pointer, CurrentPtr, cdac_offsets<ThreadStressLog>::curPtr)
CDAC_TYPE_FIELD(ThreadStressLog, /* pointer */, ChunkListHead, cdac_offsets<ThreadStressLog>::chunkListHead)
CDAC_TYPE_FIELD(ThreadStressLog, /* pointer */, ChunkListTail, cdac_offsets<ThreadStressLog>::chunkListTail)
CDAC_TYPE_FIELD(ThreadStressLog, /* pointer */, CurrentWriteChunk, cdac_offsets<ThreadStressLog>::curWriteChunk)
CDAC_TYPE_END(ThreadStressLog)

CDAC_TYPE_BEGIN(StressLogChunk)
CDAC_TYPE_SIZE(sizeof(StressLogChunk))
CDAC_TYPE_FIELD(StressLogChunk, /* pointer */, Prev, offsetof(StressLogChunk, prev))
CDAC_TYPE_FIELD(StressLogChunk, /* pointer */, Next, offsetof(StressLogChunk, next))
CDAC_TYPE_FIELD(StressLogChunk, /* uint8[STRESSLOG_CHUNK_SIZE] */, Buf, offsetof(StressLogChunk, buf))
CDAC_TYPE_FIELD(StressLogChunk, /* uint32 */, Sig1, offsetof(StressLogChunk, dwSig1))
CDAC_TYPE_FIELD(StressLogChunk, /* uint32 */, Sig2, offsetof(StressLogChunk, dwSig2))
CDAC_TYPE_END(StressLogChunk)

// The StressMsg Header is the fixed size portion of the StressMsg
CDAC_TYPE_BEGIN(StressMsgHeader)
CDAC_TYPE_SIZE(sizeof(StressMsg))
CDAC_TYPE_END(StressMsgHeader)

CDAC_TYPE_BEGIN(StressMsg)
CDAC_TYPE_INDETERMINATE(StressMsg)
CDAC_TYPE_FIELD(StressMsg, StressMsgHeader, Header, 0)
CDAC_TYPE_FIELD(StressMsg, /* pointer */, Args, offsetof(StressMsg, args))
CDAC_TYPE_END(StressMsg)
#endif

CDAC_TYPE_BEGIN(MethodDesc)
CDAC_TYPE_SIZE(sizeof(MethodDesc))
CDAC_TYPE_FIELD(MethodDesc, /*uint8*/, ChunkIndex, cdac_data<MethodDesc>::ChunkIndex)
CDAC_TYPE_FIELD(MethodDesc, /*uint16*/, Slot, cdac_data<MethodDesc>::Slot)
CDAC_TYPE_FIELD(MethodDesc, /*uint16*/, Flags, cdac_data<MethodDesc>::Flags)
CDAC_TYPE_FIELD(MethodDesc, /*uint16*/, Flags3AndTokenRemainder, cdac_data<MethodDesc>::Flags3AndTokenRemainder)
CDAC_TYPE_FIELD(MethodDesc, /*uint8*/, EntryPointFlags, cdac_data<MethodDesc>::EntryPointFlags)
CDAC_TYPE_FIELD(MethodDesc, /*pointer*/, CodeData, cdac_data<MethodDesc>::CodeData)
#ifdef HAVE_GCCOVER
CDAC_TYPE_FIELD(MethodDesc, /*pointer*/, GCCoverageInfo, offsetof(MethodDesc, m_GcCover))
#endif // HAVE_GCCOVER
CDAC_TYPE_END(MethodDesc)

CDAC_TYPE_BEGIN(MethodDescChunk)
CDAC_TYPE_SIZE(sizeof(MethodDescChunk))
CDAC_TYPE_FIELD(MethodDescChunk, /*pointer*/, MethodTable, cdac_data<MethodDescChunk>::MethodTable)
CDAC_TYPE_FIELD(MethodDescChunk, /*pointer*/, Next, cdac_data<MethodDescChunk>::Next)
CDAC_TYPE_FIELD(MethodDescChunk, /*uint8*/, Size, cdac_data<MethodDescChunk>::Size)
CDAC_TYPE_FIELD(MethodDescChunk, /*uint8*/, Count, cdac_data<MethodDescChunk>::Count)
CDAC_TYPE_FIELD(MethodDescChunk, /*uint16*/, FlagsAndTokenRange, cdac_data<MethodDescChunk>::FlagsAndTokenRange)
CDAC_TYPE_END(MethodDescChunk)

CDAC_TYPE_BEGIN(NonVtableSlot)
CDAC_TYPE_SIZE(sizeof(MethodDesc::NonVtableSlot))
CDAC_TYPE_END(NonVtableSlot)

CDAC_TYPE_BEGIN(MethodImpl)
CDAC_TYPE_SIZE(sizeof(MethodImpl))
CDAC_TYPE_END(MethodImpl)

CDAC_TYPE_BEGIN(NativeCodeSlot)
CDAC_TYPE_SIZE(sizeof(MethodDesc::NativeCodeSlot))
CDAC_TYPE_END(NativeCodeSlot)

CDAC_TYPE_BEGIN(InstantiatedMethodDesc)
CDAC_TYPE_SIZE(sizeof(InstantiatedMethodDesc))
CDAC_TYPE_FIELD(InstantiatedMethodDesc, /*pointer*/, PerInstInfo, cdac_data<InstantiatedMethodDesc>::PerInstInfo)
CDAC_TYPE_FIELD(InstantiatedMethodDesc, /*uint16*/, Flags2, cdac_data<InstantiatedMethodDesc>::Flags2)
CDAC_TYPE_FIELD(InstantiatedMethodDesc, /*uint16*/, NumGenericArgs, cdac_data<InstantiatedMethodDesc>::NumGenericArgs)
CDAC_TYPE_END(InstantiatedMethodDesc)

CDAC_TYPE_BEGIN(StoredSigMethodDesc)
CDAC_TYPE_INDETERMINATE(StoredSigMethodDesc)
CDAC_TYPE_FIELD(StoredSigMethodDesc, /*pointer*/, Sig, cdac_data<StoredSigMethodDesc>::Sig)
CDAC_TYPE_FIELD(StoredSigMethodDesc, /*uint32*/, cSig, cdac_data<StoredSigMethodDesc>::cSig)
CDAC_TYPE_FIELD(StoredSigMethodDesc, /*uint32*/, ExtendedFlags, cdac_data<StoredSigMethodDesc>::ExtendedFlags)
CDAC_TYPE_END(StoredSigMethodDesc)

CDAC_TYPE_BEGIN(DynamicMethodDesc)
CDAC_TYPE_SIZE(sizeof(DynamicMethodDesc))
CDAC_TYPE_FIELD(DynamicMethodDesc, /*pointer*/, MethodName, cdac_data<DynamicMethodDesc>::MethodName)
CDAC_TYPE_END(DynamicMethodDesc)

CDAC_TYPE_BEGIN(ArrayMethodDesc)
CDAC_TYPE_SIZE(sizeof(ArrayMethodDesc))
CDAC_TYPE_END(ArrayMethodDesc)

CDAC_TYPE_BEGIN(FCallMethodDesc)
CDAC_TYPE_SIZE(sizeof(FCallMethodDesc))
CDAC_TYPE_END(FCallMethodDesc)

CDAC_TYPE_BEGIN(PInvokeMethodDesc)
CDAC_TYPE_SIZE(sizeof(NDirectMethodDesc))
CDAC_TYPE_END(PInvokeMethodDesc)

CDAC_TYPE_BEGIN(EEImplMethodDesc)
CDAC_TYPE_SIZE(sizeof(EEImplMethodDesc))
CDAC_TYPE_END(EEImplMethodDesc)

#ifdef FEATURE_COMINTEROP
CDAC_TYPE_BEGIN(CLRToCOMCallMethodDesc)
CDAC_TYPE_SIZE(sizeof(CLRToCOMCallMethodDesc))
CDAC_TYPE_END(CLRToCOMCallMethodDesc)
#endif // FEATURE_COMINTEROP

CDAC_TYPE_BEGIN(CodePointer)
CDAC_TYPE_SIZE(sizeof(PCODE))
CDAC_TYPE_END(CodePointer)

CDAC_TYPE_BEGIN(MethodDescCodeData)
CDAC_TYPE_INDETERMINATE(MethodDescCodeData)
CDAC_TYPE_FIELD(MethodDescCodeData, /*CodePointer*/, TemporaryEntryPoint, offsetof(MethodDescCodeData,TemporaryEntryPoint))
CDAC_TYPE_FIELD(MethodDescCodeData, /*pointer*/, VersioningState, offsetof(MethodDescCodeData,VersioningState))
CDAC_TYPE_END(MethodDescCodeData)

CDAC_TYPE_BEGIN(MethodDescVersioningState)
CDAC_TYPE_INDETERMINATE(MethodDescVersioningState)
CDAC_TYPE_FIELD(MethodDescVersioningState, /*pointer*/, NativeCodeVersionNode, cdac_data<MethodDescVersioningState>::NativeCodeVersionNode)
CDAC_TYPE_FIELD(MethodDescVersioningState, /*uint8*/, Flags, cdac_data<MethodDescVersioningState>::Flags)
CDAC_TYPE_END(MethodDescVersioningState)

CDAC_TYPE_BEGIN(PrecodeMachineDescriptor)
CDAC_TYPE_INDETERMINATE(PrecodeMachineDescriptor)
CDAC_TYPE_FIELD(PrecodeMachineDescriptor, /*uint8*/, InvalidPrecodeType, offsetof(PrecodeMachineDescriptor, InvalidPrecodeType))
#ifdef HAS_NDIRECT_IMPORT_PRECODE
CDAC_TYPE_FIELD(PrecodeMachineDescriptor, /*uint8*/, PInvokeImportPrecodeType, offsetof(PrecodeMachineDescriptor, PInvokeImportPrecodeType))
#endif

#ifdef HAS_FIXUP_PRECODE
CDAC_TYPE_FIELD(PrecodeMachineDescriptor, /*uint8*/, FixupPrecodeType, offsetof(PrecodeMachineDescriptor, FixupPrecodeType))
CDAC_TYPE_FIELD(PrecodeMachineDescriptor, /*uint8*/, FixupCodeOffset, offsetof(PrecodeMachineDescriptor, FixupCodeOffset))
CDAC_TYPE_FIELD(PrecodeMachineDescriptor, /*uint8*/, FixupStubPrecodeSize, offsetof(PrecodeMachineDescriptor, FixupStubPrecodeSize))
CDAC_TYPE_FIELD(PrecodeMachineDescriptor, /*byte[]*/, FixupBytes, offsetof(PrecodeMachineDescriptor, FixupBytes))
CDAC_TYPE_FIELD(PrecodeMachineDescriptor, /*byte[]*/, FixupIgnoredBytes, offsetof(PrecodeMachineDescriptor, FixupIgnoredBytes))
#endif // HAS_FIXUP_PRECODE

CDAC_TYPE_FIELD(PrecodeMachineDescriptor, /*uint8*/, StubPrecodeSize, offsetof(PrecodeMachineDescriptor, StubPrecodeSize))
CDAC_TYPE_FIELD(PrecodeMachineDescriptor, /*uint8*/, StubPrecodeType, offsetof(PrecodeMachineDescriptor, StubPrecodeType))

CDAC_TYPE_FIELD(PrecodeMachineDescriptor, /*byte[]*/, StubBytes, offsetof(PrecodeMachineDescriptor, StubBytes))
CDAC_TYPE_FIELD(PrecodeMachineDescriptor, /*byte[]*/, StubIgnoredBytes, offsetof(PrecodeMachineDescriptor, StubIgnoredBytes))

#ifdef HAS_THISPTR_RETBUF_PRECODE
CDAC_TYPE_FIELD(PrecodeMachineDescriptor, /*uint8*/, ThisPointerRetBufPrecodeType, offsetof(PrecodeMachineDescriptor, ThisPointerRetBufPrecodeType))
#endif
#ifdef FEATURE_INTERPRETER
CDAC_TYPE_FIELD(PrecodeMachineDescriptor, /*uint8*/, InterpreterPrecodeType, offsetof(PrecodeMachineDescriptor, InterpreterPrecodeType))
#endif
#ifdef FEATURE_STUBPRECODE_DYNAMIC_HELPERS
CDAC_TYPE_FIELD(PrecodeMachineDescriptor, /*uint8*/, DynamicHelperPrecodeType, offsetof(PrecodeMachineDescriptor, DynamicHelperPrecodeType))
#endif
CDAC_TYPE_FIELD(PrecodeMachineDescriptor, /*uint8*/, UMEntryPrecodeType, offsetof(PrecodeMachineDescriptor, UMEntryPrecodeType))
CDAC_TYPE_FIELD(PrecodeMachineDescriptor, /*uint32*/, StubCodePageSize, offsetof(PrecodeMachineDescriptor, StubCodePageSize))
CDAC_TYPE_END(PrecodeMachineDescriptor)

CDAC_TYPE_BEGIN(PlatformMetadata)
CDAC_TYPE_INDETERMINATE(PlatformMetadata)
CDAC_TYPE_FIELD(PlatformMetadata, /*PrecodeMachineDescriptor*/, PrecodeMachineDescriptor, offsetof(CDacPlatformMetadata, precode))
CDAC_TYPE_FIELD(PlatformMetadata, /*uint8*/, CodePointerFlags, offsetof(CDacPlatformMetadata, codePointerFlags))
CDAC_TYPE_END(PlatformMetadata)

CDAC_TYPE_BEGIN(StubPrecodeData)
CDAC_TYPE_INDETERMINATE(StubPrecodeData)
CDAC_TYPE_FIELD(StubPrecodeData, /*pointer*/, SecretParam, offsetof(StubPrecodeData, SecretParam))
CDAC_TYPE_FIELD(StubPrecodeData, /*uint8*/, Type, offsetof(StubPrecodeData, Type))
CDAC_TYPE_END(StubPrecodeData)

#ifdef HAS_THISPTR_RETBUF_PRECODE
CDAC_TYPE_BEGIN(ThisPtrRetBufPrecodeData)
CDAC_TYPE_INDETERMINATE(ThisPtrRetBufPrecodeData)
CDAC_TYPE_FIELD(ThisPtrRetBufPrecodeData, /*pointer*/, MethodDesc, offsetof(ThisPtrRetBufPrecodeData, MethodDesc))
CDAC_TYPE_END(ThisPtrRetBufPrecodeData)
#endif

CDAC_TYPE_BEGIN(FixupPrecodeData)
CDAC_TYPE_INDETERMINATE(FixupPrecodeData)
CDAC_TYPE_FIELD(FixupPrecodeData, /*pointer*/, MethodDesc, offsetof(FixupPrecodeData, MethodDesc))
CDAC_TYPE_END(FixupPrecodeData)

CDAC_TYPE_BEGIN(ReadyToRunInfo)
CDAC_TYPE_INDETERMINATE(ReadyToRunInfo)
CDAC_TYPE_FIELD(ReadyToRunInfo, /*pointer*/, ReadyToRunHeader, cdac_data<ReadyToRunInfo>::ReadyToRunHeader)
CDAC_TYPE_FIELD(ReadyToRunInfo, /*pointer*/, CompositeInfo, cdac_data<ReadyToRunInfo>::CompositeInfo)
CDAC_TYPE_FIELD(ReadyToRunInfo, /*uint32*/, NumRuntimeFunctions, cdac_data<ReadyToRunInfo>::NumRuntimeFunctions)
CDAC_TYPE_FIELD(ReadyToRunInfo, /*pointer*/, RuntimeFunctions, cdac_data<ReadyToRunInfo>::RuntimeFunctions)
CDAC_TYPE_FIELD(ReadyToRunInfo, /*uint32*/, NumHotColdMap, cdac_data<ReadyToRunInfo>::NumHotColdMap)
CDAC_TYPE_FIELD(ReadyToRunInfo, /*pointer*/, HotColdMap, cdac_data<ReadyToRunInfo>::HotColdMap)
CDAC_TYPE_FIELD(ReadyToRunInfo, /*pointer*/, DelayLoadMethodCallThunks, cdac_data<ReadyToRunInfo>::DelayLoadMethodCallThunks)
CDAC_TYPE_FIELD(ReadyToRunInfo, /*HashMap*/, EntryPointToMethodDescMap, cdac_data<ReadyToRunInfo>::EntryPointToMethodDescMap)
CDAC_TYPE_END(ReadyToRunInfo)

CDAC_TYPE_BEGIN(ReadyToRunHeader)
CDAC_TYPE_INDETERMINATE(READYTORUN_HEADER)
CDAC_TYPE_FIELD(ReadyToRunHeader, /*uint16*/, MajorVersion, offsetof(READYTORUN_HEADER, MajorVersion))
CDAC_TYPE_FIELD(ReadyToRunHeader, /*uint16*/, MinorVersion, offsetof(READYTORUN_HEADER, MinorVersion))
CDAC_TYPE_END(ReadyToRunHeader)

CDAC_TYPE_BEGIN(ImageDataDirectory)
CDAC_TYPE_SIZE(sizeof(IMAGE_DATA_DIRECTORY))
CDAC_TYPE_FIELD(ImageDataDirectory, /*uint32*/, VirtualAddress, offsetof(IMAGE_DATA_DIRECTORY, VirtualAddress))
CDAC_TYPE_FIELD(ImageDataDirectory, /*uint32*/, Size, offsetof(IMAGE_DATA_DIRECTORY, Size))
CDAC_TYPE_END(ImageDataDirectory)

CDAC_TYPE_BEGIN(RuntimeFunction)
CDAC_TYPE_SIZE(sizeof(RUNTIME_FUNCTION))
CDAC_TYPE_FIELD(RuntimeFunction, /*uint32*/, BeginAddress, offsetof(RUNTIME_FUNCTION, BeginAddress))
#ifdef TARGET_AMD64
CDAC_TYPE_FIELD(RuntimeFunction, /*uint32*/, EndAddress, offsetof(RUNTIME_FUNCTION, EndAddress))
#endif
CDAC_TYPE_FIELD(RuntimeFunction, /*uint32*/, UnwindData, offsetof(RUNTIME_FUNCTION, UnwindData))
CDAC_TYPE_END(RuntimeFunction)

CDAC_TYPE_BEGIN(UnwindInfo)
CDAC_TYPE_INDETERMINATE(UnwindInfo)
#ifdef TARGET_X86
CDAC_TYPE_FIELD(UnwindInfo, /*uint32*/, FunctionLength, offsetof(UNWIND_INFO, FunctionLength))
#endif
CDAC_TYPE_END(UnwindInfo)

CDAC_TYPE_BEGIN(HashMap)
CDAC_TYPE_INDETERMINATE(HashMap)
CDAC_TYPE_FIELD(HashMap, /*pointer*/, Buckets, cdac_data<HashMap>::Buckets)
CDAC_TYPE_END(HashMap)

CDAC_TYPE_BEGIN(Bucket)
CDAC_TYPE_SIZE(sizeof(Bucket))
CDAC_TYPE_FIELD(Bucket, /*pointer*/, Keys, offsetof(Bucket, m_rgKeys))
CDAC_TYPE_FIELD(Bucket, /*pointer*/, Values, offsetof(Bucket, m_rgValues))
CDAC_TYPE_END(Bucket)

CDAC_TYPE_BEGIN(RangeSectionMap)
CDAC_TYPE_INDETERMINATE(RangeSectionMap)
CDAC_TYPE_FIELD(RangeSectionMap, /*pointer*/, TopLevelData, cdac_data<RangeSectionMap>::TopLevelData)
CDAC_TYPE_END(RangeSectionMap)

CDAC_TYPE_BEGIN(RangeSectionFragment)
CDAC_TYPE_INDETERMINATE(RangeSectionFragment)
CDAC_TYPE_FIELD(RangeSectionFragment, /*pointer*/, RangeBegin, cdac_data<RangeSectionMap>::RangeSectionFragment::RangeBegin)
CDAC_TYPE_FIELD(RangeSectionFragment, /*pointer*/, RangeEndOpen, cdac_data<RangeSectionMap>::RangeSectionFragment::RangeEndOpen)
CDAC_TYPE_FIELD(RangeSectionFragment, /*pointer*/, RangeSection, cdac_data<RangeSectionMap>::RangeSectionFragment::RangeSection)
CDAC_TYPE_FIELD(RangeSectionFragment, /*pointer*/, Next, cdac_data<RangeSectionMap>::RangeSectionFragment::Next)
CDAC_TYPE_END(RangeSectionFragment)

CDAC_TYPE_BEGIN(RangeSection)
CDAC_TYPE_INDETERMINATE(RangeSection)
CDAC_TYPE_FIELD(RangeSection, /*pointer*/, RangeBegin, cdac_data<RangeSection>::RangeBegin)
CDAC_TYPE_FIELD(RangeSection, /*pointer*/, RangeEndOpen, cdac_data<RangeSection>::RangeEndOpen)
CDAC_TYPE_FIELD(RangeSection, /*pointer*/, NextForDelete, cdac_data<RangeSection>::NextForDelete)
CDAC_TYPE_FIELD(RangeSection, /*pointer*/, JitManager, cdac_data<RangeSection>::JitManager)
CDAC_TYPE_FIELD(RangeSection, /*int32_t*/, Flags, cdac_data<RangeSection>::Flags)
CDAC_TYPE_FIELD(RangeSection, /*pointer*/, HeapList, cdac_data<RangeSection>::HeapList)
CDAC_TYPE_FIELD(RangeSection, /*pointer*/, R2RModule, cdac_data<RangeSection>::R2RModule)
CDAC_TYPE_END(RangeSection)

CDAC_TYPE_BEGIN(RealCodeHeader)
CDAC_TYPE_INDETERMINATE(RealCodeHeader)
CDAC_TYPE_FIELD(RealCodeHeader, /*pointer*/, MethodDesc, offsetof(RealCodeHeader, phdrMDesc))
CDAC_TYPE_FIELD(RealCodeHeader, /*pointer*/, GCInfo, offsetof(RealCodeHeader, phdrJitGCInfo))
#ifdef FEATURE_EH_FUNCLETS
CDAC_TYPE_FIELD(RealCodeHeader, /*uint32*/, NumUnwindInfos, offsetof(RealCodeHeader, nUnwindInfos))
CDAC_TYPE_FIELD(RealCodeHeader, /* T_RUNTIME_FUNCTION */, UnwindInfos, offsetof(RealCodeHeader, unwindInfos))
#endif // FEATURE_EH_FUNCLETS
CDAC_TYPE_END(RealCodeHeader)

CDAC_TYPE_BEGIN(CodeHeapListNode)
CDAC_TYPE_FIELD(CodeHeapListNode, /*pointer*/, Next, offsetof(HeapList, hpNext))
CDAC_TYPE_FIELD(CodeHeapListNode, /*pointer*/, StartAddress, offsetof(HeapList, startAddress))
CDAC_TYPE_FIELD(CodeHeapListNode, /*pointer*/, EndAddress, offsetof(HeapList, endAddress))
CDAC_TYPE_FIELD(CodeHeapListNode, /*pointer*/, MapBase, offsetof(HeapList, mapBase))
CDAC_TYPE_FIELD(CodeHeapListNode, /*pointer*/, HeaderMap, offsetof(HeapList, pHdrMap))
CDAC_TYPE_END(CodeHeapListNode)

CDAC_TYPE_BEGIN(ILCodeVersioningState)
CDAC_TYPE_INDETERMINATE(ILCodeVersioningState)
CDAC_TYPE_FIELD(ILCodeVersioningState, /*pointer*/, FirstVersionNode, cdac_data<ILCodeVersioningState>::FirstVersionNode)
CDAC_TYPE_FIELD(ILCodeVersioningState, /*uint32*/, ActiveVersionKind, cdac_data<ILCodeVersioningState>::ActiveVersionKind)
CDAC_TYPE_FIELD(ILCodeVersioningState, /*pointer*/, ActiveVersionNode, cdac_data<ILCodeVersioningState>::ActiveVersionNode)
CDAC_TYPE_FIELD(ILCodeVersioningState, /*pointer*/, ActiveVersionModule, cdac_data<ILCodeVersioningState>::ActiveVersionModule)
CDAC_TYPE_FIELD(ILCodeVersioningState, /*uint32*/, ActiveVersionMethodDef, cdac_data<ILCodeVersioningState>::ActiveVersionMethodDef)
CDAC_TYPE_END(ILCodeVersioningState)

CDAC_TYPE_BEGIN(NativeCodeVersionNode)
CDAC_TYPE_INDETERMINATE(NativeCodeVersionNode)
CDAC_TYPE_FIELD(NativeCodeVersionNode, /*pointer*/, Next, cdac_data<NativeCodeVersionNode>::Next)
CDAC_TYPE_FIELD(NativeCodeVersionNode, /*pointer*/, MethodDesc, cdac_data<NativeCodeVersionNode>::MethodDesc)
CDAC_TYPE_FIELD(NativeCodeVersionNode, /*pointer*/, NativeCode, cdac_data<NativeCodeVersionNode>::NativeCode)
CDAC_TYPE_FIELD(NativeCodeVersionNode, /*uint32*/, Flags, cdac_data<NativeCodeVersionNode>::Flags)
CDAC_TYPE_FIELD(NativeCodeVersionNode, /*nuint*/, ILVersionId, cdac_data<NativeCodeVersionNode>::ILVersionId)
#ifdef HAVE_GCCOVER
CDAC_TYPE_FIELD(NativeCodeVersionNode, /*pointer*/, GCCoverageInfo, cdac_data<NativeCodeVersionNode>::GCCoverageInfo)
#endif // HAVE_GCCOVER
CDAC_TYPE_END(NativeCodeVersionNode)

CDAC_TYPE_BEGIN(ILCodeVersionNode)
CDAC_TYPE_INDETERMINATE(ILCodeVersionNode)
CDAC_TYPE_FIELD(ILCodeVersionNode, /*nuint*/, VersionId, cdac_data<ILCodeVersionNode>::VersionId)
CDAC_TYPE_FIELD(ILCodeVersionNode, /*pointer*/, Next, cdac_data<ILCodeVersionNode>::Next)
CDAC_TYPE_FIELD(ILCodeVersionNode, /*uint32*/, RejitState, cdac_data<ILCodeVersionNode>::RejitState)
CDAC_TYPE_END(ILCodeVersionNode)

CDAC_TYPE_BEGIN(ProfControlBlock)
CDAC_TYPE_FIELD(ProfControlBlock, /*uint64*/, GlobalEventMask, offsetof(ProfControlBlock, globalEventMask))
CDAC_TYPE_END(ProfControlBlock)

#ifdef HAVE_GCCOVER
CDAC_TYPE_BEGIN(GCCoverageInfo)
CDAC_TYPE_INDETERMINATE(GCCoverageInfo)
CDAC_TYPE_FIELD(GCCoverageInfo, /*pointer*/, SavedCode, offsetof(GCCoverageInfo, savedCode))
CDAC_TYPE_END(GCCoverageInfo)
#endif // HAVE_GCCOVER

CDAC_TYPE_BEGIN(Frame)
CDAC_TYPE_INDETERMINATE(Frame)
CDAC_TYPE_FIELD(Frame, /*pointer*/, Next, cdac_data<Frame>::Next)
CDAC_TYPE_END(Frame)

CDAC_TYPE_BEGIN(InlinedCallFrame)
CDAC_TYPE_SIZE(sizeof(InlinedCallFrame))
CDAC_TYPE_FIELD(InlinedCallFrame, /*pointer*/, CallSiteSP, offsetof(InlinedCallFrame, m_pCallSiteSP))
CDAC_TYPE_FIELD(InlinedCallFrame, /*pointer*/, CallerReturnAddress, offsetof(InlinedCallFrame, m_pCallerReturnAddress))
CDAC_TYPE_FIELD(InlinedCallFrame, /*pointer*/, CalleeSavedFP, offsetof(InlinedCallFrame, m_pCalleeSavedFP))
#ifdef TARGET_ARM
CDAC_TYPE_FIELD(InlinedCallFrame, /*pointer*/, SPAfterProlog, offsetof(InlinedCallFrame, m_pSPAfterProlog))
#endif // TARGET_ARM
CDAC_TYPE_END(InlinedCallFrame)

CDAC_TYPE_BEGIN(SoftwareExceptionFrame)
CDAC_TYPE_SIZE(sizeof(SoftwareExceptionFrame))
CDAC_TYPE_FIELD(SoftwareExceptionFrame, /*T_CONTEXT*/, TargetContext, cdac_data<SoftwareExceptionFrame>::TargetContext)
CDAC_TYPE_FIELD(SoftwareExceptionFrame, /*pointer*/, ReturnAddress, cdac_data<SoftwareExceptionFrame>::ReturnAddress)
CDAC_TYPE_END(SoftwareExceptionFrame)

CDAC_TYPE_BEGIN(FramedMethodFrame)
CDAC_TYPE_SIZE(sizeof(FramedMethodFrame))
CDAC_TYPE_FIELD(FramedMethodFrame, /*pointer*/, TransitionBlockPtr, cdac_data<FramedMethodFrame>::TransitionBlockPtr)
CDAC_TYPE_END(FramedMethodFrame)

CDAC_TYPE_BEGIN(TransitionBlock)
CDAC_TYPE_SIZE(sizeof(TransitionBlock))
CDAC_TYPE_FIELD(TransitionBlock, /*pointer*/, ReturnAddress, offsetof(TransitionBlock, m_ReturnAddress))
CDAC_TYPE_FIELD(TransitionBlock, /*CalleeSavedRegisters*/, CalleeSavedRegisters, offsetof(TransitionBlock, m_calleeSavedRegisters))
#ifdef TARGET_ARM
CDAC_TYPE_FIELD(TransitionBlock, /*ArgumentRegisters*/, ArgumentRegisters, offsetof(TransitionBlock, m_argumentRegisters))
#endif // TARGET_ARM
CDAC_TYPE_END(TransitionBlock)

#ifdef DEBUGGING_SUPPORTED
CDAC_TYPE_BEGIN(FuncEvalFrame)
CDAC_TYPE_SIZE(sizeof(FuncEvalFrame))
CDAC_TYPE_FIELD(FuncEvalFrame, /*pointer*/, DebuggerEvalPtr, cdac_data<FuncEvalFrame>::DebuggerEvalPtr)
CDAC_TYPE_END(FuncEvalFrame)

CDAC_TYPE_BEGIN(DebuggerEval)
CDAC_TYPE_SIZE(sizeof(DebuggerEval))
CDAC_TYPE_FIELD(DebuggerEval, /*T_CONTEXT*/, TargetContext, offsetof(DebuggerEval, m_context))
CDAC_TYPE_FIELD(DebuggerEval, /*bool*/, EvalDuringException, offsetof(DebuggerEval, m_evalDuringException))
CDAC_TYPE_END(DebuggerEval)
#endif // DEBUGGING_SUPPORTED

#ifdef FEATURE_HIJACK
CDAC_TYPE_BEGIN(ResumableFrame)
CDAC_TYPE_SIZE(sizeof(ResumableFrame))
CDAC_TYPE_FIELD(ResumableFrame, /*pointer*/, TargetContextPtr, cdac_data<ResumableFrame>::TargetContextPtr)
CDAC_TYPE_END(ResumableFrame)

CDAC_TYPE_BEGIN(HijackFrame)
CDAC_TYPE_SIZE(sizeof(HijackFrame))
CDAC_TYPE_FIELD(HijackFrame, /*pointer*/, ReturnAddress, cdac_data<HijackFrame>::ReturnAddress)
CDAC_TYPE_FIELD(HijackFrame, /*pointer*/, HijackArgsPtr, cdac_data<HijackFrame>::HijackArgsPtr)
CDAC_TYPE_END(HijackFrame)

// HijackArgs struct is different on each platform
CDAC_TYPE_BEGIN(HijackArgs)
CDAC_TYPE_SIZE(sizeof(HijackArgs))
#if defined(TARGET_AMD64)

CDAC_TYPE_FIELD(HijackArgs, /*CalleeSavedRegisters*/, CalleeSavedRegisters, offsetof(HijackArgs, Regs))
#ifdef TARGET_WINDOWS
CDAC_TYPE_FIELD(HijackArgs, /*pointer*/, Rsp, offsetof(HijackArgs, Rsp))
#endif // TARGET_WINDOWS

#elif defined(TARGET_ARM64)

CDAC_TYPE_FIELD(HijackArgs, /*pointer*/, X0, offsetof(HijackArgs, X0))
CDAC_TYPE_FIELD(HijackArgs, /*pointer*/, X1, offsetof(HijackArgs, X1))
CDAC_TYPE_FIELD(HijackArgs, /*pointer*/, X19, offsetof(HijackArgs, X19))
CDAC_TYPE_FIELD(HijackArgs, /*pointer*/, X20, offsetof(HijackArgs, X20))
CDAC_TYPE_FIELD(HijackArgs, /*pointer*/, X21, offsetof(HijackArgs, X21))
CDAC_TYPE_FIELD(HijackArgs, /*pointer*/, X22, offsetof(HijackArgs, X22))
CDAC_TYPE_FIELD(HijackArgs, /*pointer*/, X23, offsetof(HijackArgs, X23))
CDAC_TYPE_FIELD(HijackArgs, /*pointer*/, X24, offsetof(HijackArgs, X24))
CDAC_TYPE_FIELD(HijackArgs, /*pointer*/, X25, offsetof(HijackArgs, X25))
CDAC_TYPE_FIELD(HijackArgs, /*pointer*/, X26, offsetof(HijackArgs, X26))
CDAC_TYPE_FIELD(HijackArgs, /*pointer*/, X27, offsetof(HijackArgs, X27))
CDAC_TYPE_FIELD(HijackArgs, /*pointer*/, X28, offsetof(HijackArgs, X28))
CDAC_TYPE_FIELD(HijackArgs, /*pointer*/, Fp, offsetof(HijackArgs, X29))
CDAC_TYPE_FIELD(HijackArgs, /*pointer*/, Lr, offsetof(HijackArgs, Lr))

#elif defined(TARGET_X86)

CDAC_TYPE_FIELD(HijackArgs, /*pointer*/, Edi, offsetof(HijackArgs, Edi))
CDAC_TYPE_FIELD(HijackArgs, /*pointer*/, Esi, offsetof(HijackArgs, Esi))
CDAC_TYPE_FIELD(HijackArgs, /*pointer*/, Ebx, offsetof(HijackArgs, Ebx))
CDAC_TYPE_FIELD(HijackArgs, /*pointer*/, Edx, offsetof(HijackArgs, Edx))
CDAC_TYPE_FIELD(HijackArgs, /*pointer*/, Ecx, offsetof(HijackArgs, Ecx))
CDAC_TYPE_FIELD(HijackArgs, /*pointer*/, Eax, offsetof(HijackArgs, Eax))
CDAC_TYPE_FIELD(HijackArgs, /*pointer*/, Ebp, offsetof(HijackArgs, Ebp))
CDAC_TYPE_FIELD(HijackArgs, /*pointer*/, Eip, offsetof(HijackArgs, Eip))

#elif defined(TARGET_ARM)

CDAC_TYPE_FIELD(HijackArgs, /*pointer*/, R0, offsetof(HijackArgs, R0))
CDAC_TYPE_FIELD(HijackArgs, /*pointer*/, R2, offsetof(HijackArgs, R2))
CDAC_TYPE_FIELD(HijackArgs, /*pointer*/, R4, offsetof(HijackArgs, R4))
CDAC_TYPE_FIELD(HijackArgs, /*pointer*/, R5, offsetof(HijackArgs, R5))
CDAC_TYPE_FIELD(HijackArgs, /*pointer*/, R6, offsetof(HijackArgs, R6))
CDAC_TYPE_FIELD(HijackArgs, /*pointer*/, R7, offsetof(HijackArgs, R7))
CDAC_TYPE_FIELD(HijackArgs, /*pointer*/, R8, offsetof(HijackArgs, R8))
CDAC_TYPE_FIELD(HijackArgs, /*pointer*/, R9, offsetof(HijackArgs, R9))
CDAC_TYPE_FIELD(HijackArgs, /*pointer*/, R10, offsetof(HijackArgs, R10))
CDAC_TYPE_FIELD(HijackArgs, /*pointer*/, R11, offsetof(HijackArgs, R11))

#endif // Platform switch
CDAC_TYPE_END(HijackArgs)
#endif // FEATURE_HIJACK

CDAC_TYPE_BEGIN(FaultingExceptionFrame)
CDAC_TYPE_SIZE(sizeof(FaultingExceptionFrame))
#ifdef FEATURE_EH_FUNCLETS
CDAC_TYPE_FIELD(FaultingExceptionFrame, /*T_CONTEXT*/, TargetContext, cdac_data<FaultingExceptionFrame>::TargetContext)
#endif // FEATURE_EH_FUNCLETS
CDAC_TYPE_END(FaultingExceptionFrame)

#if defined(TARGET_X86) && !defined(UNIX_X86_ABI)
CDAC_TYPE_BEGIN(TailCallFrame)
CDAC_TYPE_SIZE(sizeof(TailCallFrame))
CDAC_TYPE_FIELD(TailCallFrame, /*CalleeSavedRegisters*/, CalleeSavedRegisters, cdac_data<TailCallFrame>::CalleeSavedRegisters)
CDAC_TYPE_FIELD(TailCallFrame, /*pointer*/, ReturnAddress, cdac_data<TailCallFrame>::ReturnAddress)
CDAC_TYPE_END(TailCallFrame)
#endif // TARGET_X86 && !UNIX_X86_ABI

// ArgumentRegisters struct is different on each platform
CDAC_TYPE_BEGIN(ArgumentRegisters)
CDAC_TYPE_SIZE(sizeof(ArgumentRegisters))
#if defined(TARGET_ARM)

CDAC_TYPE_FIELD(ArgumentRegisters, /*nuint*/, R0, offsetof(ArgumentRegisters, r[0]))
CDAC_TYPE_FIELD(ArgumentRegisters, /*nuint*/, R1, offsetof(ArgumentRegisters, r[1]))
CDAC_TYPE_FIELD(ArgumentRegisters, /*nuint*/, R2, offsetof(ArgumentRegisters, r[2]))
CDAC_TYPE_FIELD(ArgumentRegisters, /*nuint*/, R3, offsetof(ArgumentRegisters, r[3]))

#endif // TARGET_ARM
CDAC_TYPE_END(ArgumentRegisters)

// CalleeSavedRegisters struct is different on each platform
CDAC_TYPE_BEGIN(CalleeSavedRegisters)
CDAC_TYPE_SIZE(sizeof(CalleeSavedRegisters))
#if defined(TARGET_AMD64) || defined(TARGET_X86)

#define CALLEE_SAVED_REGISTER(regname) \
    CDAC_TYPE_FIELD(CalleeSavedRegisters, /*nuint*/, regname, offsetof(CalleeSavedRegisters, regname))
ENUM_CALLEE_SAVED_REGISTERS()
#undef CALLEE_SAVED_REGISTER

#elif defined(TARGET_ARM)

CDAC_TYPE_FIELD(CalleeSavedRegisters, /*nuint*/, R4, offsetof(CalleeSavedRegisters, r4))
CDAC_TYPE_FIELD(CalleeSavedRegisters, /*nuint*/, R5, offsetof(CalleeSavedRegisters, r5))
CDAC_TYPE_FIELD(CalleeSavedRegisters, /*nuint*/, R6, offsetof(CalleeSavedRegisters, r6))
CDAC_TYPE_FIELD(CalleeSavedRegisters, /*nuint*/, R7, offsetof(CalleeSavedRegisters, r7))
CDAC_TYPE_FIELD(CalleeSavedRegisters, /*nuint*/, R8, offsetof(CalleeSavedRegisters, r8))
CDAC_TYPE_FIELD(CalleeSavedRegisters, /*nuint*/, R9, offsetof(CalleeSavedRegisters, r9))
CDAC_TYPE_FIELD(CalleeSavedRegisters, /*nuint*/, R10, offsetof(CalleeSavedRegisters, r10))
CDAC_TYPE_FIELD(CalleeSavedRegisters, /*nuint*/, R11, offsetof(CalleeSavedRegisters, r11))
CDAC_TYPE_FIELD(CalleeSavedRegisters, /*nuint*/, Lr, offsetof(CalleeSavedRegisters, r14))

#elif defined(TARGET_ARM64)

CDAC_TYPE_FIELD(CalleeSavedRegisters, /*nuint*/, X19, offsetof(CalleeSavedRegisters, x19))
CDAC_TYPE_FIELD(CalleeSavedRegisters, /*nuint*/, X20, offsetof(CalleeSavedRegisters, x20))
CDAC_TYPE_FIELD(CalleeSavedRegisters, /*nuint*/, X21, offsetof(CalleeSavedRegisters, x21))
CDAC_TYPE_FIELD(CalleeSavedRegisters, /*nuint*/, X22, offsetof(CalleeSavedRegisters, x22))
CDAC_TYPE_FIELD(CalleeSavedRegisters, /*nuint*/, X23, offsetof(CalleeSavedRegisters, x23))
CDAC_TYPE_FIELD(CalleeSavedRegisters, /*nuint*/, X24, offsetof(CalleeSavedRegisters, x24))
CDAC_TYPE_FIELD(CalleeSavedRegisters, /*nuint*/, X25, offsetof(CalleeSavedRegisters, x25))
CDAC_TYPE_FIELD(CalleeSavedRegisters, /*nuint*/, X26, offsetof(CalleeSavedRegisters, x26))
CDAC_TYPE_FIELD(CalleeSavedRegisters, /*nuint*/, X27, offsetof(CalleeSavedRegisters, x27))
CDAC_TYPE_FIELD(CalleeSavedRegisters, /*nuint*/, X28, offsetof(CalleeSavedRegisters, x28))
CDAC_TYPE_FIELD(CalleeSavedRegisters, /*nuint*/, Fp, offsetof(CalleeSavedRegisters, x29))
CDAC_TYPE_FIELD(CalleeSavedRegisters, /*nuint*/, Lr, offsetof(CalleeSavedRegisters, x30))

#endif // Platform switch
CDAC_TYPE_END(CalleeSavedRegisters)

CDAC_TYPES_END()

CDAC_GLOBALS_BEGIN()

#if defined(TARGET_UNIX)
CDAC_GLOBAL_STRING(OperatingSystem, unix)
#elif defined(TARGET_WINDOWS)
CDAC_GLOBAL_STRING(OperatingSystem, windows)
#else
#error TARGET_{OS} define is not recognized by the cDAC. Update this switch and the enum values in IRuntimeInfo.cs
#endif

#if defined(TARGET_X86)
CDAC_GLOBAL_STRING(Architecture, x86)
#elif defined(TARGET_AMD64)
CDAC_GLOBAL_STRING(Architecture, x64)
#elif defined(TARGET_ARM)
CDAC_GLOBAL_STRING(Architecture, arm)
#elif defined(TARGET_ARM64)
CDAC_GLOBAL_STRING(Architecture, arm64)
#elif defined(TARGET_LOONGARCH64)
CDAC_GLOBAL_STRING(Architecture, loongarch64)
#elif defined(TARGET_RISCV64)
CDAC_GLOBAL_STRING(Architecture, riscv64)
#else
#error TARGET_{ARCH} define is not recognized by the cDAC. Update this switch and the enum values in IRuntimeInfo.cs
#endif

CDAC_GLOBAL_STRING(RID, RID_STRING)

CDAC_GLOBAL(GCInfoVersion, uint32, GCINFO_VERSION)

CDAC_GLOBAL_POINTER(AppDomain, &AppDomain::m_pTheAppDomain)
CDAC_GLOBAL_POINTER(SystemDomain, cdac_data<SystemDomain>::SystemDomainPtr)
CDAC_GLOBAL_POINTER(ThreadStore, &ThreadStore::s_pThreadStore)
CDAC_GLOBAL_POINTER(FinalizerThread, &::g_pFinalizerThread)
CDAC_GLOBAL_POINTER(GCThread, &::g_pSuspensionThread)

// Add FrameIdentifier for all defined Frame types. Used to differentiate Frame objects.
#define FRAME_TYPE_NAME(frameType) \
    CDAC_GLOBAL(frameType##Identifier, nuint, (uint64_t)FrameIdentifier::frameType)

    #include "frames.h"
#undef FRAME_TYPE_NAME

CDAC_GLOBAL(MethodDescTokenRemainderBitCount, uint8, METHOD_TOKEN_REMAINDER_BIT_COUNT)
#if FEATURE_COMINTEROP
CDAC_GLOBAL(FeatureCOMInterop, uint8, 1)
#else
CDAC_GLOBAL(FeatureCOMInterop, uint8, 0)
#endif
// See Object::GetGCSafeMethodTable
#ifdef TARGET_64BIT
CDAC_GLOBAL(ObjectToMethodTableUnmask, uint8, 1 | 1 << 1 | 1 << 2)
#else
CDAC_GLOBAL(ObjectToMethodTableUnmask, uint8, 1 | 1 << 1)
#endif //TARGET_64BIT
CDAC_GLOBAL(SOSBreakingChangeVersion, uint8, SOS_BREAKING_CHANGE_VERSION)
CDAC_GLOBAL(DirectorySeparator, uint8, (uint8_t)DIRECTORY_SEPARATOR_CHAR_A)
CDAC_GLOBAL(HashMapSlotsPerBucket, uint32, SLOTS_PER_BUCKET)
CDAC_GLOBAL(HashMapValueMask, uint64, VALUE_MASK)
CDAC_GLOBAL(MethodDescAlignment, uint64, MethodDesc::ALIGNMENT)
CDAC_GLOBAL(ObjectHeaderSize, uint64, OBJHEADER_SIZE)
CDAC_GLOBAL(SyncBlockValueToObjectOffset, uint16, OBJHEADER_SIZE - cdac_data<ObjHeader>::SyncBlockValue)
CDAC_GLOBAL(StubCodeBlockLast, uint8, STUB_CODE_BLOCK_LAST)
CDAC_GLOBAL_POINTER(ArrayBoundsZero, cdac_data<ArrayBase>::ArrayBoundsZero)
CDAC_GLOBAL_POINTER(ExceptionMethodTable, &::g_pExceptionClass)
CDAC_GLOBAL_POINTER(FreeObjectMethodTable, &::g_pFreeObjectMethodTable)
CDAC_GLOBAL_POINTER(ObjectMethodTable, &::g_pObjectClass)
CDAC_GLOBAL_POINTER(ObjectArrayMethodTable, &::g_pPredefinedArrayTypes[ELEMENT_TYPE_OBJECT])
CDAC_GLOBAL_POINTER(StringMethodTable, &::g_pStringClass)
CDAC_GLOBAL_POINTER(SyncTableEntries, &::g_pSyncTable)
CDAC_GLOBAL_POINTER(MiniMetaDataBuffAddress, &::g_MiniMetaDataBuffAddress)
CDAC_GLOBAL_POINTER(MiniMetaDataBuffMaxSize, &::g_MiniMetaDataBuffMaxSize)
CDAC_GLOBAL_POINTER(OffsetOfCurrentThreadInfo, &::g_offsetOfCurrentThreadInfo)
#ifdef TARGET_WINDOWS
CDAC_GLOBAL_POINTER(TlsIndexBase, &::_tls_index)
#endif // TARGET_WINDOWS
#ifdef STRESS_LOG
CDAC_GLOBAL(StressLogEnabled, uint8, 1)
CDAC_GLOBAL_POINTER(StressLog, &g_pStressLog)
CDAC_GLOBAL(StressLogHasModuleTable, uint8, 1)
CDAC_GLOBAL_POINTER(StressLogModuleTable, &g_pStressLog->modules)
CDAC_GLOBAL(StressLogMaxModules, uint64, cdac_offsets<StressLog>::MAX_MODULES)
CDAC_GLOBAL(StressLogChunkSize, uint32, STRESSLOG_CHUNK_SIZE)
CDAC_GLOBAL(StressLogValidChunkSig, uint32, StressLogChunk::ValidChunkSig)
CDAC_GLOBAL(StressLogMaxMessageSize, uint64, (uint64_t)StressMsg::maxMsgSize)
#else
CDAC_GLOBAL(StressLogEnabled, uint8, 0)
#endif
CDAC_GLOBAL_POINTER(ExecutionManagerCodeRangeMapAddress, cdac_data<ExecutionManager>::CodeRangeMapAddress)
CDAC_GLOBAL_POINTER(PlatformMetadata, &::g_cdacPlatformMetadata)
CDAC_GLOBAL_POINTER(ProfilerControlBlock, &::g_profControlBlock)
CDAC_GLOBALS_END()

#undef CDAC_BASELINE
#undef CDAC_TYPES_BEGIN
#undef CDAC_TYPE_BEGIN
#undef CDAC_TYPE_INDETERMINATE
#undef CDAC_TYPE_SIZE
#undef CDAC_TYPE_FIELD
#undef CDAC_TYPE_END
#undef CDAC_TYPES_END
#undef CDAC_GLOBALS_BEGIN
#undef CDAC_GLOBAL
#undef CDAC_GLOBAL_POINTER
#undef CDAC_GLOBAL_STRING
#undef CDAC_GLOBALS_END

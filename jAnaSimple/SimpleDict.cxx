// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME SimpleDict
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "ROOT/RConfig.hxx"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Header files passed as explicit arguments
#include "src/AliJBaseTrack.h"
#include "src/AliJBaseEventHeader.h"
#include "src/JBaseTrack.h"
#include "src/JBaseEventHeader.h"
#include "src/JTreeDataManager.h"
#include "src/JTreeDataManager_Pythia.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_AliJBaseTrack(void *p = nullptr);
   static void *newArray_AliJBaseTrack(Long_t size, void *p);
   static void delete_AliJBaseTrack(void *p);
   static void deleteArray_AliJBaseTrack(void *p);
   static void destruct_AliJBaseTrack(void *p);
   static void streamer_AliJBaseTrack(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AliJBaseTrack*)
   {
      ::AliJBaseTrack *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::AliJBaseTrack >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("AliJBaseTrack", ::AliJBaseTrack::Class_Version(), "AliJBaseTrack.h", 33,
                  typeid(::AliJBaseTrack), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::AliJBaseTrack::Dictionary, isa_proxy, 16,
                  sizeof(::AliJBaseTrack) );
      instance.SetNew(&new_AliJBaseTrack);
      instance.SetNewArray(&newArray_AliJBaseTrack);
      instance.SetDelete(&delete_AliJBaseTrack);
      instance.SetDeleteArray(&deleteArray_AliJBaseTrack);
      instance.SetDestructor(&destruct_AliJBaseTrack);
      instance.SetStreamerFunc(&streamer_AliJBaseTrack);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AliJBaseTrack*)
   {
      return GenerateInitInstanceLocal(static_cast<::AliJBaseTrack*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::AliJBaseTrack*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_AliJBaseEventHeader(void *p = nullptr);
   static void *newArray_AliJBaseEventHeader(Long_t size, void *p);
   static void delete_AliJBaseEventHeader(void *p);
   static void deleteArray_AliJBaseEventHeader(void *p);
   static void destruct_AliJBaseEventHeader(void *p);
   static void streamer_AliJBaseEventHeader(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AliJBaseEventHeader*)
   {
      ::AliJBaseEventHeader *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::AliJBaseEventHeader >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("AliJBaseEventHeader", ::AliJBaseEventHeader::Class_Version(), "AliJBaseEventHeader.h", 27,
                  typeid(::AliJBaseEventHeader), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::AliJBaseEventHeader::Dictionary, isa_proxy, 16,
                  sizeof(::AliJBaseEventHeader) );
      instance.SetNew(&new_AliJBaseEventHeader);
      instance.SetNewArray(&newArray_AliJBaseEventHeader);
      instance.SetDelete(&delete_AliJBaseEventHeader);
      instance.SetDeleteArray(&deleteArray_AliJBaseEventHeader);
      instance.SetDestructor(&destruct_AliJBaseEventHeader);
      instance.SetStreamerFunc(&streamer_AliJBaseEventHeader);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AliJBaseEventHeader*)
   {
      return GenerateInitInstanceLocal(static_cast<::AliJBaseEventHeader*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::AliJBaseEventHeader*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_JBaseTrack(void *p = nullptr);
   static void *newArray_JBaseTrack(Long_t size, void *p);
   static void delete_JBaseTrack(void *p);
   static void deleteArray_JBaseTrack(void *p);
   static void destruct_JBaseTrack(void *p);
   static void streamer_JBaseTrack(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::JBaseTrack*)
   {
      ::JBaseTrack *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::JBaseTrack >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("JBaseTrack", ::JBaseTrack::Class_Version(), "JBaseTrack.h", 26,
                  typeid(::JBaseTrack), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::JBaseTrack::Dictionary, isa_proxy, 16,
                  sizeof(::JBaseTrack) );
      instance.SetNew(&new_JBaseTrack);
      instance.SetNewArray(&newArray_JBaseTrack);
      instance.SetDelete(&delete_JBaseTrack);
      instance.SetDeleteArray(&deleteArray_JBaseTrack);
      instance.SetDestructor(&destruct_JBaseTrack);
      instance.SetStreamerFunc(&streamer_JBaseTrack);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::JBaseTrack*)
   {
      return GenerateInitInstanceLocal(static_cast<::JBaseTrack*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::JBaseTrack*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_JBaseEventHeader(void *p = nullptr);
   static void *newArray_JBaseEventHeader(Long_t size, void *p);
   static void delete_JBaseEventHeader(void *p);
   static void deleteArray_JBaseEventHeader(void *p);
   static void destruct_JBaseEventHeader(void *p);
   static void streamer_JBaseEventHeader(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::JBaseEventHeader*)
   {
      ::JBaseEventHeader *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::JBaseEventHeader >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("JBaseEventHeader", ::JBaseEventHeader::Class_Version(), "JBaseEventHeader.h", 22,
                  typeid(::JBaseEventHeader), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::JBaseEventHeader::Dictionary, isa_proxy, 16,
                  sizeof(::JBaseEventHeader) );
      instance.SetNew(&new_JBaseEventHeader);
      instance.SetNewArray(&newArray_JBaseEventHeader);
      instance.SetDelete(&delete_JBaseEventHeader);
      instance.SetDeleteArray(&deleteArray_JBaseEventHeader);
      instance.SetDestructor(&destruct_JBaseEventHeader);
      instance.SetStreamerFunc(&streamer_JBaseEventHeader);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::JBaseEventHeader*)
   {
      return GenerateInitInstanceLocal(static_cast<::JBaseEventHeader*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::JBaseEventHeader*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *JTreeDataManager_Dictionary();
   static void JTreeDataManager_TClassManip(TClass*);
   static void *new_JTreeDataManager(void *p = nullptr);
   static void *newArray_JTreeDataManager(Long_t size, void *p);
   static void delete_JTreeDataManager(void *p);
   static void deleteArray_JTreeDataManager(void *p);
   static void destruct_JTreeDataManager(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::JTreeDataManager*)
   {
      ::JTreeDataManager *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::JTreeDataManager));
      static ::ROOT::TGenericClassInfo 
         instance("JTreeDataManager", "JTreeDataManager.h", 36,
                  typeid(::JTreeDataManager), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &JTreeDataManager_Dictionary, isa_proxy, 0,
                  sizeof(::JTreeDataManager) );
      instance.SetNew(&new_JTreeDataManager);
      instance.SetNewArray(&newArray_JTreeDataManager);
      instance.SetDelete(&delete_JTreeDataManager);
      instance.SetDeleteArray(&deleteArray_JTreeDataManager);
      instance.SetDestructor(&destruct_JTreeDataManager);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::JTreeDataManager*)
   {
      return GenerateInitInstanceLocal(static_cast<::JTreeDataManager*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::JTreeDataManager*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *JTreeDataManager_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const ::JTreeDataManager*>(nullptr))->GetClass();
      JTreeDataManager_TClassManip(theClass);
   return theClass;
   }

   static void JTreeDataManager_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *JTreeDataManager_Pythia_Dictionary();
   static void JTreeDataManager_Pythia_TClassManip(TClass*);
   static void *new_JTreeDataManager_Pythia(void *p = nullptr);
   static void *newArray_JTreeDataManager_Pythia(Long_t size, void *p);
   static void delete_JTreeDataManager_Pythia(void *p);
   static void deleteArray_JTreeDataManager_Pythia(void *p);
   static void destruct_JTreeDataManager_Pythia(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::JTreeDataManager_Pythia*)
   {
      ::JTreeDataManager_Pythia *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::JTreeDataManager_Pythia));
      static ::ROOT::TGenericClassInfo 
         instance("JTreeDataManager_Pythia", "JTreeDataManager_Pythia.h", 37,
                  typeid(::JTreeDataManager_Pythia), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &JTreeDataManager_Pythia_Dictionary, isa_proxy, 0,
                  sizeof(::JTreeDataManager_Pythia) );
      instance.SetNew(&new_JTreeDataManager_Pythia);
      instance.SetNewArray(&newArray_JTreeDataManager_Pythia);
      instance.SetDelete(&delete_JTreeDataManager_Pythia);
      instance.SetDeleteArray(&deleteArray_JTreeDataManager_Pythia);
      instance.SetDestructor(&destruct_JTreeDataManager_Pythia);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::JTreeDataManager_Pythia*)
   {
      return GenerateInitInstanceLocal(static_cast<::JTreeDataManager_Pythia*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::JTreeDataManager_Pythia*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *JTreeDataManager_Pythia_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const ::JTreeDataManager_Pythia*>(nullptr))->GetClass();
      JTreeDataManager_Pythia_TClassManip(theClass);
   return theClass;
   }

   static void JTreeDataManager_Pythia_TClassManip(TClass* ){
   }

} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr AliJBaseTrack::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *AliJBaseTrack::Class_Name()
{
   return "AliJBaseTrack";
}

//______________________________________________________________________________
const char *AliJBaseTrack::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AliJBaseTrack*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int AliJBaseTrack::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AliJBaseTrack*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *AliJBaseTrack::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AliJBaseTrack*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *AliJBaseTrack::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AliJBaseTrack*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr AliJBaseEventHeader::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *AliJBaseEventHeader::Class_Name()
{
   return "AliJBaseEventHeader";
}

//______________________________________________________________________________
const char *AliJBaseEventHeader::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AliJBaseEventHeader*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int AliJBaseEventHeader::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AliJBaseEventHeader*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *AliJBaseEventHeader::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AliJBaseEventHeader*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *AliJBaseEventHeader::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AliJBaseEventHeader*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr JBaseTrack::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *JBaseTrack::Class_Name()
{
   return "JBaseTrack";
}

//______________________________________________________________________________
const char *JBaseTrack::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JBaseTrack*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int JBaseTrack::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JBaseTrack*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *JBaseTrack::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JBaseTrack*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *JBaseTrack::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JBaseTrack*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr JBaseEventHeader::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *JBaseEventHeader::Class_Name()
{
   return "JBaseEventHeader";
}

//______________________________________________________________________________
const char *JBaseEventHeader::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JBaseEventHeader*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int JBaseEventHeader::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JBaseEventHeader*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *JBaseEventHeader::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JBaseEventHeader*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *JBaseEventHeader::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JBaseEventHeader*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void AliJBaseTrack::Streamer(TBuffer &R__b)
{
   // Stream an object of class AliJBaseTrack.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TLorentzVector::Streamer(R__b);
      R__b >> fID;
      R__b >> fLabel;
      R__b >> fParticleType;
      R__b >> fCharge;
      R__b >> fStatus;
      R__b >> fFlags;
      R__b.CheckByteCount(R__s, R__c, AliJBaseTrack::IsA());
   } else {
      R__c = R__b.WriteVersion(AliJBaseTrack::IsA(), kTRUE);
      TLorentzVector::Streamer(R__b);
      R__b << fID;
      R__b << fLabel;
      R__b << fParticleType;
      R__b << fCharge;
      R__b << fStatus;
      R__b << fFlags;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_AliJBaseTrack(void *p) {
      return  p ? new(p) ::AliJBaseTrack : new ::AliJBaseTrack;
   }
   static void *newArray_AliJBaseTrack(Long_t nElements, void *p) {
      return p ? new(p) ::AliJBaseTrack[nElements] : new ::AliJBaseTrack[nElements];
   }
   // Wrapper around operator delete
   static void delete_AliJBaseTrack(void *p) {
      delete (static_cast<::AliJBaseTrack*>(p));
   }
   static void deleteArray_AliJBaseTrack(void *p) {
      delete [] (static_cast<::AliJBaseTrack*>(p));
   }
   static void destruct_AliJBaseTrack(void *p) {
      typedef ::AliJBaseTrack current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_AliJBaseTrack(TBuffer &buf, void *obj) {
      ((::AliJBaseTrack*)obj)->::AliJBaseTrack::Streamer(buf);
   }
} // end of namespace ROOT for class ::AliJBaseTrack

//______________________________________________________________________________
void AliJBaseEventHeader::Streamer(TBuffer &R__b)
{
   // Stream an object of class AliJBaseEventHeader.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TNamed::Streamer(R__b);
      R__b >> fEventID;
      {float R_Dummy; R__b >> R_Dummy; fCentrality=Double32_t(R_Dummy);}
      {float R_Dummy; R__b >> R_Dummy; fVtxX=Double32_t(R_Dummy);}
      {float R_Dummy; R__b >> R_Dummy; fVtxY=Double32_t(R_Dummy);}
      {float R_Dummy; R__b >> R_Dummy; fVtxZ=Double32_t(R_Dummy);}
      {float R_Dummy; R__b >> R_Dummy; fVtxZErr=Double32_t(R_Dummy);}
      {float R_Dummy; R__b >> R_Dummy; fVtxMCX=Double32_t(R_Dummy);}
      {float R_Dummy; R__b >> R_Dummy; fVtxMCY=Double32_t(R_Dummy);}
      {float R_Dummy; R__b >> R_Dummy; fVtxMCZ=Double32_t(R_Dummy);}
      R__b.CheckByteCount(R__s, R__c, AliJBaseEventHeader::IsA());
   } else {
      R__c = R__b.WriteVersion(AliJBaseEventHeader::IsA(), kTRUE);
      TNamed::Streamer(R__b);
      R__b << fEventID;
      R__b << float(fCentrality);
      R__b << float(fVtxX);
      R__b << float(fVtxY);
      R__b << float(fVtxZ);
      R__b << float(fVtxZErr);
      R__b << float(fVtxMCX);
      R__b << float(fVtxMCY);
      R__b << float(fVtxMCZ);
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_AliJBaseEventHeader(void *p) {
      return  p ? new(p) ::AliJBaseEventHeader : new ::AliJBaseEventHeader;
   }
   static void *newArray_AliJBaseEventHeader(Long_t nElements, void *p) {
      return p ? new(p) ::AliJBaseEventHeader[nElements] : new ::AliJBaseEventHeader[nElements];
   }
   // Wrapper around operator delete
   static void delete_AliJBaseEventHeader(void *p) {
      delete (static_cast<::AliJBaseEventHeader*>(p));
   }
   static void deleteArray_AliJBaseEventHeader(void *p) {
      delete [] (static_cast<::AliJBaseEventHeader*>(p));
   }
   static void destruct_AliJBaseEventHeader(void *p) {
      typedef ::AliJBaseEventHeader current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_AliJBaseEventHeader(TBuffer &buf, void *obj) {
      ((::AliJBaseEventHeader*)obj)->::AliJBaseEventHeader::Streamer(buf);
   }
} // end of namespace ROOT for class ::AliJBaseEventHeader

//______________________________________________________________________________
void JBaseTrack::Streamer(TBuffer &R__b)
{
   // Stream an object of class JBaseTrack.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TLorentzVector::Streamer(R__b);
      R__b >> fID;
      R__b >> fLabel;
      R__b >> fParticleType;
      R__b >> fCharge;
      R__b >> fStatus;
      R__b.CheckByteCount(R__s, R__c, JBaseTrack::IsA());
   } else {
      R__c = R__b.WriteVersion(JBaseTrack::IsA(), kTRUE);
      TLorentzVector::Streamer(R__b);
      R__b << fID;
      R__b << fLabel;
      R__b << fParticleType;
      R__b << fCharge;
      R__b << fStatus;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_JBaseTrack(void *p) {
      return  p ? new(p) ::JBaseTrack : new ::JBaseTrack;
   }
   static void *newArray_JBaseTrack(Long_t nElements, void *p) {
      return p ? new(p) ::JBaseTrack[nElements] : new ::JBaseTrack[nElements];
   }
   // Wrapper around operator delete
   static void delete_JBaseTrack(void *p) {
      delete (static_cast<::JBaseTrack*>(p));
   }
   static void deleteArray_JBaseTrack(void *p) {
      delete [] (static_cast<::JBaseTrack*>(p));
   }
   static void destruct_JBaseTrack(void *p) {
      typedef ::JBaseTrack current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_JBaseTrack(TBuffer &buf, void *obj) {
      ((::JBaseTrack*)obj)->::JBaseTrack::Streamer(buf);
   }
} // end of namespace ROOT for class ::JBaseTrack

//______________________________________________________________________________
void JBaseEventHeader::Streamer(TBuffer &R__b)
{
   // Stream an object of class JBaseEventHeader.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TNamed::Streamer(R__b);
      R__b >> fEventID;
      {float R_Dummy; R__b >> R_Dummy; fCentrality=Double32_t(R_Dummy);}
      {float R_Dummy; R__b >> R_Dummy; fEP=Double32_t(R_Dummy);}
      {float R_Dummy; R__b >> R_Dummy; fVtxX=Double32_t(R_Dummy);}
      {float R_Dummy; R__b >> R_Dummy; fVtxY=Double32_t(R_Dummy);}
      {float R_Dummy; R__b >> R_Dummy; fVtxZ=Double32_t(R_Dummy);}
      {float R_Dummy; R__b >> R_Dummy; fVtxZErr=Double32_t(R_Dummy);}
      R__b.CheckByteCount(R__s, R__c, JBaseEventHeader::IsA());
   } else {
      R__c = R__b.WriteVersion(JBaseEventHeader::IsA(), kTRUE);
      TNamed::Streamer(R__b);
      R__b << fEventID;
      R__b << float(fCentrality);
      R__b << float(fEP);
      R__b << float(fVtxX);
      R__b << float(fVtxY);
      R__b << float(fVtxZ);
      R__b << float(fVtxZErr);
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_JBaseEventHeader(void *p) {
      return  p ? new(p) ::JBaseEventHeader : new ::JBaseEventHeader;
   }
   static void *newArray_JBaseEventHeader(Long_t nElements, void *p) {
      return p ? new(p) ::JBaseEventHeader[nElements] : new ::JBaseEventHeader[nElements];
   }
   // Wrapper around operator delete
   static void delete_JBaseEventHeader(void *p) {
      delete (static_cast<::JBaseEventHeader*>(p));
   }
   static void deleteArray_JBaseEventHeader(void *p) {
      delete [] (static_cast<::JBaseEventHeader*>(p));
   }
   static void destruct_JBaseEventHeader(void *p) {
      typedef ::JBaseEventHeader current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_JBaseEventHeader(TBuffer &buf, void *obj) {
      ((::JBaseEventHeader*)obj)->::JBaseEventHeader::Streamer(buf);
   }
} // end of namespace ROOT for class ::JBaseEventHeader

namespace ROOT {
   // Wrappers around operator new
   static void *new_JTreeDataManager(void *p) {
      return  p ? new(p) ::JTreeDataManager : new ::JTreeDataManager;
   }
   static void *newArray_JTreeDataManager(Long_t nElements, void *p) {
      return p ? new(p) ::JTreeDataManager[nElements] : new ::JTreeDataManager[nElements];
   }
   // Wrapper around operator delete
   static void delete_JTreeDataManager(void *p) {
      delete (static_cast<::JTreeDataManager*>(p));
   }
   static void deleteArray_JTreeDataManager(void *p) {
      delete [] (static_cast<::JTreeDataManager*>(p));
   }
   static void destruct_JTreeDataManager(void *p) {
      typedef ::JTreeDataManager current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::JTreeDataManager

namespace ROOT {
   // Wrappers around operator new
   static void *new_JTreeDataManager_Pythia(void *p) {
      return  p ? new(p) ::JTreeDataManager_Pythia : new ::JTreeDataManager_Pythia;
   }
   static void *newArray_JTreeDataManager_Pythia(Long_t nElements, void *p) {
      return p ? new(p) ::JTreeDataManager_Pythia[nElements] : new ::JTreeDataManager_Pythia[nElements];
   }
   // Wrapper around operator delete
   static void delete_JTreeDataManager_Pythia(void *p) {
      delete (static_cast<::JTreeDataManager_Pythia*>(p));
   }
   static void deleteArray_JTreeDataManager_Pythia(void *p) {
      delete [] (static_cast<::JTreeDataManager_Pythia*>(p));
   }
   static void destruct_JTreeDataManager_Pythia(void *p) {
      typedef ::JTreeDataManager_Pythia current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::JTreeDataManager_Pythia

namespace ROOT {
   // Registration Schema evolution read functions
   int RecordReadRules_SimpleDict() {
      return 0;
   }
   static int _R__UNIQUE_DICT_(ReadRules_SimpleDict) = RecordReadRules_SimpleDict();R__UseDummy(_R__UNIQUE_DICT_(ReadRules_SimpleDict));
} // namespace ROOT
namespace {
  void TriggerDictionaryInitialization_SimpleDict_Impl() {
    static const char* headers[] = {
"src/AliJBaseTrack.h",
"src/AliJBaseEventHeader.h",
"src/JBaseTrack.h",
"src/JBaseEventHeader.h",
"src/JTreeDataManager.h",
"src/JTreeDataManager_Pythia.h",
nullptr
    };
    static const char* includePaths[] = {
"src",
"/Users/djkim/alice/sw/osx_arm64/ROOT/v6-36-04-alice2-local1/include/",
"/Users/djkim/Documents/GitHub/draw_jet/jAnaSimple/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "SimpleDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$src/AliJBaseTrack.h")))  AliJBaseTrack;
class __attribute__((annotate("$clingAutoload$src/AliJBaseEventHeader.h")))  AliJBaseEventHeader;
class __attribute__((annotate("$clingAutoload$src/JBaseTrack.h")))  JBaseTrack;
class __attribute__((annotate("$clingAutoload$src/JBaseEventHeader.h")))  JBaseEventHeader;
class __attribute__((annotate("$clingAutoload$src/JTreeDataManager.h")))  JTreeDataManager;
class __attribute__((annotate("$clingAutoload$src/JTreeDataManager_Pythia.h")))  JTreeDataManager_Pythia;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "SimpleDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "src/AliJBaseTrack.h"
#include "src/AliJBaseEventHeader.h"
#include "src/JBaseTrack.h"
#include "src/JBaseEventHeader.h"
#include "src/JTreeDataManager.h"
#include "src/JTreeDataManager_Pythia.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"AliJBaseEventHeader", payloadCode, "@",
"AliJBaseTrack", payloadCode, "@",
"JBaseEventHeader", payloadCode, "@",
"JBaseTrack", payloadCode, "@",
"JTreeDataManager", payloadCode, "@",
"JTreeDataManager_Pythia", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("SimpleDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_SimpleDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_SimpleDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_SimpleDict() {
  TriggerDictionaryInitialization_SimpleDict_Impl();
}

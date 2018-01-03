#pragma once

#include <vector>
#include <set>
#include <map>
#include <v8.h>
#include <nan.h>
#include <glib.h>
#include <girepository.h>

namespace gir {

using namespace v8;
using namespace std;

class GIRObject;

typedef Nan::Persistent<FunctionTemplate, CopyablePersistentTraits<FunctionTemplate>> PersistentFunctionTemplate;

struct ObjectFunctionTemplate {
    char *type_name;
    GIObjectInfo *info;
    PersistentFunctionTemplate object_template;
    GType type;
    char *namespace_;
};

struct MarshalData {
    GIRObject *that;
    char *event_name;
};

struct InstanceData {
    Local<Value> instance;
    GIRObject *obj;
};


class GIRObject : public Nan::ObjectWrap {
  public:
    GIRObject() {};
    GIRObject(GIObjectInfo *info_, map<string, GValue> &properties);
    virtual ~GIRObject();

    GObject *obj;
    bool abstract;
    GIBaseInfo *info;

    static std::set<GIRObject *> instances;
    static std::vector<ObjectFunctionTemplate *> templates;

    static Local<Value> FromExisting(GObject *obj, GType t);
    static NAN_METHOD(New);

    static Local<Object> Prepare(GIObjectInfo *object_info);
    static void RegisterMethods(GIObjectInfo *object_info, const char *namespace_, Local<FunctionTemplate> &object_template);
    static void SetCustomFields(Local<FunctionTemplate> &object_template, GIObjectInfo *object_info);
    static void SetCustomPrototypeMethods(Local<FunctionTemplate> &object_template);
    static void ExtendParent(Local<FunctionTemplate> &object_template, GIObjectInfo *object_info);

    static void Initialize(Local<Object> target, char *namespace_);

    static NAN_METHOD(GetProperty);
    static NAN_METHOD(SetProperty);
    static NAN_METHOD(GetInterface);
    static NAN_METHOD(GetField);
    static NAN_METHOD(CallVFunc);
    static NAN_METHOD(Connect);
    static NAN_METHOD(Disconnect);

    static MaybeLocal<Value> GetInstance(GObject *obj);
    static ObjectFunctionTemplate* CreateObjectTemplate(GIObjectInfo *object_info);
    static ObjectFunctionTemplate* FindTemplateFromObjectInfo(GIObjectInfo *object_info);
    static ObjectFunctionTemplate* FindOrCreateTemplateFromObjectInfo(GIObjectInfo *object_info);

    static GIFunctionInfo *FindMethod(GIObjectInfo *inf, char *name);
    static GIFunctionInfo *FindProperty(GIObjectInfo *inf, char *name);
    static GIFunctionInfo *FindInterface(GIObjectInfo *inf, char *name);
    static GIFunctionInfo *FindField(GIObjectInfo *inf, char *name);
    static GIFunctionInfo *FindSignal(GIObjectInfo *inf, char *name);
    static GIFunctionInfo *FindVFunc(GIObjectInfo *inf, char *name);

  private:
    static void SetMethod(Local<FunctionTemplate> &target, GIFunctionInfo *function_info);

    static Local<ObjectTemplate> PropertyList(GIObjectInfo *info);
    static Local<ObjectTemplate> MethodList(GIObjectInfo *info);
    static Local<ObjectTemplate> InterfaceList(GIObjectInfo *info);
    static Local<ObjectTemplate> FieldList(GIObjectInfo *info);
    static Local<ObjectTemplate> SignalList(GIObjectInfo *info);
    static Local<ObjectTemplate> VFuncList(GIObjectInfo *info);
    static GType GetObjectPropertyType(GIObjectInfo *object_info, const char *property_name);
    static map<string, GValue> ParseConstructorArgument(Local<Object> properties_object, GIObjectInfo *object_info);
};

}

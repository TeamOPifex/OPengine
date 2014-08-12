#include <node.h>
#include <v8.h>

using namespace v8;

#include "./Scripting/include/wrappers/Global.h"
#include "./Scripting/include/wrappers/MathWrapper.h"
#include "./Scripting/include/wrappers/DataWrapper.h"
#include "./Scripting/include/wrappers/HumanWrapper.h"

void InitializeMethods(V8ObjectGlobal target) {
	GlobalInitializeMethodsO(NULL, target);
	MathInitializeMethodsO(NULL, target);
	DataInitializeMethodsO(NULL, target);
	HumanInitializeMethodsO(NULL, target);
}

NODE_MODULE(OPengine, InitializeMethods)
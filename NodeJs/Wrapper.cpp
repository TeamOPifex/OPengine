#include <node.h>
#include <v8.h>

using namespace v8;

#include "include/wrappers/Global.h"
#include "include/wrappers/MathWrapper.h"
#include "include/wrappers/DataWrapper.h"
#include "include/wrappers/HumanWrapper.h"

void InitializeMethods(Handle<Object> target) {
	GlobalInitializeMethods(NULL, target);
	MathInitializeMethods(NULL, target);
	DataInitializeMethods(NULL, target);
	HumanInitializeMethods(NULL, target);
}

NODE_MODULE(OPengine, InitializeMethods)
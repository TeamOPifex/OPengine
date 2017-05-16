#include "./Scripting/include/JavaScript/OPjavaScriptHelper.h"
#include "./Core/include/OPlog.h"

i32 _JS_ARGC = 0;

#ifdef OPIFEX_OPTION_V8

void ReportException(v8::Isolate* isolate, v8::TryCatch* try_catch) {
	v8::HandleScope handle_scope(isolate);
	v8::String::Utf8Value exception(try_catch->Exception());
	v8::Handle<v8::Message> message = try_catch->Message();

	if (message.IsEmpty()) {
		// V8 didn't provide any extra information about this error; just
		// print the exception.
		OPlogErr("!!! %s\n", *exception);
	} else {
		// Print (filename):(line number): (message).
		v8::String::Utf8Value filename(message->GetScriptOrigin().ResourceName());
		int linenum = message->GetLineNumber();
		OPlogErr("### %s:%i: %s\n", *filename, linenum, *exception);

		// Print line of source code.
		v8::String::Utf8Value sourceline(message->GetSourceLine());
		OPlogErr("### %s\n", *sourceline);

		// Print wavy underline (GetUnderline is deprecated).
		int start = message->GetStartColumn();
		for (int i = 0; i < start; i++) {
			OPlg(" ");
		}
		int end = message->GetEndColumn();
		for (int i = start; i < end; i++) {
			OPlg("^");
		}
		OPlg("\n");

		v8::String::Utf8Value stack_trace(try_catch->StackTrace());
		if (stack_trace.length() > 0) {
			OPlogErr("### %s\n", *stack_trace);
		}
	}
}

#endif

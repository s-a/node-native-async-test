/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2016 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>
#include "async.h"  // NOLINT(build/include)

using v8::Function;
using v8::Local;
using v8::Number;
using v8::Value;
using Nan::AsyncQueueWorker;
using Nan::AsyncWorker;
using Nan::Callback;
using Nan::HandleScope;
using Nan::New;
using Nan::Null;
using Nan::To;

class PiWorker : public AsyncWorker {
 public:
  PiWorker(Callback *callback, char *points)
    : AsyncWorker(callback), points(points), estimate(0) {}
  ~PiWorker() {}

  // Executed inside the worker-thread.
  // It is not safe to access V8, or V8 data structures
  // here, so everything we need for input and output
  // should go on `this`.
  void Execute () {
    estimate = 0.1;//Estimate(points);
  }

  // Executed when the async work is complete
  // this function will be run inside the main event loop
  // so it is safe to use V8 again
  void HandleOKCallback () {
    HandleScope scope;
    Nan::MaybeLocal<v8::Object> res = Nan::NewBuffer(points, sizeof(points));
    Local<Value> argv[] = {
        Null()
      , res.ToLocalChecked()
    };

    callback->Call(2, argv);
  }

 private:
  char *points;
  double estimate;
};

// Asynchronous access to the `Estimate()` function
NAN_METHOD(CalculateAsync) {
//  int points = To<int>(info[0]).FromJust();
  Callback *callback = new Callback(info[1].As<Function>());



    v8::Local<v8::Object> bufferObj = info[0]->ToObject();
    char* msg = node::Buffer::Data(bufferObj);

  AsyncQueueWorker(new PiWorker(callback, msg));
}

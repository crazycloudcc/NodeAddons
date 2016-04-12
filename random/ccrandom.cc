
#include<node.h>

#include<stdio.h>
#include<stdlib.h>
#include<time.h>

namespace ccrandom {
    using v8::FunctionCallbackInfo;
    using v8::Isolate;
    using v8::Local;
    using v8::Object ;
    using v8::String ;
    using v8::Boolean ;
    using v8::Number ;
    using v8::Value ;

    int* pArray = 0 ;
    int nIndex = 0 ;
    int nCount = 0 ;
    int nOdds = 0 ;

    void ResetPool (int _random)
    {
        int randomCount = _random == 0 ? nCount : _random ;
        int index1 = 0 ;
        int index2 = 0 ;
        int tempValue = 0 ;

        srand((unsigned)(time(0))) ;

        for (int i = 0; i < randomCount; ++i)
        {
            index1 = (int)(rand() % nCount) ;
            index2 = (int)(rand() % nCount) ;
            tempValue = pArray[index1] ;
            pArray[index1] = pArray[index2] ;
            pArray[index2] = tempValue ;
            // printf("for : [%d], [%d], [%d]\r\n", i, index1, index2) ;
        }
    }

    void Init (const FunctionCallbackInfo<Value>& args)
    {
        Isolate* isolate = args.GetIsolate() ;
        if (args.Length() < 2)
        {
            isolate->ThrowException(v8::Exception::TypeError(String::NewFromUtf8(isolate, "Arguments Number Error!"))) ;
            return ;
        }

        if (!(args[0]->IsNumber()))
        {
            isolate->ThrowException(v8::Exception::TypeError(String::NewFromUtf8(isolate, "args[0] Error: is not number type."))) ;
            return ;
        }

        if (!(args[1]->IsNumber()))
        {
            isolate->ThrowException(v8::Exception::TypeError(String::NewFromUtf8(isolate, "args[1] Error: is not number type."))) ;
            return ;
        }

        nCount = args[0]->NumberValue() ;
        nOdds = args[1]->NumberValue() ;

        pArray = new int[nCount] ;
        for (int i = 0; i < nCount; ++i)
        {
            pArray[i] = i ;
        }
        nIndex = 0 ;
        // args.GetReturnValue().Set(String::NewFromUtf8(isolate, "world")) ;
    }

    void Reset (const FunctionCallbackInfo<Value>& args)
    {
        int randomCount = nCount ;
        Isolate* isolate = args.GetIsolate() ;
        if (args.Length() >= 1)
        {
            if (!(args[0]->IsNumber()))
            {
                isolate->ThrowException(v8::Exception::TypeError(String::NewFromUtf8(isolate, "args[0] Error: is not number type."))) ;
                return ;
            }
            randomCount = args[0]->NumberValue() ;
        }
        ResetPool(randomCount) ;
        // args.GetReturnValue().Set(Number::New(isolate, randomCount)) ;
    }

    void IsHit (const FunctionCallbackInfo<Value>& args)
    {
        Isolate* isolate = args.GetIsolate() ;
        if (nIndex >= nCount)
        {
            ResetPool(0) ;
            nIndex = 0 ;
        }
        bool _ret = pArray[nIndex++] < nOdds ? true : false ;
        args.GetReturnValue().Set(Boolean::New(isolate, _ret)) ;
    }


    void init (Local<Object> exports)
    {
        NODE_SET_METHOD(exports, "Init", Init) ;
        NODE_SET_METHOD(exports, "Reset", Reset) ;
        NODE_SET_METHOD(exports, "IsHit", IsHit) ;
    }

    NODE_MODULE(addon, init) ;
} ;

